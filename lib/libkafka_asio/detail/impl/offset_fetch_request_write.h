//
// detail/impl/offset_fetch_request_write.h
// ----------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_FETCH_REQUEST_WRITE_H_508FD276_A6DC_4FC5_B188_1C8ABB9BBDC6
#define OFFSET_FETCH_REQUEST_WRITE_H_508FD276_A6DC_4FC5_B188_1C8ABB9BBDC6

#include <boost/foreach.hpp>
#include <libkafka_asio/detail/request_write.h>

namespace libkafka_asio
{
namespace detail
{

inline Int32 RequestMessageWireSize(const OffsetFetchRequest& request)
{
  Int32 size = StringWireSize(request.consumer_group());
  // Topics Array
  size += sizeof(Int32);
  BOOST_FOREACH(const OffsetFetchRequest::Topic& topic, request.topics())
  {
    size += StringWireSize(topic.topic_name);
    // Partitions Array
    size += sizeof(Int32);
    size += static_cast<Int32>(topic.partitions.size()) * sizeof(Int32);
  }
  return size;
}

inline void WriteRequestMessage(const OffsetFetchRequest& request,
                                std::ostream& os)
{
  WriteString(request.consumer_group(), os);
  // Topics Array
  WriteInt32(static_cast<Int32>(request.topics().size()), os);
  BOOST_FOREACH(const OffsetFetchRequest::Topic& topic, request.topics())
  {
    WriteString(topic.topic_name, os);
    // Partitions Array
    WriteInt32(static_cast<Int32>(topic.partitions.size()), os);
    BOOST_FOREACH(const OffsetFetchRequest::Partition partition,
                  topic.partitions)
    {
      WriteInt32(partition.partition, os);
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_FETCH_REQUEST_WRITE_H_508FD276_A6DC_4FC5_B188_1C8ABB9BBDC6
