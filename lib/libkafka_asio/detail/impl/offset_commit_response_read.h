//
// detail/impl/offset_commit_response_read.h
// -----------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_COMMIT_RESPONSE_READ_H_2BC25D29_FD96_4830_AD74_E2495AA55545
#define OFFSET_COMMIT_RESPONSE_READ_H_2BC25D29_FD96_4830_AD74_E2495AA55545

#include <boost/foreach.hpp>
#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream& is,
                                MutableOffsetCommitResponse& response,
                                boost::system::error_code& ec)
{
  // error code not used    
  (void)ec;
  int topic_count = ReadInt32(is);
  for (int t = 0; t < topic_count; ++t)
  {
    OffsetCommitResponse::Topics::key_type key;
    OffsetCommitResponse::Topics::mapped_type topic;
    key = ReadString(is);

    int partition_count = ReadInt32(is);
    for (int p = 0; p < partition_count; ++p)
    {
      OffsetCommitResponse::Partitions::key_type key;
      OffsetCommitResponse::Partitions::mapped_type partition;
      key = ReadInt32(is);
      partition.error_code = ReadInt16(is);
      topic.partitions.insert(std::make_pair(key, partition));
    }
    response.mutable_topics().insert(std::make_pair(key, topic));
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_COMMIT_RESPONSE_READ_H_2BC25D29_FD96_4830_AD74_E2495AA55545
