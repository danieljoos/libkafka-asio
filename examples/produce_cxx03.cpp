//
// examples/produce_cxx03.cpp
// --------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//
// ----------------------------------
//
// This example shows how to prepare a 'Produce' request, connect to a Kafka 
// server and send the produce request. Errors will be reported to stderr.
//

#include <iostream>
#include <boost/asio.hpp>
#include <libkafka_asio/libkafka_asio.h>

using libkafka_asio::Connection;
using libkafka_asio::ProduceRequest;
using libkafka_asio::ProduceResponse;

void HandleRequest(const Connection::ErrorCodeType& err,
                   const ProduceResponse::OptionalType& response)
{
  if (err)
  {
    std::cerr
      << "Error: " << boost::system::system_error(err).what()
      << std::endl;
    return;
  }
  std::cout << "Successfully produced message!" << std::endl;
}

int main(int argc, char **argv)
{
  Connection::Configuration configuration;
  configuration.auto_connect = true;
  configuration.client_id = "libkafka_asio_example";
  configuration.socket_timeout = 10000;
  configuration.AddBrokerFromString("192.168.15.137:49162");
  configuration.AddBrokerFromString("192.168.15.137:49164");
  configuration.AddBrokerFromString("192.168.15.137:49166");

  boost::asio::io_service ios;
  Connection connection(ios, configuration);

  // Create a 'Produce' request and add a single message to it. The value of
  // that message is set to "Hello World". The message is produced for topic
  // "mytopic" and partition 0.
  ProduceRequest request;
  request.AddValue("Hello World", "mytopic", 0);

  // Send the prepared produce request.
  // The connection will attempt to automatically connect to one of the brokers,
  // specified in the configuration.
  connection.AsyncRequest(request, &HandleRequest);

  // Let's go!
  ios.run();
  return 0;
}
