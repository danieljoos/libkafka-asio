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

#include <vector>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/response.h>
#include <libkafka_asio/detail/fetch_response_iterator.h>
#include <libkafka_asio/detail/topic_partition_block.h>

namespace libkafka_asio
{

class MutableFetchResponse;

// Kafka Fetch API Response Implementation
class FetchResponse :
  public Response<FetchResponse>
{
  friend class MutableFetchResponse;

  struct TopicPartitionProperties
  {
    Int16 error_code;
    Int64 highwater_mark_offset;
    MessageSet messages;
  };

public:
  typedef detail::TopicPartitionBlock<TopicPartitionProperties> Topic;
  typedef Topic::VectorType TopicVector;
  typedef detail::FetchResponseIterator<TopicVector> const_iterator;

  const TopicVector& topics() const;

  // Start iterator, used for iterating over all received messages
  const_iterator begin() const;

  // End iterator
  const_iterator end() const;

private:
  TopicVector topics_;
};

class MutableFetchResponse :
  public MutableResponse<FetchResponse>
{
public:
  FetchResponse::TopicVector& mutable_topics();
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/fetch_response.h>

#endif  // FETCH_RESPONSE_H_AEAFD09F_2738_4548_B44B_0843DC1B9BB1
