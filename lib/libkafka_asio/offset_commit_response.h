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

#include <vector>
#include <libkafka_asio/response.h>

namespace libkafka_asio
{

class MutableOffsetCommitResponse;

class OffsetCommitResponse :
  public Response<OffsetCommitResponse>
{
  friend class MutableOffsetCommitResponse;

public:

  struct TopicPartition
  {
    Int32 partition;
    Int16 error_code;
  };

  struct Topic
  {
    typedef std::vector<TopicPartition> TopicPartitionVector;
    String topic_name;
    TopicPartitionVector partitions;
  };

  typedef std::vector<Topic> TopicVector;

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
