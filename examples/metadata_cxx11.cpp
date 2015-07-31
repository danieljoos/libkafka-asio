//
// examples/metadata_cxx11.cpp
// ---------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//
// ----------------------------------
//
// This example shows how to create a 'TopicMetadataRequest'. It can be used
// to determine the leader for a specific topic-partition.
// On success, this example prints the leader host:port to stdout. Errors will
// be printed to stderr.
// Your compiler needs to know about C++11 and respective flags need to be set!
//

#include <iostream>
#include <boost/asio.hpp>
#include <libkafka_asio/libkafka_asio.h>

using libkafka_asio::Connection;
using libkafka_asio::MetadataRequest;
using libkafka_asio::MetadataResponse;

int main(int argc, char** argv)
{
  Connection::Configuration configuration;
  configuration.auto_connect = true;
  configuration.client_id = "libkafka_asio_example";
  configuration.socket_timeout = 2000;
  configuration.AddBrokerFromString("192.168.59.104:49156");

  boost::asio::io_service ios;
  Connection connection(ios, configuration);

  MetadataRequest request;
  request.AddTopicName("mytopic");

  connection.AsyncRequest(
    request,
    [&](const Connection::ErrorCodeType& err,
        const MetadataResponse::OptionalType& response)
    {
      if (err || !response)
      {
        std::cerr
          << "Error: " << boost::system::system_error(err).what()
          << std::endl;
        return;
      }
      // Find the leader for topic 'mytopic' and partition 0
      auto leader = response->PartitionLeader("mytopic", 0);
      if (!leader)
      {
        std::cerr << "No leader found!" << std::endl;
        return;
      }
      std::cout
        << "Found leader: " << leader->host << ":" << leader->port
        << std::endl;
    });

  ios.run();
  return 0;
}
