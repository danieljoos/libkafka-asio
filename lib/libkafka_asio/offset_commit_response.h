//
// offset_commit_response.h
// ------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_COMMIT_RESPONSE_H_96791A91_0B01_4306_BB23_70B03D36B8F3
#define OFFSET_COMMIT_RESPONSE_H_96791A91_0B01_4306_BB23_70B03D36B8F3

#include <libkafka_asio/response.h>
#include <libkafka_asio/detail/topic_partition_block.h>

namespace libkafka_asio
{

class MutableOffsetCommitResponse;

// Kafka Offset Commit/Fetch API response implementation:
// Offset commit response
class OffsetCommitResponse :
  public Response<OffsetCommitResponse>
{
  friend class MutableOffsetCommitResponse;

  struct TopicPartitionProperties
  {
    Int16 error_code;
  };

public:
  typedef detail::TopicPartitionBlock<TopicPartitionProperties> Topic;
  typedef Topic::VectorType TopicVector;

  const TopicVector& topics() const;

private:
  TopicVector topics_;
};

class MutableOffsetCommitResponse :
  public MutableResponse<OffsetCommitResponse>
{
public:
  OffsetCommitResponse::TopicVector& mutable_topics();
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/offset_commit_response.h>

#endif  // OFFSET_COMMIT_RESPONSE_H_96791A91_0B01_4306_BB23_70B03D36B8F3
