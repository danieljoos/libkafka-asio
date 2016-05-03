//
// impl/offset_request.h
// ---------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_REQUEST_H_AD3922D4_7FA5_4EF7_A4A9_984D5FF43311
#define OFFSET_REQUEST_H_AD3922D4_7FA5_4EF7_A4A9_984D5FF43311

#include <libkafka_asio/constants.h>
#include <libkafka_asio/detail/functional.h>

namespace libkafka_asio
{

inline Int16 OffsetRequest::ApiKey()
{
  return constants::kApiKeyOffsetRequest;
}

inline Int32 OffsetRequest::replica_id() const
{
  return -1;
}

inline const OffsetRequest::Topics& OffsetRequest::topics() const
{
  return topics_;
}

inline void OffsetRequest::FetchTopicOffset(const String& topic_name,
                                            Int32 partition,
                                            Int64 time,
                                            Int32 max_number_of_offsets)
{
  Topics::iterator topic_iter = detail::FindTopicByName(topic_name, topics_);
  Partitions::iterator partition_iter =
    detail::FindTopicPartitionByNumber(partition, topic_iter->partitions);
  partition_iter->time = time;
  partition_iter->max_number_of_offsets = max_number_of_offsets;
}

inline void OffsetRequest::Clear()
{
  topics_.clear();
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/offset_request_write.h>

#endif  // OFFSET_REQUEST_H_AD3922D4_7FA5_4EF7_A4A9_984D5FF43311
