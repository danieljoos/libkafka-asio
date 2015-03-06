//
// detail/impl/offset_fetch_response_read.h
// ----------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_FETCH_RESPONSE_READ_H_EF46A204_533F_42EF_837F_877019C5989E
#define OFFSET_FETCH_RESPONSE_READ_H_EF46A204_533F_42EF_837F_877019C5989E

#include <libkafka_asio/primitives.h>
#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream& is,
                                MutableOffsetFetchResponse& response,
                                boost::system::error_code& ec)
{
  Int32 topics_size = ReadInt32(is);
  response.mutable_topics().resize(topics_size);
  for (Int32 i = 0; i < topics_size; ++i)
  {
    OffsetFetchResponse::Topic& topic = response.mutable_topics()[i];
    topic.topic_name = ReadString(is);
    Int32 partition_size = ReadInt32(is);
    topic.partitions.resize(partition_size);
    for (Int32 j = 0; j < partition_size; ++j)
    {
      OffsetFetchResponse::Topic::Partition& partition = topic.partitions[j];
      partition.partition = ReadInt32(is);
      partition.offset = ReadInt64(is);
      partition.metadata = ReadString(is);
      partition.error_code = ReadInt16(is);
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_FETCH_RESPONSE_READ_H_EF46A204_533F_42EF_837F_877019C5989E
