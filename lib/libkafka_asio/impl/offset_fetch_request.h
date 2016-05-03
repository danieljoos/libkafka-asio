//
// impl/offset_fetch_request.h
// ---------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_FETCH_REQUEST_H_FCE526A2_0BA6_48F4_9717_4729A46D6D5D
#define OFFSET_FETCH_REQUEST_H_FCE526A2_0BA6_48F4_9717_4729A46D6D5D

#include <libkafka_asio/constants.h>
#include <libkafka_asio/detail/functional.h>

namespace libkafka_asio
{

inline Int16 OffsetFetchRequest::ApiKey()
{
  return constants::kApiKeyOffsetFetchRequest;
}

inline const String& OffsetFetchRequest::consumer_group() const
{
  return consumer_group_;
}

inline const OffsetFetchRequest::Topics& OffsetFetchRequest::topics() const
{
  return topics_;
}

inline void OffsetFetchRequest::set_consumer_group(const String& consumer_group)
{
  consumer_group_ = consumer_group;
}

inline void OffsetFetchRequest::FetchOffset(const String& topic_name,
                                            Int32 partition)
{
  Topics::iterator topic_iter = detail::FindTopicByName(topic_name, topics_);
  Partitions::iterator partition_iter =
    detail::FindTopicPartitionByNumber(partition, topic_iter->partitions);
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/offset_fetch_request_write.h>

#endif  // OFFSET_FETCH_REQUEST_H_FCE526A2_0BA6_48F4_9717_4729A46D6D5D
