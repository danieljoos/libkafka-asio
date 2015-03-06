//
// detail/impl/fetch_response_read.h
// ---------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef FETCH_RESPONSE_READ_H_03C2A581_53D0_498E_AF6A_2082227485A3
#define FETCH_RESPONSE_READ_H_03C2A581_53D0_498E_AF6A_2082227485A3

#include <libkafka_asio/detail/response_read.h>
#include <libkafka_asio/fetch_response.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream& is,
                                MutableFetchResponse& response,
                                boost::system::error_code& ec)
{
  // Topic array
  Int32 topics_size = ReadInt32(is);
  response.mutable_topics().resize(topics_size);
  for (Int32 i = 0; i < topics_size; ++i)
  {
    FetchResponse::Topic& topic = response.mutable_topics()[i];
    topic.topic_name = ReadString(is);

    // Partitions array
    Int32 partitions_size = ReadInt32(is);
    topic.partitions.resize(partitions_size);
    for (Int32 j = 0; j < partitions_size; ++j)
    {
      FetchResponse::Topic::Partition& partition = topic.partitions[j];
      partition.partition = ReadInt32(is);
      partition.error_code = ReadInt16(is);
      partition.highwater_mark_offset = ReadInt64(is);

      if (partition.error_code)
      {
        ec = (KafkaError) partition.error_code;
        return;
      }

      // MessageSet
      Int32 message_set_size = ReadInt32(is);
      ReadMessageSet(is, partition.messages, message_set_size);
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // FETCH_RESPONSE_READ_H_03C2A581_53D0_498E_AF6A_2082227485A3
