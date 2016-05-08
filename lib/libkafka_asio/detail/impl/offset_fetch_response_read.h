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

#include <boost/foreach.hpp>
#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream& is,
                                MutableOffsetFetchResponse& response,
                                boost::system::error_code& ec)
{
  // error code not used    
  (void)ec;
  int topic_count = ReadInt32(is);
  for (int t = 0; t < topic_count; ++t)
  {
    OffsetFetchResponse::Topics::key_type key;
    OffsetFetchResponse::Topics::mapped_type topic;
    key = ReadString(is);

    int partition_count = ReadInt32(is);
    for (int p = 0; p < partition_count; ++p)
    {
      OffsetFetchResponse::Partitions::key_type key;
      OffsetFetchResponse::Partitions::mapped_type partition;
      key = ReadInt32(is);
      partition.offset = ReadInt64(is);
      partition.metadata = ReadString(is);
      partition.error_code = ReadInt16(is);
      topic.partitions.insert(std::make_pair(key, partition));
    }
    response.mutable_topics().insert(std::make_pair(key, topic));
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_FETCH_RESPONSE_READ_H_EF46A204_533F_42EF_837F_877019C5989E
