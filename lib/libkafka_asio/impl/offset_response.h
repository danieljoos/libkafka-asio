//
// impl/offset_response.h
// ----------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_RESPONSE_H_59E9CB4F_A0BD_46FE_8650_3F0890A7C0D2
#define OFFSET_RESPONSE_H_59E9CB4F_A0BD_46FE_8650_3F0890A7C0D2

#include <libkafka_asio/detail/functional.h>

namespace libkafka_asio
{

inline const OffsetResponse::Topics& OffsetResponse::topics() const
{
  return topics_;
}

inline OffsetResponse::Partition::OptionalType
OffsetResponse::TopicPartitionOffset(const String& topic_name,
                                     Int32 partition) const
{
  Topics::const_iterator topic_iter = topics_.find(topic_name);
  if (topic_iter == topics_.end())
  {
    return Partition::OptionalType();
  }
  Partitions::const_iterator partition_iter =
    topic_iter->second.partitions.find(partition);
  if (partition_iter == topic_iter->second.partitions.end())
  {
    return Partition::OptionalType();
  }
  return partition_iter->second;
}

inline OffsetResponse::Topics& MutableOffsetResponse::mutable_topics()
{
  return response_.topics_;
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/offset_response_read.h>

#endif  // OFFSET_RESPONSE_H_59E9CB4F_A0BD_46FE_8650_3F0890A7C0D2
