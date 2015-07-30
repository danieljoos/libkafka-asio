//
// examples/offset_fetch_cxx11.cpp
// -------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//
// ----------------------------------
//
// This example shows how to fetch offset data for a consumer group using the
// 'OffsetFetch' API of Kafka 0.8.2. On the other side it demonstrates how
// the library can be used in combination with C++11 promised/futures and is
// therefore running all IO related work inside of a separate thread.
// Your compiler needs to know about C++11 and respective flags need to be set!
//

#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/system/system_error.hpp>
#include <libkafka_asio/libkafka_asio.h>

using boost::lexical_cast;
using boost::system::system_error;
using libkafka_asio::Connection;
using libkafka_asio::String;
using libkafka_asio::Int32;
using libkafka_asio::ConsumerMetadataRequest;
using libkafka_asio::ConsumerMetadataResponse;
using libkafka_asio::OffsetFetchRequest;
using libkafka_asio::OffsetFetchResponse;


// This will create handler functions which will set the value of the given
// promise on success or set the exception on error.
// The promise will be bound to the handler function.
// The promise rvalue is therefore moved into a promise, wrapped by a shared
// pointer.
template<typename T>
std::function<
  void(const Connection::ErrorCodeType&, const typename T::OptionalType&)>
PromiseHandler(std::promise<T>&& pr)
{
  typedef std::promise<T> PromiseType;
  typedef std::shared_ptr<PromiseType> SharedPromiseType;
  return std::bind(
    [](const Connection::ErrorCodeType& err,
       const typename T::OptionalType& response,
       SharedPromiseType result)
    {
      if (err || !response)
      {
        result->set_exception(std::make_exception_ptr(system_error(err)));
        return;
      }
      result->set_value(*response);
    },
    std::placeholders::_1,
    std::placeholders::_2,
    SharedPromiseType(new PromiseType(std::move(pr)))
  );
}

// MAIN!
int main(int argc, char **argv)
{
  // Run all IO work inside of another thread.
  // All request handlers are invoked from inside that thread as well.
  boost::asio::io_service ios;
  boost::asio::io_service::work work(ios);
  std::thread worker([&ios]() { ios.run(); });

  // Construct a `libkafka_asio` connection object
  Connection::Configuration configuration;
  configuration.auto_connect = true;
  configuration.client_id = "libkafka_asio_example";
  configuration.socket_timeout = 2000;
  configuration.AddBrokerFromString("192.168.59.103:49154");
  String consumer_group = "ExampleGroup";
  String topic_name = "example";
  Int32 partition = 0;
  Connection connection(ios, configuration);

  // Discover the coordinator broker.
  // It uses a promise, which will be set inside the handler function of the
  // request. This function returns the associated future object.
  auto discover_coordinator = [&]() -> std::future<ConsumerMetadataResponse>
    {
      std::promise<ConsumerMetadataResponse> result;
      auto ret = result.get_future();
      ConsumerMetadataRequest request;
      request.set_consumer_group(consumer_group);
      connection.AsyncRequest(request, PromiseHandler(std::move(result)));
      return ret;
    };

  // Fetch offset data for the above consumer group. Again using a promise.
  auto fetch_offset = [&]() -> std::future<OffsetFetchResponse>
    {
      std::promise<OffsetFetchResponse> result;
      auto ret = result.get_future();
      OffsetFetchRequest request;
      request.set_consumer_group(consumer_group);
      request.FetchOffset(topic_name, partition);
      connection.AsyncRequest(request, PromiseHandler(std::move(result)));
      return ret;
    };

  try
  {
    // First get the broker that is currently coordinating the offset management
    auto coordinator = discover_coordinator().get();

    // Then re-connect the connection to the coordinator
    std::promise<bool> connected;
    auto is_connected = connected.get_future();
    connection.Close();
    connection.AsyncConnect(
      coordinator.coordinator_host(),
      lexical_cast<String>(coordinator.coordinator_port()),
      [&](const Connection::ErrorCodeType& err)
      {
        if (err)
        {
          connected.set_exception(std::make_exception_ptr(system_error(err)));
          return;
        }
        connected.set_value(true);
      });

    // Finally, fetch offset data from the coordinator and print it
    if (is_connected.get())
    {
      auto offset = fetch_offset().get();
      std::cout
        << "Offset: " << offset.topics()[0].partitions[0].offset
        << std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  ios.stop();
  worker.join();
  return 0;
}
