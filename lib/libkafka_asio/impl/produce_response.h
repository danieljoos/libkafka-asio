//
// impl/produce_response.h
// -----------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef PRODUCE_RESPONSE_H_C0AC4959_A1B0_4497_9B4C_ED995AA5E3CC
#define PRODUCE_RESPONSE_H_C0AC4959_A1B0_4497_9B4C_ED995AA5E3CC

#include <libkafka_asio/detail/functional.h>

namespace libkafka_asio
{

inline ProduceResponse::Topic::OptionalType ProduceResponse::FindTopic(
  const String& topic_name) const
{
  Topics::const_iterator iter = topics_.find(topic_name);
  if (iter != topics_.end())
  {
    return iter->second;
  }
  return Topic::OptionalType();
}

inline ProduceResponse::Partition::OptionalType
ProduceResponse::FindTopicPartition(const String& topic_name,
                                    Int32 partition) const
{
  Topic::OptionalType topic = FindTopic(topic_name);
  if (topic)
  {
    Partitions::const_iterator iter = topic->partitions.find(partition);
    if (iter != topic->partitions.end())
    {
      return iter->second;
    }
  }
  return Partition::OptionalType();
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/produce_response_read.h>

#endif  // PRODUCE_RESPONSE_H_C0AC4959_A1B0_4497_9B4C_ED995AA5E3CC
