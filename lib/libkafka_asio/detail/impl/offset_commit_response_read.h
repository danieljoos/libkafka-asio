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
  response.mutable_topics().resize(ReadInt32(is));
  BOOST_FOREACH(OffsetCommitResponse::Topic& topic, response.mutable_topics())
  {
    topic.topic_name = ReadString(is);
    topic.partitions.resize(ReadInt32(is));
    BOOST_FOREACH(OffsetCommitResponse::Topic::Partition& partition,
                  topic.partitions)
    {
      partition.partition = ReadInt32(is);
      partition.error_code = ReadInt16(is);
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_COMMIT_RESPONSE_READ_H_2BC25D29_FD96_4830_AD74_E2495AA55545
