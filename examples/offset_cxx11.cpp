//
// examples/offset_cxx11.cpp
// -------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//
// ----------------------------------
//
// This example shows how to create a 'OffsetRequest' to get the information
// about the latest offset of a specific topic-partition
// On success, this example prints the retrieved offset to stdout. Errors will
// be printed to stderr.
// Your compiler needs to know about C++11 and respective flags need to be set!
//

#include <iostream>
#include <boost/asio.hpp>
#include <libkafka_asio/libkafka_asio.h>

using libkafka_asio::Connection;
using libkafka_asio::OffsetRequest;
using libkafka_asio::OffsetResponse;

int main(int argc, char** argv)
{
  Connection::Configuration configuration;
  configuration.auto_connect = true;
  configuration.client_id = "libkafka_asio_example";
  configuration.socket_timeout = 2000;
  configuration.AddBrokerFromString("192.168.59.104:49156");

  boost::asio::io_service ios;
  Connection client(ios, configuration);

  // Request the latest offset for partition 1 of topic 'mytopic' on the
  // configured broker.
  using libkafka_asio::constants::kOffsetTimeLatest;
  OffsetRequest request;
  request.FetchTopicOffset("mytopic", 1, kOffsetTimeLatest);

  client.AsyncRequest(
    request,
    [&](const Connection::ErrorCodeType& err,
        const OffsetResponse::OptionalType& response)
    {
      if (err || !response)
      {
        std::cerr
          << "Error: " << boost::system::system_error(err).what()
          << std::endl;
        return;
      }
      auto offset = response->TopicPartitionOffset("mytopic", 1);
      if (!offset || offset->offsets.empty())
      {
        std::cerr << "Failed to fetch offset!" << std::endl;
        return;
      }
      std::cout
        << "Received latest offset: " << offset->offsets[0]
        << std::endl;
    });

  ios.run();
  return 0;
}