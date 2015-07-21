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
  response.mutable_topics().resize(ReadInt32(is));
  BOOST_FOREACH(FetchResponse::Topic& topic, response.mutable_topics())
  {
    topic.topic_name = ReadString(is);

    // Partitions array
    topic.partitions.resize(ReadInt32(is));
    BOOST_FOREACH(FetchResponse::Topic::Partition& partition, topic.partitions)
    {
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
      ReadMessageSet(is, partition.messages, message_set_size, ec);
      if (ec != kErrorSuccess)
      {
        return;
      }
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // FETCH_RESPONSE_READ_H_03C2A581_53D0_498E_AF6A_2082227485A3
