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

inline const MetadataResponse::BrokerVector& MetadataResponse::broker() const
{
  return broker_;
}

inline const MetadataResponse::TopicMetadataVector&
MetadataResponse::topic_metadata() const
{
  return topic_metadata_;
}

inline MetadataResponse::BrokerVector& MutableMetadataResponse::mutable_broker()
{
  return response_.broker_;
}

inline MetadataResponse::TopicMetadataVector&
MutableMetadataResponse::mutable_topic_metadata()
{
  return response_.topic_metadata_;
}

inline MetadataResponse::OptionalBrokerType
MetadataResponse::PartitionLeader(const String& topic, Int32 partition) const
{
  TopicMetadataVector::const_iterator topic_iter =
    detail::FindTopicByName(topic, topic_metadata_);
  if (topic_iter == topic_metadata_.end())
  {
    return OptionalBrokerType();
  }
  TopicMetadata::PartitionMetadataVector::const_iterator partition_iter =
    detail::FindTopicPartitionByNumber(partition,
                                       topic_iter->partition_metadata);
  if (partition_iter == topic_iter->partition_metadata.end() ||
    partition_iter->leader == constants::kMetadataLeaderUndecided)
  {
    return OptionalBrokerType();
  }
  BrokerVector::const_iterator broker_iter =
    detail::FindBrokerById(partition_iter->leader, broker_);
  if (broker_iter != broker_.end())
  {
    return *broker_iter;
  }
  return OptionalBrokerType();
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/metadata_response_read.h>

#endif  // METADATA_RESPONSE_H_E93D2A75_E1ED_4CEA_97A4_48983B568306
