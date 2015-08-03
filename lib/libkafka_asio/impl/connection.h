//
// impl/connection.h
// -------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CONNECTION_H_ED1F1865_1B5B_4132_AA22_35A9C9AE433F
#define CONNECTION_H_ED1F1865_1B5B_4132_AA22_35A9C9AE433F

#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <libkafka_asio/constants.h>
#include <libkafka_asio/detail/request_write.h>
#include <libkafka_asio/detail/response_read.h>
#include <libkafka_asio/error.h>

namespace libkafka_asio
{

inline Connection::Connection(boost::asio::io_service& io_service,
                      const Connection::Configuration& configuration) :
  configuration_(configuration),
  state_(new Connection::ConnectionState(kStateClosed)),
  io_service_(io_service),
  resolver_(io_service_),
  socket_(io_service_),
  deadline_(io_service_)
{
}

inline Connection::~Connection()
{
  Close();
  *state_ = kStateDestroyed;
}

inline void Connection::AsyncConnect(const std::string& host,
                                 const std::string& service,
                                 const Connection::ConnectionHandlerType& handler)
{
  if (*state_ != kStateClosed)
  {
    io_service_.post(boost::bind(handler, kErrorAlreadyConnected));
    return;
  }

  ResolverType::query query(host, service);
  resolver_.async_resolve(
    query,
    boost::bind(&Connection::HandleAsyncResolve, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::iterator,
                state_,
                handler));
  *state_ = kStateConnecting;
  SetDeadline();
}

template<typename Tx, typename Ty>
inline void Connection::AsyncConnect(Tx host,
                                 Ty service,
                                 const Connection::ConnectionHandlerType& handler)
{
  using boost::lexical_cast;
  AsyncConnect(lexical_cast<std::string>(host),
               lexical_cast<std::string>(service),
               handler);
}

inline void Connection::AsyncConnect(const Connection::ConnectionHandlerType& handler)
{
  if (*state_ != kStateClosed)
  {
    io_service_.post(boost::bind(handler, kErrorAlreadyConnected));
    return;
  }
  if (configuration_.broker_list.empty())
  {
    io_service_.post(boost::bind(handler, kErrorNoBroker));
    return;
  }

  Configuration::BrokerList::const_iterator broker_iter =
    configuration_.broker_list.begin();
  AutoConnect(handler, broker_iter);
}

template<typename TRequest>
void Connection::AsyncRequest(
  const TRequest& request,
  const typename Connection::Handler<TRequest>::Type& handler)
{
  SerializeAndEnqueue(request, handler);
  if (*state_ == kStateClosed)
  {
    if (configuration_.auto_connect)
    {
      ConnectionHandlerType wrapped_handler = boost::bind(
        &Connection::SendNextRequest, this, ::_1, state_);
      AsyncConnect(wrapped_handler);
      return;
    }
    typename TRequest::ResponseType::OptionalType empty_response;
    io_service_.post(boost::bind(handler, kErrorNotConnected, empty_response));
    return;
  }
  SendNextRequest(kErrorSuccess, state_);
}

inline void Connection::Close()
{
  *state_ = kStateClosed;
  boost::system::error_code ec;
  resolver_.cancel();
  socket_.shutdown(SocketType::shutdown_both, ec);
  socket_.close(ec);
  deadline_.cancel(ec);
}

inline void Connection::SetDeadline()
{
  using boost::posix_time::milliseconds;
  deadline_.expires_from_now(milliseconds(configuration_.socket_timeout));
  deadline_.async_wait(
    boost::bind(&Connection::HandleDeadline, this, state_));
}

inline void Connection::AutoConnect(
  const Connection::ConnectionHandlerType& handler,
  const Connection::Configuration::BrokerList::const_iterator& broker_iter)
{
  ResolverType::query query(broker_iter->hostname, broker_iter->service);
  ConnectionHandlerType wrapped_handler = boost::bind(
      &Connection::HandleAsyncAutoConnect, this, ::_1, handler, broker_iter);
  resolver_.async_resolve(
    query,
    boost::bind(&Connection::HandleAsyncResolve, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::iterator,
                state_,
                wrapped_handler));
  *state_ = kStateConnecting;
  SetDeadline();
}

inline void Connection::SendNextRequest(
  const Connection::ErrorCodeType& error,
  const Connection::SharedConnectionState& state)
{
  if (*state == kStateDestroyed)
  {
    return;
  }
  //lock scope
  {
    ScopedLockType scopedLock(write_queue_mtx_);
    if (error)
    {
      while (!write_queue_.empty())
      {
        WriteQueueItem& item = write_queue_.front();
        io_service_.post(boost::bind(item.write_handler, error, 0));
        write_queue_.pop_front();
      }
      Close();
      return;
    }
    if (*state != kStateConnected ||
        write_queue_.empty())
    {
      return;
    }
    WriteQueueItem& item = write_queue_.front();
    boost::asio::async_write(socket_, *item.buffer, item.write_handler);
    write_queue_.pop_front();
  }
  *state_ = kStateWriting;
  SetDeadline();
}

template<typename TRequest>
inline void Connection::SerializeAndEnqueue(
  const TRequest& request,
  const typename Connection::Handler<TRequest>::Type& handler)
{
  // serialize
  StreambufType buffer(new StreambufType::element_type());
  std::ostream os(buffer.get());
  detail::WriteRequest(request, configuration_.client_id, os);
  // and enqueue
  WriteQueueItem item;
  item.buffer = buffer;
  bool response_expected = request.ResponseExpected();
  item.write_handler = 
    boost::bind(&Connection::HandleAsyncRequestWrite<TRequest>, this, 
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                buffer,
                state_,
                handler,
                response_expected);
  ScopedLockType scopedLock(write_queue_mtx_);
  write_queue_.push_back(item);
}

inline void Connection::HandleAsyncResolve(
  const Connection::ErrorCodeType& error,
  Connection::ResolverType::iterator iter,
  const Connection::SharedConnectionState& state,
  const Connection::ConnectionHandlerType& handler)
{
  if (*state == kStateDestroyed)
  {
    return;
  }
  if (error)
  {
    io_service_.post(boost::bind(handler, error));
    Close();
    return;
  }
  boost::asio::async_connect(
    socket_, iter,
    boost::bind(&Connection::HandleAsyncConnect, this,
                boost::asio::placeholders::error,
                state_,
                handler));
  SetDeadline();
}

inline void Connection::HandleAsyncConnect(
  const Connection::ErrorCodeType& error,
  const Connection::SharedConnectionState& state,
  const Connection::ConnectionHandlerType& handler)
{
  if (*state == kStateDestroyed)
  {
    return;
  }
  if (error)
  {
    Close();
  }
  else
  {
    *state_ = kStateConnected;
    deadline_.cancel();
  }
  io_service_.post(boost::bind(handler, error));
}

inline void Connection::HandleAsyncAutoConnect(
  const Connection::ErrorCodeType& error,
  const Connection::ConnectionHandlerType& handler,
  Connection::Configuration::BrokerList::const_iterator& broker_iter)
{
  if (error)
  {
    broker_iter++;
    if (broker_iter != configuration_.broker_list.end())
    {
      AutoConnect(handler, broker_iter);
      return;
    }
  }
  io_service_.post(boost::bind(handler, error));
}

template<typename TRequest>
void Connection::HandleAsyncRequestWrite(
  const Connection::ErrorCodeType& error,
  size_t bytes_transferred,
  Connection::StreambufType buffer,
  const Connection::SharedConnectionState& state,
  const typename Connection::Handler<TRequest>::Type& handler,
  bool response_expected)
{
  if (*state == kStateDestroyed)
  {
    return;
  }
  typedef typename TRequest::ResponseType::OptionalType OptionalResponse;
  buffer->consume(buffer->size());
  if (error)
  {
    OptionalResponse empty_response;
    io_service_.post(boost::bind(handler, error, empty_response));
    Close();
    return;
  }
  else if (*state_ == kStateClosed)
  {
    OptionalResponse empty_response;
    io_service_.post(boost::bind(handler, kErrorNotConnected, empty_response));
    return;
  }
  else if (!response_expected)
  {
    OptionalResponse empty_response;
    io_service_.post(boost::bind(handler, error, empty_response));
    *state_ = kStateConnected;
    deadline_.cancel();
    SendNextRequest(kErrorSuccess, state_);
    return;
  }
  boost::asio::async_read(
    socket_,
    buffer->prepare(sizeof(Int32)),
    boost::asio::transfer_exactly(sizeof(Int32)),
    boost::bind(&Connection::HandleAsyncResponseSizeRead<TRequest>, this, 
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                buffer,
                state_,
                handler));
  *state_ = kStateReading;
  SetDeadline();
}

template<typename TRequest>
void Connection::HandleAsyncResponseSizeRead(
  const ErrorCodeType& error,
  size_t bytes_transferred,
  Connection::StreambufType buffer,
  const Connection::SharedConnectionState& state,
  const typename Connection::Handler<TRequest>::Type& handler)
{
  if (*state == kStateDestroyed)
  {
    return;
  }
  typedef typename TRequest::ResponseType::OptionalType OptionalResponse;
  buffer->commit(bytes_transferred);
  if (error)
  {
    OptionalResponse empty_response;
    io_service_.post(boost::bind(handler, error, empty_response));
    Close();
    return;
  }
  else if (*state_ == kStateClosed)
  {
    OptionalResponse empty_response;
    io_service_.post(boost::bind(handler, kErrorNotConnected, empty_response));
    return;
  }

  std::istream is(buffer.get());
  Int32 size = detail::ReadInt32(is);
  if (size >= configuration_.message_max_bytes)
  {
    OptionalResponse empty_response;
    io_service_.post(boost::bind(handler, kErrorMessageSizeTooLarge,
                                 empty_response));
    Close();
    return;
  }

  boost::asio::async_read(
    socket_,
    buffer->prepare(size),
    boost::asio::transfer_exactly(size),
    boost::bind(&Connection::HandleAsyncResponseRead<TRequest>, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                buffer,
                state_,
                handler));
  SetDeadline();
}

template<typename TRequest>
void Connection::HandleAsyncResponseRead(
  const Connection::ErrorCodeType& error,
  size_t bytes_transferred,
  Connection::StreambufType buffer,
  const Connection::SharedConnectionState& state,
  const typename Connection::Handler<TRequest>::Type& handler)
{
  if (*state == kStateDestroyed)
  {
    return;
  }
  typedef typename TRequest::ResponseType::OptionalType OptionalResponse;
  buffer->commit(bytes_transferred);
  if (error)
  {
    OptionalResponse empty_response;
    io_service_.post(boost::bind(handler, error, empty_response));
    Close();
    return;
  }

  std::istream is(buffer.get());
  typename TRequest::MutableResponseType response;
  boost::system::error_code ec;
  detail::ReadResponse(is, response, ec);
  if (ec)
  {
    OptionalResponse empty_response;
    io_service_.post(boost::bind(handler, ec, empty_response));
    Close();
  }
  else
  {
    io_service_.post(boost::bind(handler, ec, response.response()));
    *state_ = kStateConnected;
    deadline_.cancel();
    SendNextRequest(kErrorSuccess, state_);
  }
}

inline void Connection::HandleDeadline(const Connection::SharedConnectionState& state)
{
  if (*state == kStateDestroyed ||
      *state == kStateClosed || 
      *state == kStateConnected)
  {
    return;
  }
  if (deadline_.expires_at() <= DeadlineTimerType::traits_type::now())
  {
    // Socket operation timed out!
    deadline_.expires_at(boost::posix_time::pos_infin);
    Close();
  }
}

}  // namespace libkafka_asio

#endif  // CONNECTION_H_ED1F1865_1B5B_4132_AA22_35A9C9AE433F
