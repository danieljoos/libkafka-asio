//
// detail/basic_connection.h
// -------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef BASIC_CONNECTION_H_C3BEC97E_D24D_4938_BCAE_7A746C0989B4
#define BASIC_CONNECTION_H_C3BEC97E_D24D_4938_BCAE_7A746C0989B4

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <libkafka_asio/connection_configuration.h>

namespace libkafka_asio
{
namespace detail
{

template<typename Service>
class BasicConnection :
  public boost::asio::basic_io_object<Service>
{
  typedef typename Service::implementation_type::element_type
    ServiceImplType;

public:

  // Configuration type
  typedef ConnectionConfiguration Configuration;

  // Error Code Type
  typedef typename ServiceImplType::ErrorCodeType
    ErrorCodeType;

  // Connection handler type
  typedef typename ServiceImplType::ConnectionHandlerType
    ConnectionHandlerType;

  // Handler type deduction template
  template<typename TRequest>
  struct Handler
  {
    typedef typename ServiceImplType::template Handler<TRequest>::ResponseType
      ResponseType;
    typedef typename ServiceImplType::template Handler<TRequest>::Type
      Type;
  };

  // Create a new connection object.
  // Connection attempts and requests to the Kafka server will be scheduled
  // on the given io_service object.
  explicit BasicConnection(
    boost::asio::io_service& io_service,
    const Configuration& configuration = Configuration()) :
    boost::asio::basic_io_object<Service>(io_service)
  {
    this->get_implementation()->set_configuration(configuration);
  }

  // Closes the connection to the Kafka server
  void Close()
  {
    this->get_implementation()->Close();
  }

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
                    const ConnectionHandlerType& handler)
  {
    this->get_implementation()->AsyncConnect(host, service, handler);
  }

  // Asynchronously connects to the Kafka server, identified by the given
  // hostname and service. Both arguments will be casted to string using
  // lexical_cast.
  // The given handler function object will be called on success as well as
  // on error.
  // The function always returns immediately.
  // The signature of the handler function must be:
  // ```
  // void handler(
  //   const boost::system::error_code& error  // Success status
  // );
  // ```
  //
  template<typename Tx, typename Ty>
  void AsyncConnect(Tx host,
                    Ty service,
                    const ConnectionHandlerType& handler)
  {
    using boost::lexical_cast;
    this->AsyncConnect(lexical_cast<std::string>(host),
                       lexical_cast<std::string>(service),
                       handler);
  }

  // Tries to connect to the broker, specified in the configuration given to
  // this client object. If no such broker address was configured, the handler
  // function will be scheduled with ErrorNoBroker.
  // The function always returns immediately.
  // The signature of the handler function must be:
  // ```
  // void handler(
  //   const boost::system::error_code& error  // Success status
  // );
  // ```
  //
  void AsyncConnect(const ConnectionHandlerType& handler)
  {
    this->get_implementation()->AsyncConnect(handler);
  }

  // Asynchronously sends the given request to the connected Kafka server.
  // The given handler function object will be called on success as well as on
  // error condition.
  //
  // If this connection object is not in 'connected' state, the handler function
  // will be scheduled with ErrorNotConnected.
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
  //
  template<typename TRequest>
  void AsyncRequest(const TRequest& request,
                    const typename Handler<TRequest>::Type& handler)
  {
    this->get_implementation()->
      template AsyncRequest<TRequest>(request, handler);
  }

};

}  // namespace detail
}  // namespace libkafka_asio

#endif  // BASIC_CONNECTION_H_C3BEC97E_D24D_4938_BCAE_7A746C0989B4
