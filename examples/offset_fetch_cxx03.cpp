//
// examples/offset_fetch_cxx03.cpp
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
// the library can be used in combination with promised/futures and is
// therefore running all IO related work inside of a separate thread.
// The example is mainly a port of the C++11-based example but is using the
// boost library instead of the C++11 standard library.
//

#include <boost/exception_ptr.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/system/system_error.hpp>
#include <libkafka_asio/libkafka_asio.h>

using libkafka_asio::Client;
using libkafka_asio::String;
using libkafka_asio::Int32;
using libkafka_asio::ConsumerMetadataRequest;
using libkafka_asio::ConsumerMetadataResponse;
using libkafka_asio::OffsetFetchRequest;
using libkafka_asio::OffsetFetchResponse;

// This will create handler functions which will set the value of the given
// promise on success or set the exception on error.
// The promise will be bound to the handler function.
template<typename T>
boost::function<
  void(const Client::ErrorCodeType&, const typename T::OptionalType&)>
PromiseHandler(boost::shared_ptr<boost::promise<T> > pr)
{
  struct local
  {
    static void Handle(const Client::ErrorCodeType& err,
                       const typename T::OptionalType& response,
                       boost::shared_ptr<boost::promise<T> > result)
    {
      if (err || !response)
      {
        result->set_exception(
          boost::copy_exception(boost::system::system_error(err)));
        return;
      }
      result->set_value(*response);
    }
  };
  return boost::bind(local::Handle, ::_1, ::_2, pr);
}

// Overload of the above function for boolean promises.
// We will use this as handler for the re-connect operation.
boost::function<void(const Client::ErrorCodeType&)>
PromiseHandler(boost::shared_ptr<boost::promise<bool> > pr)
{
  struct local
  {
    static void Handle(const Client::ErrorCodeType& err,
                       boost::shared_ptr<boost::promise<bool> > result)
    {
      if (err)
      {
        result->set_exception(
          boost::copy_exception(boost::system::system_error(err)));
        return;
      }
      result->set_value(true);
    }
  };
  return boost::bind(local::Handle, ::_1, pr);
}

// Discover the coordinating broker for the given consumer group.
// The function schedules a ConsumerMetadata request. The returned future
// object is used to wait for the requested data to be available.
boost::BOOST_THREAD_FUTURE<ConsumerMetadataResponse>
DiscoverCoordinator(Client& client, const String& consumer_group)
{
  boost::shared_ptr<boost::promise<ConsumerMetadataResponse> >
    result(new boost::promise<ConsumerMetadataResponse>());
  ConsumerMetadataRequest request;
  request.set_consumer_group(consumer_group);
  client.AsyncRequest(request, PromiseHandler(result));
  return result->get_future();
}

// Re-connects the given client to the provided hostname and port.
// This also shows how the client can be connected without scheduling a request.
// Again, the returned future is used to wait until the operation completes
// successfully.
boost::BOOST_THREAD_FUTURE<bool>
ReConnect(Client& client, const String& hostname, Int32 port)
{
  boost::shared_ptr<boost::promise<bool> > result(new boost::promise<bool>());
  client.Close();
  client.AsyncConnect(hostname, port, PromiseHandler(result));
  return result->get_future();
}

// Fetch offset data for the given consumer group and topic-partition.
// Same as above: the function returns a future object.
boost::BOOST_THREAD_FUTURE<OffsetFetchResponse>
FetchOffset(Client& client,
            const String& consumer_group,
            const String& topic_name,
            Int32 partition)
{
  boost::shared_ptr<boost::promise<OffsetFetchResponse> >
    result(new boost::promise<OffsetFetchResponse>());
  OffsetFetchRequest request;
  request.set_consumer_group(consumer_group);
  request.FetchOffset(topic_name, partition);
  client.AsyncRequest(request, PromiseHandler(result));
  return result->get_future();
}

// The starting point of our program...
int main(int argc, char **argv)
{
  // Run all IO work inside of another thread.
  // All request handlers are invoked from inside that thread as well.
  boost::asio::io_service ios;
  boost::asio::io_service::work work(ios);
  boost::thread worker(boost::bind(&boost::asio::io_service::run, &ios));

  // Construct a `libkafka_asio` client object
  Client::Configuration configuration;
  configuration.auto_connect = true;
  configuration.client_id = "libkafka_asio_example";
  configuration.socket_timeout = 2000;
  configuration.AddBrokerFromString("192.168.59.104:49158");
  String consumer_group = "ExampleGroup";
  String topic_name = "example";
  Int32 partition = 0;
  Client client(ios, configuration);

  try
  {
    // First get the broker that is currently coordinating the offset management
    ConsumerMetadataResponse coordinator =
      DiscoverCoordinator(client, consumer_group).get();

    // Then re-connect the client to the coordinator
    bool is_connected =
      ReConnect(client,
                coordinator.coordinator_host(),
                coordinator.coordinator_port()).get();

    // Finally, fetch offset data from the coordinator and print it
    if (is_connected)
    {
      OffsetFetchResponse offset =
        FetchOffset(client, consumer_group, topic_name, partition).get();
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
