//
// client.h
// --------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CLIENT_H_229D7905_40B7_49F1_BAC5_910B10FADDBA
#define CLIENT_H_229D7905_40B7_49F1_BAC5_910B10FADDBA

#include <string>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/client_configuration.h>

namespace libkafka_asio
{

//
// Kafka API client using the asio library. Client objects are used to connect
// to a Kafka server, send prepared requests and receive a response from the
// server.
// The class follows the asynchronous programming model of the asio library
// and uses callback function objects. The actual action (e.g. 'establish a
// connection' or 'send a request') requires a blocking call to the 
// `io_service::run()` method of the io_service object given to this client.
// Please see the documentation of boost::asio for further details on the asio
// programming model:
// http://www.boost.org/doc/libs/release/libs/asio/
//
class Client :
  private boost::noncopyable
{
  typedef boost::asio::ip::tcp::resolver ResolverType;
  typedef boost::asio::ip::tcp::socket SocketType;
  typedef boost::asio::deadline_timer DeadlineTimerType;
  typedef boost::shared_ptr<boost::asio::streambuf> StreambufType;

  enum ClientState
  {
    kStateClosed = 0,
    kStateConnecting,
    kStateConnected,
    kStateWriting,
    kStateReading
  };

public:
  // Configuration type
  typedef ClientConfiguration Configuration;
  
  // Error Code Type
  typedef boost::system::error_code ErrorCodeType;

  // Handler type deduction template.
  template<typename TRequest>
  struct Handler
  {
    typedef typename TRequest::ResponseType::OptionalType ResponseType;
    typedef boost::function<void(const ErrorCodeType&,
                                 const ResponseType&)> Type;
  };

  // Connection handler type
  typedef boost::function<void(const ErrorCodeType&)> ConnectionHandlerType;

  // Create a new client object.
  // Connection attempts and requests to the Kafka server will be scheduled
  // on the given io_service object.
  Client(boost::asio::io_service& io_service,
         const Configuration& configuration = Configuration());

  // A possibly open connection will be closed on destruction of client objects.
  // All pending asynchronous operations will be cancelled and the respective
  // handler functions will be called with `operation_aborted` error.
  ~Client();

  // Asynchronously connects to the Kafka server, identified by the given
  // hostname and port. The given handler function object will be called on
  // success as well as on error.
  // The function always returns immediately.
  // The signature of the handler function must be:
  // ```
  // void handler(
  //   const boost::system::error_code& error  // Success status
  // );
  // ```
  //
  void AsyncConnect(const std::string& host,
                    const std::string& service,
                    const ConnectionHandlerType& handler);

  // Tries to connect to the brokers, specified in the configuration given to
  // this client object. If no such broker address was configured, the handler
  // function will be scheduled with ErrorNoBroker.
  // Connection attempts will be made in the sequence, the broker addresses
  // were added to the configuration.
  // The function always returns immediately.
  // The signature of the handler function must be:
  // ```
  // void handler(
  //   const boost::system::error_code& error  // Success status
  // );
  // ```
  //
  void AsyncConnect(const ConnectionHandlerType& handler);

  // Asynchronously sends the given request to the connected Kafka server.
  // The given handler function object will be called on success as well as on
  // error condition.
  //
  // If this client object is not in 'connected' state, the handler function
  // will be scheduled with ErrorNotConnected.
  // If the 'auto-connect' option was enabled in the configuration, this
  // function will try to connect to one the brokers, specified in the
  // configuration (See function `AsyncConnect(handler)` above).
  //
  // The function always returns immediately.
  // The signature of the handler function must be:
  // ```
  // void handler(
  //   const boost::system::error_code& error,  // Success status
  //   const Response::OptionalType& response   // Optional response object
  // );
  // ```
  // The type of the optional response object, handed to the handler function,
  // depends on the given request type.
  // Please see the documentation of the boost::optional library for further
  // details about optional objects:
  // http://www.boost.org/doc/libs/release/libs/optional/
  //
  template<typename TRequest>
  void AsyncRequest(const TRequest& request,
                    const typename Handler<TRequest>::Type& handler);

  // Closes the connection to the Kafka server.
  // All asynchronous operations will be cancelled immediately with an
  // `operation_aborted` error.
  //
  void Close();

private:
  Client(const Client&);

  Client& operator=(const Client&);

  // Resets the socket operation timeout
  void SetDeadline();

  // Attempt to connect to the broken, pointed to by the given iterator
  void AutoConnect(
    const ConnectionHandlerType& handler,
    const Configuration::BrokerList::const_iterator& broker_iter);

  // Send the given request. At this point, a connection is required
  template<typename TRequest>
  void SendAsyncRequest(const ErrorCodeType& error,
                        const TRequest& request,
                        const typename Handler<TRequest>::Type& handler);

  // Handle async resolve operations
  void HandleAsyncResolve(const ErrorCodeType& error,
                          ResolverType::iterator iter,
                          const ConnectionHandlerType& handler);

  // Handle async connect operations
  void HandleAsyncConnect(const ErrorCodeType& error,
                          const ConnectionHandlerType& handler);

  // Handle auto-connect. Tries the next broker on error.
  void HandleAsyncAutoConnect(
    const ErrorCodeType& error,
    const ConnectionHandlerType& handler,
    Configuration::BrokerList::const_iterator& broker_iter);

  // Handle async request write operations
  template<typename TRequest>
  void HandleAsyncRequestWrite(
    const ErrorCodeType& error,
    size_t bytes_transferred,
    StreambufType buffer,
    const typename Handler<TRequest>::Type& handler,
    bool response_expected);

  // Handle async read of response size
  template<typename TRequest>
  void HandleAsyncResponseSizeRead(
    const ErrorCodeType& error,
    size_t bytes_transferred,
    StreambufType buffer,
    const typename Handler<TRequest>::Type& handler);

  // Handle async read of response body
  template<typename TRequest>
  void HandleAsyncResponseRead(
    const ErrorCodeType& error,
    size_t bytes_transferred,
    StreambufType buffer,
    const typename Handler<TRequest>::Type& handler);

  // Handle socket operation timeout
  void HandleDeadline();

  Configuration configuration_;
  ClientState state_;
  boost::asio::io_service& io_service_;
  ResolverType resolver_;
  SocketType socket_;
  DeadlineTimerType deadline_;
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/client.h>

#endif  // CLIENT_H_229D7905_40B7_49F1_BAC5_910B10FADDBA
