//
// detail/impl/offset_response_read.h
// ----------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_RESPONSE_READ_H_BAFF7FC5_03CA_46CD_B8EC_AD2CDBFF19F0
#define OFFSET_RESPONSE_READ_H_BAFF7FC5_03CA_46CD_B8EC_AD2CDBFF19F0

#include <boost/foreach.hpp>
#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream & is,
                                MutableOffsetResponse & response,
                                boost::system::error_code & ec)
{
  response.mutable_topics().resize(ReadInt32(is));
  BOOST_FOREACH(OffsetResponse::Topic& topic, response.mutable_topics())
  {
    topic.topic_name = ReadString(is);
    topic.partitions.resize(ReadInt32(is));
    BOOST_FOREACH(OffsetResponse::Topic::Partition& partition,
                  topic.partitions)
    {
      partition.partition = ReadInt32(is);
      partition.error_code = ReadInt16(is);
      if (partition.error_code)
      {
        ec = (KafkaError) partition.error_code;
        return;
      }
      Int32 offsets_size = ReadInt32(is);
      for (Int32 k = 0; k < offsets_size; ++k)
      {
        partition.offsets.push_back(ReadInt64(is));
      }
    }
  }
}


}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_RESPONSE_READ_H_BAFF7FC5_03CA_46CD_B8EC_AD2CDBFF19F0
