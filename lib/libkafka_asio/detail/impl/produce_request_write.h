//
// detail/impl/produce_request_write.h
// -----------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef PRODUCE_REQUEST_WRITE_H_E92E8BE2_2AB8_4383_A2DD_698DD8482789
#define PRODUCE_REQUEST_WRITE_H_E92E8BE2_2AB8_4383_A2DD_698DD8482789

#include <boost/foreach.hpp>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/detail/request_write.h>

namespace libkafka_asio
{
namespace detail
{

inline Int32 RequestMessageWireSize(const ProduceRequest& request)
{
  Int32 size =
    sizeof(Int16) +  // RequiredAcks
    sizeof(Int32);   // Timeout

  // Topic Array
  size += sizeof(Int32);
  BOOST_FOREACH(const ProduceRequest::Topic& topic, request.topics())
  {
    size += StringWireSize(topic.topic_name);

    // Partition array
    size += sizeof(Int32);
    BOOST_FOREACH(const ProduceRequest::Partition& partition,
                  topic.partitions)
    {
      size +=
        sizeof(Int32) +  // Partition
        sizeof(Int32) +  // MessageSetSize
        MessageSetWireSize(partition.messages);  // MessageSet
    }
  }
  return size;
}

inline void WriteRequestMessage(const ProduceRequest& request, std::ostream& os)
{
  WriteInt16(request.required_acks(), os);
  WriteInt32(request.timeout(), os);

  // Topic Array
  WriteInt32(static_cast<Int32>(request.topics().size()), os);
  BOOST_FOREACH(const ProduceRequest::Topic& topic, request.topics())
  {
    WriteString(topic.topic_name, os);

    // Partition Array
    WriteInt32(static_cast<Int32>(topic.partitions.size()), os);
    BOOST_FOREACH(const ProduceRequest::Partition& partition,
                  topic.partitions)
    {
      WriteInt32(partition.partition, os);
      WriteInt32(MessageSetWireSize(partition.messages), os);
      WriteMessageSet(partition.messages, os);
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // PRODUCE_REQUEST_WRITE_H_E92E8BE2_2AB8_4383_A2DD_698DD8482789
