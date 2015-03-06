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

#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream & is,
                                MutableOffsetResponse & response,
                                boost::system::error_code & ec)
{
  Int32 topics_size = ReadInt32(is);
  for (Int32 i = 0; i < topics_size; ++i)
  {
    OffsetResponse::Topic topic;
    topic.topic_name = ReadString(is);

    Int32 partitions_size = ReadInt32(is);
    for (Int32 j = 0; j < partitions_size; ++j)
    {
      OffsetResponse::Topic::Partition partition_offset;
      partition_offset.partition = ReadInt32(is);
      partition_offset.error_code = ReadInt16(is);

      if (partition_offset.error_code)
      {
        ec = (KafkaError) partition_offset.error_code;
        return;
      }

      Int32 offsets_size = ReadInt32(is);
      for (Int32 k = 0; k < offsets_size; ++k)
      {
        partition_offset.offsets.push_back(ReadInt64(is));
      }

      topic.partitions.push_back(partition_offset);
    }
    response.mutable_topics().push_back(topic);
  }
}


}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_RESPONSE_READ_H_BAFF7FC5_03CA_46CD_B8EC_AD2CDBFF19F0
