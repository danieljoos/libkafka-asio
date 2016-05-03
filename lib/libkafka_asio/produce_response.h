//
// produce_response.h
// ------------------
//
// Copyright (c) 2015-2016 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef PRODUCE_RESPONSE_H_8880F422_07A4_42F6_9B3B_D500A69EF83A
#define PRODUCE_RESPONSE_H_8880F422_07A4_42F6_9B3B_D500A69EF83A

#include <vector>
#include <boost/optional.hpp>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/response.h>
#include <libkafka_asio/detail/functional.h>
#include <libkafka_asio/detail/topics_partitions.h>

namespace libkafka_asio
{

class MutableProduceResponse;

// Kafka Produce API response implementation
class ProduceResponse :
  public Response<ProduceResponse>
{
  friend class MutableProduceResponse;

  struct PartitionProperties
  {
    Int16 error_code;
    Int64 offset;
  };

  typedef detail::TopicsPartitionsMap<
    detail::EmptyProperties,
    PartitionProperties
  > TopicsPartitions;

public:
  typedef TopicsPartitions::TopicType Topic;
  typedef TopicsPartitions::PartitionType Partition;
  typedef TopicsPartitions::TopicsType Topics;
  typedef TopicsPartitions::PartitionsType Partitions;

  const Topics& topics() const
  {
    return topics_;
  }

  Topic::OptionalType FindTopic(const String& topic_name) const;

  Partition::OptionalType FindTopicPartition(const String& topic_name,
                                             Int32 partition) const;

private:
  Topics topics_;
};

class MutableProduceResponse :
  public MutableResponse<ProduceResponse>
{
public:
  ProduceResponse::Topics& mutable_topics()
  {
    return response_.topics_;
  }
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/produce_response.h>

#endif  // PRODUCE_RESPONSE_H_8880F422_07A4_42F6_9B3B_D500A69EF83A
