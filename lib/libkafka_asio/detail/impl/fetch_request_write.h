//
// detail/impl/fetch_request_write.h
// ---------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef FETCH_REQUEST_WRITE_H_B441DB98_D6A0_42AF_B49E_B7FEA169DCC6
#define FETCH_REQUEST_WRITE_H_B441DB98_D6A0_42AF_B49E_B7FEA169DCC6

#include <boost/foreach.hpp>
#include <libkafka_asio/detail/request_write.h>

namespace libkafka_asio
{
namespace detail
{

inline Int32 RequestMessageWireSize(const FetchRequest& request)
{
  Int32 size =
    sizeof(Int32) +  // ReplicaId
    sizeof(Int32) +  // MaxWaitTime
    sizeof(Int32);   // MinByte

  // Topics Array
  size += sizeof(Int32);
  BOOST_FOREACH(const FetchRequest::Topic& topic, request.topics())
  {
    size += StringWireSize(topic.topic_name);

    // Partitions Array
    size += static_cast<Int32>(
      sizeof(Int32) +    // Array Length
      topic.partitions.size() * (
        sizeof(Int32) +  // Partition
        sizeof(Int64) +  // FetchOffset
        sizeof(Int32))); // MaxBytes
  }
  return size;
}

inline void WriteRequestMessage(const FetchRequest& request, std::ostream& os)
{
  WriteInt32(request.replica_id(), os);
  WriteInt32(request.max_wait_time(), os);
  WriteInt32(request.min_bytes(), os);

  // Topics Array
  WriteInt32(static_cast<Int32>(request.topics().size()), os);
  BOOST_FOREACH(const FetchRequest::Topic& topic, request.topics())
  {
    WriteString(topic.topic_name, os);

    // Partitions Array
    WriteInt32(static_cast<Int32>(topic.partitions.size()), os);
    BOOST_FOREACH(const FetchRequest::Partition& partition, topic.partitions)
    {
      WriteInt32(partition.partition, os);
      WriteInt64(partition.fetch_offset, os);
      WriteInt32(partition.max_bytes, os);
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // FETCH_REQUEST_WRITE_H_B441DB98_D6A0_42AF_B49E_B7FEA169DCC6
