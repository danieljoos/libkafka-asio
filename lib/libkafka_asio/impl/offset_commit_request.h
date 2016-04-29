//
// impl/offset_commit_request.h
// ----------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_COMMIT_REQUEST_H_391FB5CD_2126_42DC_8CC4_16F5F38FFB82
#define OFFSET_COMMIT_REQUEST_H_391FB5CD_2126_42DC_8CC4_16F5F38FFB82

#include <libkafka_asio/constants.h>
#include <libkafka_asio/detail/functional.h>

namespace libkafka_asio
{

inline Int16 OffsetCommitRequest::ApiKey()
{
  return constants::kApiKeyOffsetCommitRequest;
}

inline const String& OffsetCommitRequest::consumer_group() const
{
  return consumer_group_;
}

inline const OffsetCommitRequest::Topics& OffsetCommitRequest::topics() const
{
  return topics_;
}

inline void OffsetCommitRequest::set_consumer_group(
  const String& consumer_group)
{
  consumer_group_ = consumer_group;
}

inline void OffsetCommitRequest::CommitOffset(const String& topic_name,
                                              Int32 partition,
                                              Int64 offset,
                                              Int64 timestamp,
                                              const String& metadata)
{
  Topics::iterator topic_iter = detail::FindTopicByName(topic_name, topics_);
  Partitions::iterator partition_iter =
    detail::FindTopicPartitionByNumber(partition, topic_iter->partitions);
  partition_iter->offset = offset;
  partition_iter->timestamp = timestamp;
  partition_iter->metadata = metadata;
}

inline void OffsetCommitRequest::Clear()
{
  topics_.clear();
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/offset_commit_request_write.h>

#endif  // OFFSET_COMMIT_REQUEST_H_391FB5CD_2126_42DC_8CC4_16F5F38FFB82
