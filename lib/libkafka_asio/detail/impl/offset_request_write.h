//
// detail/impl/offset_request_write.h
// ----------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_REQUEST_WRITE_H_B5A2F2C0_18BC_4AD5_B892_27834F8D87DE
#define OFFSET_REQUEST_WRITE_H_B5A2F2C0_18BC_4AD5_B892_27834F8D87DE

#include <boost/foreach.hpp>
#include <libkafka_asio/detail/request_write.h>

namespace libkafka_asio
{
namespace detail
{

inline Int32 RequestMessageWireSize(const OffsetRequest& request)
{
  Int32 size = sizeof(Int32);  // ReplicaId

  // Topics array
  size += sizeof(Int32);
  BOOST_FOREACH(const OffsetRequest::Topic& topic, request.topics())
  {
    size += StringWireSize(topic.topic_name);

    // Partitions array
    size += sizeof(Int32);
    size += static_cast<Int32>(topic.partitions.size() * (
      sizeof(Int32) +  // Partition
      sizeof(Int64) +  // Time
      sizeof(Int32))); // MaxNumberOfOffsets
  }
  return size;
}

inline void WriteRequestMessage(const OffsetRequest& request, std::ostream& os)
{
  WriteInt32(request.replica_id(), os);

  // Topics array
  WriteInt32(static_cast<Int32>(request.topics().size()), os);
  BOOST_FOREACH(const OffsetRequest::Topic& topic, request.topics())
  {
    WriteString(topic.topic_name, os);

    // Partitions array
    WriteInt32(static_cast<Int32>(topic.partitions.size()), os);
    BOOST_FOREACH(const OffsetRequest::Partition& partition, topic.partitions)
    {
      WriteInt32(partition.partition, os);
      WriteInt64(partition.time, os);
      WriteInt32(partition.max_number_of_offsets, os);
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // OFFSET_REQUEST_WRITE_H_B5A2F2C0_18BC_4AD5_B892_27834F8D87DE
