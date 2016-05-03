//
// detail/impl/offset_commit_request_write.h
// ---------------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_COMMIT_REQUEST_WRITE_H_8C84B333_19EF_4E81_8E1D_0236C0EA7061
#define OFFSET_COMMIT_REQUEST_WRITE_H_8C84B333_19EF_4E81_8E1D_0236C0EA7061

#include <boost/foreach.hpp>
#include <libkafka_asio/detail/request_write.h>

namespace libkafka_asio
{
namespace detail
{

inline Int32 RequestMessageWireSize(const OffsetCommitRequest& request)
{
  Int32 size = StringWireSize(request.consumer_group());

  // Topics Array
  size += sizeof(Int32);
  BOOST_FOREACH(const OffsetCommitRequest::Topic& topic, request.topics())
  {
    // Partitions Array
    size += sizeof(Int32);
    BOOST_FOREACH(const OffsetCommitRequest::Partition& partition,
                  topic.partitions)
    {
      size += 
        sizeof(Int32) +  // Partition
        sizeof(Int64) +  // Offset
        sizeof(Int64) +  // Timestamp
        StringWireSize(partition.metadata);  // Metadata
    }
  }
  return size;
}

inline void WriteRequestMessage(const OffsetCommitRequest& request,
                                std::ostream& os)
{
  WriteString(request.consumer_group(), os);
  
  // Topics Array
  WriteInt32(static_cast<Int32>(request.topics().size()), os);
  BOOST_FOREACH(const OffsetCommitRequest::Topic& topic, request.topics())
  {
    WriteString(topic.topic_name, os);

    // Partitions Array
    WriteInt32(static_cast<Int32>(topic.partitions.size()), os);
    BOOST_FOREACH(const OffsetCommitRequest::Partition& partition,
                  topic.partitions)
    {
      WriteInt32(partition.partition, os);
      WriteInt64(partition.offset, os);
      WriteInt64(partition.timestamp, os);
      WriteString(partition.metadata, os);
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_COMMIT_REQUEST_WRITE_H_8C84B333_19EF_4E81_8E1D_0236C0EA7061
