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

inline void ReadResponseMessage(std::istream& is,
                                MutableOffsetResponse& response,
                                boost::system::error_code& ec)
{
  int topic_count = ReadInt32(is);
  for (int t = 0; t < topic_count; ++t)
  {
    OffsetResponse::Topics::key_type key;
    OffsetResponse::Topics::mapped_type topic;
    key = ReadString(is);

    int partition_count = ReadInt32(is);
    for (int p = 0; p < partition_count; ++p)
    {
      OffsetResponse::Partitions::key_type key;
      OffsetResponse::Partitions::mapped_type partition;
      key = ReadInt32(is);
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
      topic.partitions.insert(std::make_pair(key, partition));
    }
    response.mutable_topics().insert(std::make_pair(key, topic));
  }
}


}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_RESPONSE_READ_H_BAFF7FC5_03CA_46CD_B8EC_AD2CDBFF19F0
