//
// produce_response.h
// ------------------
//
// Copyright (c) 2015 Daniel Joos
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
#include <libkafka_asio/detail/topic_partition_block.h>

namespace libkafka_asio
{

class MutableProduceResponse;

// Kafka Produce API response implementation
class ProduceResponse :
  public Response<ProduceResponse>
{
  friend class MutableProduceResponse;

  struct TopicPartitionProperties
  {
    Int16 error_code;
    Int64 offset;
  };

public:
  typedef detail::TopicPartitionBlock<TopicPartitionProperties> Topic;
  typedef Topic::VectorType TopicVector;

  const TopicVector& topics() const
  {
    return topics_;
  }

  Topic::OptionalType FindTopic(const String& topic_name) const;

  Topic::Partition::OptionalType FindTopicPartition(const String& topic_name,
                                                    Int32 partition) const;

private:
  typedef detail::IsTopicWithName<Topic> IsTopicWithName;
  typedef detail::IsTopicPartition<Topic::Partition> IsTopicPartition;

  TopicVector topics_;
};

class MutableProduceResponse :
  public MutableResponse<ProduceResponse>
{
public:
  ProduceResponse::TopicVector& mutable_topics()
  {
    return response_.topics_;
  }
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/produce_response.h>

#endif  // PRODUCE_RESPONSE_H_8880F422_07A4_42F6_9B3B_D500A69EF83A
