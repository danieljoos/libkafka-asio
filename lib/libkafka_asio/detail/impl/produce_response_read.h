//
// detail/impl/produce_response_read.h
// -----------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef PRODUCE_RESPONSE_READ_H_9046BEF5_332B_4B67_8138_5964E836BF6C
#define PRODUCE_RESPONSE_READ_H_9046BEF5_332B_4B67_8138_5964E836BF6C

#include <libkafka_asio/error.h>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream& is,
                                MutableProduceResponse& response,
                                boost::system::error_code& ec)
{
  // Topic Array
  Int32 topics_size = ReadInt32(is);
  for (Int32 i = 0; i < topics_size; ++i)
  {
    ProduceResponse::Topic topic;

    // Topic Name
    topic.topic_name = ReadString(is);

    // Partition Array
    Int32 partition_size = ReadInt32(is);
    for (Int32 j = 0; j < partition_size; ++j)
    {
      ProduceResponse::Topic::Partition topic_partition;
      topic_partition.partition = ReadInt32(is);
      topic_partition.error_code = ReadInt16(is);
      topic_partition.offset = ReadInt64(is);
      topic.partitions.push_back(topic_partition);

      if (topic_partition.error_code)
      {
        ec = (KafkaError) topic_partition.error_code;
        return;
      }
    }

    response.mutable_topics().push_back(topic);
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // PRODUCE_RESPONSE_READ_H_9046BEF5_332B_4B67_8138_5964E836BF6C
