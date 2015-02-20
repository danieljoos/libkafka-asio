//
// examples/fetch_cxx11.cpp
// ------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//
// ----------------------------------
//
// This example shows how to create a 'FetchRequest' to get messages for a
// specific Topic & partition. On success, all received messages will be print
// to stdout.
// Your compiler needs to know about C++11 and respective flags need to be set!
//

#include <iostream>
#include <boost/asio.hpp>
#include <libkafka_asio/libkafka_asio.h>

using libkafka_asio::Client;
using libkafka_asio::FetchRequest;
using libkafka_asio::FetchResponse;
using libkafka_asio::MessageAndOffset;

int main(int argc, char **argv)
{
  Client::Configuration configuration;
  configuration.auto_connect = true;
  configuration.client_id = "libkafka_asio_example";
  configuration.socket_timeout = 10000;
  configuration.AddBrokerFromString("192.168.15.137:49162");

  boost::asio::io_service ios;
  Client client(ios, configuration);

  // Create a 'Fetch' request and try to get data for partition 0 of topic
  // 'mytopic', starting with offset 1
  FetchRequest request;
  request.FetchTopic("mytopic", 0, 1);

  // Helper to interpret the received bytes as string
  auto BytesToString = [](const libkafka_asio::Bytes& bytes) -> std::string
  {
    if (!bytes || bytes->empty())
    {
      return "";
    }
    return std::string((const char*) &(*bytes)[0], bytes->size());
  };

  // Send the prepared fetch request.
  // The client will attempt to automatically connect to the broker, specified
  // in the configuration.
  client.AsyncRequest(
    request,
    [&](const Client::ErrorCodeType& err,
        const FetchResponse::OptionalType& response)
  {
    if (err)
    {
      std::cerr
        << "Error: " << boost::system::system_error(err).what()
        << std::endl;
      return;
    }

    // Loop through the received messages.
    // A range based for loop might also work.
    std::for_each(response->begin(), response->end(),
      [&](const MessageAndOffset& message)
    {
      std::cout << BytesToString(message.value()) << std::endl;
    });
  });

  // Let's go!
  ios.run();
  return 0;
}
