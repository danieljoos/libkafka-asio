//
// offset_fetch_request.h
// ----------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_FETCH_REQUEST_H_507D083D_1B17_43CF_9D84_48480E9E44F7
#define OFFSET_FETCH_REQUEST_H_507D083D_1B17_43CF_9D84_48480E9E44F7

#include <set>
#include <vector>
#include <libkafka_asio/request.h>
#include <libkafka_asio/offset_fetch_response.h>
#include <libkafka_asio/detail/topics_partitions.h>

namespace libkafka_asio
{

// Kafka Offset Commit/Fetch API request implementation:
// Offset fetch request
class OffsetFetchRequest :
  public Request<OffsetFetchRequest>
{
  friend class Request<OffsetFetchRequest>;

  static Int16 ApiKey();

  typedef detail::TopicsPartitionsVector<
    detail::EmptyProperties,
    detail::EmptyProperties
  > TopicsPartitions;

public:
  typedef OffsetFetchResponse ResponseType;
  typedef MutableOffsetFetchResponse MutableResponseType;
  typedef TopicsPartitions::TopicType Topic;
  typedef TopicsPartitions::PartitionType Partition;
  typedef TopicsPartitions::TopicsType Topics;
  typedef TopicsPartitions::PartitionsType Partitions;

  const String& consumer_group() const;

  const Topics& topics() const;

  void set_consumer_group(const String& consumer_group);

  void FetchOffset(const String& topic_name, Int32 partition);

private:
  String consumer_group_;
  Topics topics_;
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/offset_fetch_request.h>

#endif  // OFFSET_FETCH_REQUEST_H_507D083D_1B17_43CF_9D84_48480E9E44F7
