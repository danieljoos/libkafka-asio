//
// impl/client.h
// -------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CLIENT_H_ED1F1865_1B5B_4132_AA22_35A9C9AE433F
#define CLIENT_H_ED1F1865_1B5B_4132_AA22_35A9C9AE433F

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

inline Client::Client(boost::asio::io_service& io_service,
                      const Client::Configuration& configuration) :
  configuration_(configuration),
  state_(new Client::ClientState(kStateClosed)),
  io_service_(io_service),
  resolver_(io_service_),
  socket_(io_service_),
  deadline_(io_service_)
{
}

inline Client::~Client()
{
  Close();
  *state_ = kStateDestroyed;
}

inline void Client::AsyncConnect(const std::string& host,
                                 const std::string& service,
                                 const Client::ConnectionHandlerType& handler)
{
  if (*state_ != kStateClosed)
  {
    io_service_.post(boost::bind(handler, kErrorAlreadyConnected));
    return;
  }

  ResolverType::query query(host, service);
  resolver_.async_resolve(
    query,
    boost::bind(&Client::HandleAsyncResolve, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::iterator,
                state_,
                handler));
  *state_ = kStateConnecting;
  SetDeadline();
}

template<typename Tx, typename Ty>
inline void Client::AsyncConnect(Tx host,
                                 Ty service,
                                 const Client::ConnectionHandlerType& handler)
{
  using boost::lexical_cast;
  AsyncConnect(lexical_cast<std::string>(host),
               lexical_cast<std::string>(service),
               handler);
}

inline void Client::AsyncConnect(const Client::ConnectionHandlerType& handler)
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
void Client::AsyncRequest(
  const TRequest& request,
  const typename Client::Handler<TRequest>::Type& handler)
{
  if (*state_ == kStateClosed)
  {
    if (configuration_.auto_connect)
    {
      ConnectionHandlerType wrapped_handler = boost::bind(
        &Client::SendAsyncRequest<TRequest>, this, ::_1, request, handler);
      AsyncConnect(wrapped_handler);
      return;
    }
    typename TRequest::ResponseType::OptionalType empty_response;
    io_service_.post(boost::bind(handler, kErrorNotConnected, empty_response));
    return;
  }
  if (*state_ != kStateConnected)
  {
    typename TRequest::ResponseType::OptionalType empty_response;
    io_service_.post(boost::bind(handler, kErrorInProgress, empty_response));
    return;
  }
  SendAsyncRequest(kErrorSuccess, request, handler);
}

inline void Client::Close()
{
  *state_ = kStateClosed;
  boost::system::error_code ec;
  resolver_.cancel();
  socket_.shutdown(SocketType::shutdown_both, ec);
  socket_.close(ec);
  deadline_.cancel(ec);
}

inline void Client::SetDeadline()
{
  using boost::posix_time::milliseconds;
  deadline_.expires_from_now(milliseconds(configuration_.socket_timeout));
  deadline_.async_wait(
    boost::bind(&Client::HandleDeadline, this, state_));
}

inline void Client::AutoConnect(
  const Client::ConnectionHandlerType& handler,
  const Client::Configuration::BrokerList::const_iterator& broker_iter)
{
  ResolverType::query query(broker_iter->hostname, broker_iter->service);
  ConnectionHandlerType wrapped_handler = boost::bind(
      &Client::HandleAsyncAutoConnect, this, ::_1, handler, broker_iter);
  resolver_.async_resolve(
    query,
    boost::bind(&Client::HandleAsyncResolve, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::iterator,
                state_,
                wrapped_handler));
  *state_ = kStateConnecting;
  SetDeadline();
}

template<typename TRequest>
inline void Client::SendAsyncRequest(
  const Client::ErrorCodeType& error,
  const TRequest& request,
  const typename Client::Handler<TRequest>::Type& handler)
{
  StreambufType buffer(new StreambufType::element_type());
  std::ostream os(buffer.get());
  detail::WriteRequest(request, configuration_.client_id, os);

  bool response_expected = request.ResponseExpected();
  boost::asio::async_write(
    socket_, *buffer,
    boost::bind(&Client::HandleAsyncRequestWrite<TRequest>, this, 
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                buffer,
                state_,
                handler,
                response_expected));
  *state_ = kStateWriting;
  SetDeadline();
}

inline void Client::HandleAsyncResolve(
  const Client::ErrorCodeType& error,
  Client::ResolverType::iterator iter,
  const Client::SharedClientState& state,
  const Client::ConnectionHandlerType& handler)
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
    boost::bind(&Client::HandleAsyncConnect, this,
                boost::asio::placeholders::error,
                state_,
                handler));
  SetDeadline();
}

inline void Client::HandleAsyncConnect(
  const Client::ErrorCodeType& error,
  const Client::SharedClientState& state,
  const Client::ConnectionHandlerType& handler)
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

inline void Client::HandleAsyncAutoConnect(
  const Client::ErrorCodeType& error,
  const Client::ConnectionHandlerType& handler,
  Client::Configuration::BrokerList::const_iterator& broker_iter)
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
void Client::HandleAsyncRequestWrite(
  const Client::ErrorCodeType& error,
  size_t bytes_transferred,
  Client::StreambufType buffer,
  const Client::SharedClientState& state,
  const typename Client::Handler<TRequest>::Type& handler,
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
    return;
  }
  boost::asio::async_read(
    socket_,
    buffer->prepare(sizeof(Int32)),
    boost::asio::transfer_exactly(sizeof(Int32)),
    boost::bind(&Client::HandleAsyncResponseSizeRead<TRequest>, this, 
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                buffer,
                state_,
                handler));
  *state_ = kStateReading;
  SetDeadline();
}

template<typename TRequest>
void Client::HandleAsyncResponseSizeRead(
  const ErrorCodeType& error,
  size_t bytes_transferred,
  Client::StreambufType buffer,
  const Client::SharedClientState& state,
  const typename Client::Handler<TRequest>::Type& handler)
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
    boost::bind(&Client::HandleAsyncResponseRead<TRequest>, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                buffer,
                state_,
                handler));
  SetDeadline();
}

template<typename TRequest>
void Client::HandleAsyncResponseRead(
  const Client::ErrorCodeType& error,
  size_t bytes_transferred,
  Client::StreambufType buffer,
  const Client::SharedClientState& state,
  const typename Client::Handler<TRequest>::Type& handler)
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
  }
}

inline void Client::HandleDeadline(const Client::SharedClientState& state)
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

#endif  // CLIENT_H_ED1F1865_1B5B_4132_AA22_35A9C9AE433F
