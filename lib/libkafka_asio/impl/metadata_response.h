//
// impl/metadata_response.h
// ------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef METADATA_RESPONSE_H_E93D2A75_E1ED_4CEA_97A4_48983B568306
#define METADATA_RESPONSE_H_E93D2A75_E1ED_4CEA_97A4_48983B568306

#include <libkafka_asio/detail/functional.h>

namespace libkafka_asio
{

inline const MetadataResponse::BrokerVector& MetadataResponse::brokers() const
{
  return brokers_;
}

inline const MetadataResponse::TopicVector&
MetadataResponse::topics() const
{
  return topics_;
}

inline MetadataResponse::BrokerVector&
MutableMetadataResponse::mutable_brokers()
{
  return response_.brokers_;
}

inline MetadataResponse::TopicVector& MutableMetadataResponse::mutable_topics()
{
  return response_.topics_;
}

inline MetadataResponse::Broker::OptionalType
MetadataResponse::PartitionLeader(const String& topic, Int32 partition) const
{
  TopicVector::const_iterator topic_iter =
    detail::FindTopicByName(topic, topics_);
  if (topic_iter == topics_.end())
  {
    return Broker::OptionalType();
  }
  Topic::PartitionVector::const_iterator partition_iter =
    detail::FindTopicPartitionByNumber(partition, topic_iter->partitions);
  if (partition_iter == topic_iter->partitions.end() ||
      partition_iter->leader == constants::kMetadataLeaderUndecided)
  {
    return Broker::OptionalType();
  }
  BrokerVector::const_iterator broker_iter =
    detail::FindBrokerById(partition_iter->leader, brokers_);
  if (broker_iter != brokers_.end())
  {
    return *broker_iter;
  }
  return Broker::OptionalType();
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/metadata_response_read.h>

#endif  // METADATA_RESPONSE_H_E93D2A75_E1ED_4CEA_97A4_48983B568306
