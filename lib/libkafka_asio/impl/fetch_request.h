//
// impl/fetch_request.h
// --------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef FETCH_REQUEST_H_31F2C3C7_9F99_4B7F_B824_210C071C6045
#define FETCH_REQUEST_H_31F2C3C7_9F99_4B7F_B824_210C071C6045

#include <libkafka_asio/constants.h>
#include <libkafka_asio/detail/functional.h>

namespace libkafka_asio
{

inline Int16 FetchRequest::ApiKey()
{
  return constants::kApiKeyFetchRequest;
}

inline FetchRequest::FetchRequest() :
  max_wait_time_(constants::kDefaultFetchMaxWaitTime),
  min_bytes_(constants::kDefaultFetchMinBytes)
{
}

inline Int32 FetchRequest::replica_id() const
{
  return -1;
}

inline Int32 FetchRequest::max_wait_time() const
{
  return max_wait_time_;
}

inline Int32 FetchRequest::min_bytes() const
{
  return min_bytes_;
}

inline const FetchRequest::Topics& FetchRequest::topics() const
{
  return topics_;
}

inline void FetchRequest::set_max_wait_time(Int32 max_wait_time)
{
  max_wait_time_ = max_wait_time;
}

inline void FetchRequest::set_min_bytes(Int32 min_bytes)
{
  min_bytes_ = min_bytes;
}

inline void FetchRequest::FetchTopic(const String& topic_name,
                                     Int32 partition,
                                     Int64 fetch_offset,
                                     Int32 max_bytes)
{
  Topics::iterator topic_iter = detail::FindTopicByName(topic_name, topics_);
  Partitions::iterator partition_iter =
    detail::FindTopicPartitionByNumber(partition, topic_iter->partitions);
  partition_iter->fetch_offset = fetch_offset;
  partition_iter->max_bytes = max_bytes;
}

inline void FetchRequest::Clear()
{
  topics_.clear();
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/fetch_request_write.h>

#endif  // FETCH_REQUEST_H_31F2C3C7_9F99_4B7F_B824_210C071C6045
