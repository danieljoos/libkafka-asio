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

#include <boost/foreach.hpp>
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
  int topic_count = ReadInt32(is);
  for (int t = 0; t < topic_count; ++t)
  {
    ProduceResponse::Topics::key_type key;
    ProduceResponse::Topics::mapped_type topic;
    key = ReadString(is);

    int partition_count = ReadInt32(is);
    for (int p = 0; p < partition_count; ++p)
    {
      ProduceResponse::Partitions::key_type key;
      ProduceResponse::Partitions::mapped_type partition;
      key = ReadInt32(is);
      partition.error_code = ReadInt16(is);
      partition.offset = ReadInt64(is);

      if (partition.error_code)
      {
        ec = (KafkaError) partition.error_code;
        return;
      }
      topic.partitions.insert(std::make_pair(key, partition));
    }
    response.mutable_topics().insert(std::make_pair(key, topic));
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // PRODUCE_RESPONSE_READ_H_9046BEF5_332B_4B67_8138_5964E836BF6C
