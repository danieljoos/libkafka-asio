//
// fetch_request.h
// ---------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef FETCH_REQUEST_H_BFC4C4C3_2D84_45C2_9FB5_78613B53A352
#define FETCH_REQUEST_H_BFC4C4C3_2D84_45C2_9FB5_78613B53A352

#include <vector>
#include <libkafka_asio/constants.h>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/request.h>
#include <libkafka_asio/fetch_response.h>
#include <libkafka_asio/detail/topics_partitions.h>

namespace libkafka_asio
{

// Kafka Fetch API request implementation
class FetchRequest :
  public Request<FetchRequest>
{
  friend class Request<FetchRequest>;

  static Int16 ApiKey();

  struct PartitionProperties
  {
    Int64 fetch_offset;
    Int32 max_bytes;
  };

  typedef detail::TopicsPartitionsVector<
    detail::EmptyProperties,
    PartitionProperties
  > TopicsPartitions;

public:
  typedef FetchResponse ResponseType;
  typedef MutableFetchResponse MutableResponseType;
  typedef TopicsPartitions::TopicType Topic;
  typedef TopicsPartitions::PartitionType Partition;
  typedef TopicsPartitions::TopicsType Topics;
  typedef TopicsPartitions::PartitionsType Partitions;

  FetchRequest();

  Int32 replica_id() const;

  Int32 max_wait_time() const;

  Int32 min_bytes() const;

  const Topics& topics() const;

  // Maximum time to wait for message data to become available on the server.
  // This option can be used in combination with the `min_bytes` parameter.
  // The timeout must be specified in milliseconds.
  void set_max_wait_time(Int32 max_wait_time);

  // Set the minimum number of bytes to wait for on the server side.
  // If this is set to 0, the server won't wait at all. If set to 1, the server
  // waits until 1 byte of the requested topic-partition data is available or
  // the specified timeout occurs.
  void set_min_bytes(Int32 min_bytes);

  // Fetch data for the specified topic-partition.
  // If such entry already exists in this Fetch request, it gets overridden.
  // Optionally, the offset to start the Fetch operation from, as well as the
  // maximum number of bytes to fetch, can be specified.
  void FetchTopic(const String& topic_name, Int32 partition,
                  Int64 fetch_offset = constants::kDefaultFetchOffset,
                  Int32 max_bytes = constants::kDefaultFetchMaxBytes);

  // Clears this Fetch request by removing all topic/partition entries.
  void Clear();

private:
  Int32 max_wait_time_;
  Int32 min_bytes_;
  Topics topics_;
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/fetch_request.h>

#endif  // FETCH_REQUEST_H_BFC4C4C3_2D84_45C2_9FB5_78613B53A352
