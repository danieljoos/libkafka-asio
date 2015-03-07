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
  response.mutable_topics().resize(ReadInt32(is));
  BOOST_FOREACH(ProduceResponse::Topic& topic, response.mutable_topics())
  {
    topic.topic_name = ReadString(is);
    topic.partitions.resize(ReadInt32(is));
    BOOST_FOREACH(ProduceResponse::Topic::Partition& partition,
                  topic.partitions)
    {
      partition.partition = ReadInt32(is);
      partition.error_code = ReadInt16(is);
      partition.offset = ReadInt64(is);

      if (partition.error_code)
      {
        ec = (KafkaError) partition.error_code;
        return;
      }
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // PRODUCE_RESPONSE_READ_H_9046BEF5_332B_4B67_8138_5964E836BF6C
