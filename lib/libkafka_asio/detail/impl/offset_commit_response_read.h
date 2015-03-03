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

#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream& is,
                                MutableOffsetCommitResponse& response,
                                boost::system::error_code& ec)
{
  Int32 topics_size = ReadInt32(is);
  response.mutable_topics().resize(topics_size);
  for (Int32 i = 0; i < topics_size; ++i)
  {
    OffsetCommitResponse::Topic& topic = response.mutable_topics()[i];
    topic.topic_name = ReadString(is);

    Int32 partitions_size = ReadInt32(is);
    topic.partitions.resize(partitions_size);
    for (Int32 j = 0; j < partitions_size; ++j)
    {
      OffsetCommitResponse::TopicPartition& partition = topic.partitions[j];
      partition.partition = ReadInt32(is);
      partition.error_code = ReadInt16(is);
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_COMMIT_RESPONSE_READ_H_2BC25D29_FD96_4830_AD74_E2495AA55545
