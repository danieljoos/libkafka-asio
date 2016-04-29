//
// impl/produce_request.h
// ----------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef PRODUCE_REQUEST_H_36D7E2BC_EF71_420F_B3D1_12614A1FD535
#define PRODUCE_REQUEST_H_36D7E2BC_EF71_420F_B3D1_12614A1FD535

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <libkafka_asio/constants.h>

namespace libkafka_asio
{

inline Int16 ProduceRequest::ApiKey()
{
  return constants::kApiKeyProduceRequest;
}

inline ProduceRequest::ProduceRequest() :
  required_acks_(constants::kDefaultProduceRequiredAcks),
  timeout_(constants::kDefaultProduceTimeout)
{
}

inline Int16 ProduceRequest::required_acks() const
{
  return required_acks_;
}

inline Int32 ProduceRequest::timeout() const
{
  return timeout_;
}

inline const ProduceRequest::Topics& ProduceRequest::topics() const
{
  return topics_;
}

inline void ProduceRequest::set_required_acks(Int16 required_acks)
{
  required_acks_ = required_acks;
}

inline void ProduceRequest::set_timeout(Int32 timeout)
{
  timeout_ = timeout;
}

inline void ProduceRequest::AddValue(const Bytes& value,
                                     const String& topic_name,
                                     Int32 partition)
{
  Message message;
  message.mutable_value() = value;
  AddMessage(message, topic_name, partition);
}

inline void ProduceRequest::AddValue(const String& value,
                                     const String& topic_name,
                                     Int32 partition)
{
  Message message;
  message.mutable_value().reset(
    new Bytes::element_type(value.begin(), value.end()));
  AddMessage(message, topic_name, partition);
}

inline void ProduceRequest::AddMessage(const Message& message,
                                       const String& topic_name,
                                       Int32 partition)
{
  Topics::iterator topic_iter = detail::FindTopicByName(topic_name, topics_);
  Partitions::iterator partition_iter =
    detail::FindTopicPartitionByNumber(partition, topic_iter->partitions);
  MessageAndOffset message_and_offset(message, 0);
  partition_iter->messages.push_back(message_and_offset);
}

inline void ProduceRequest::AddMessageSet(const MessageSet& message_set,
                                          const String& topic_name,
                                          Int32 partition)
{
  std::for_each(message_set.begin(), message_set.end(),
                boost::bind(&ProduceRequest::AddMessage,
                            this, ::_1, topic_name, partition));
}

inline void ProduceRequest::Clear()
{
  topics_.clear();
}

inline void ProduceRequest::ClearTopic(const String& topic_name)
{
  topics_.erase(std::remove_if(topics_.begin(),
                               topics_.end(),
                               IsTopicWithName(topic_name)),
                topics_.end());
}

inline void ProduceRequest::ClearTopicPartition(const String& topic_name,
                                                Int32 partition)
{
  Topics::iterator topic_iter = std::find_if(topics_.begin(),
                                             topics_.end(),
                                             IsTopicWithName(topic_name));
  if (topic_iter != topics_.end())
  {
    topic_iter->partitions.erase(std::remove_if(topic_iter->partitions.begin(),
                                                topic_iter->partitions.end(),
                                                IsTopicPartition(partition)),
                                 topic_iter->partitions.end());
  }
}

inline bool ProduceRequest::ResponseExpected() const
{
  return required_acks_ != 0;
}

} // namespace libkafka_asio

#include <libkafka_asio/detail/impl/produce_request_write.h>

#endif  // PRODUCE_REQUEST_H_36D7E2BC_EF71_420F_B3D1_12614A1FD535
