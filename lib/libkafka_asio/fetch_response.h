//
// fetch_response.h
// ----------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef FETCH_RESPONSE_H_AEAFD09F_2738_4548_B44B_0843DC1B9BB1
#define FETCH_RESPONSE_H_AEAFD09F_2738_4548_B44B_0843DC1B9BB1

#include <libkafka_asio/primitives.h>
#include <libkafka_asio/response.h>
#include <libkafka_asio/detail/fetch_response_iterator.h>
#include <libkafka_asio/detail/topics_partitions.h>

namespace libkafka_asio
{

class MutableFetchResponse;

// Kafka Fetch API Response Implementation
class FetchResponse :
  public Response<FetchResponse>
{
  friend class MutableFetchResponse;

  struct PartitionProperties
  {
    Int16 error_code;
    Int64 highwater_mark_offset;
    MessageSet messages;
  };

  typedef detail::TopicsPartitionsMap<
    detail::EmptyProperties,
    PartitionProperties
  > TopicsPartitions;

public:
  typedef TopicsPartitions::TopicType Topic;
  typedef TopicsPartitions::PartitionType Partition;
  typedef TopicsPartitions::TopicsType Topics;
  typedef TopicsPartitions::PartitionsType Partitions;

  typedef detail::FetchResponseIterator<TopicsPartitions> const_iterator;

  const Topics& topics() const;

  // Start iterator, used for iterating over all received messages
  const_iterator begin() const;

  // End iterator
  const_iterator end() const;

private:
  Topics topics_;
};

class MutableFetchResponse :
  public MutableResponse<FetchResponse>
{
public:
  FetchResponse::Topics& mutable_topics();
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/fetch_response.h>

#endif  // FETCH_RESPONSE_H_AEAFD09F_2738_4548_B44B_0843DC1B9BB1
