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

inline const OffsetResponse::TopicVector& OffsetResponse::topics() const
{
  return topics_;
}

inline OffsetResponse::Topic::Partition::OptionalType
OffsetResponse::TopicPartitionOffset(const String& topic_name,
                                     Int32 partition) const
{
  TopicVector::const_iterator topic_iter =
    detail::FindTopicByName(topic_name, topics_);
  if (topic_iter == topics_.end())
  {
    return Topic::Partition::OptionalType();
  }
  Topic::PartitionVector::const_iterator partition_iter =
    detail::FindTopicPartitionByNumber(partition, topic_iter->partitions);
  if (partition_iter == topic_iter->partitions.end())
  {
    return Topic::Partition::OptionalType();
  }
  return *partition_iter;
}

inline OffsetResponse::TopicVector& MutableOffsetResponse::mutable_topics()
{
  return response_.topics_;
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/offset_response_read.h>

#endif  // OFFSET_RESPONSE_H_59E9CB4F_A0BD_46FE_8650_3F0890A7C0D2
