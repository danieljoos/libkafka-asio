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

#include <boost/foreach.hpp>
#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream& is,
                                MutableFetchResponse& response,
                                boost::system::error_code& ec)
{
  // Topic array
  int topic_count = ReadInt32(is);
  for (int t = 0; t < topic_count; ++t)
  {
    FetchResponse::Topics::key_type key;
    FetchResponse::Topics::mapped_type topic;
    key = ReadString(is);

    // Partitions array
    int partition_count = ReadInt32(is);
    for (int p = 0; p < partition_count; ++p)
    {
      FetchResponse::Partitions::key_type key;
      FetchResponse::Partitions::mapped_type partition;
      key = ReadInt32(is);
      partition.error_code = ReadInt16(is);
      partition.highwater_mark_offset = ReadInt64(is);

      if (partition.error_code)
      {
        ec = (KafkaError) partition.error_code;
        return;
      }

      // MessageSet
      Int32 message_set_size = ReadInt32(is);
      ReadMessageSet(is, partition.messages, message_set_size, ec);
      if (ec != kErrorSuccess)
      {
        return;
      }
      topic.partitions.insert(std::make_pair(key, partition));
    }
    response.mutable_topics().insert(std::make_pair(key, topic));
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // FETCH_RESPONSE_READ_H_03C2A581_53D0_498E_AF6A_2082227485A3
