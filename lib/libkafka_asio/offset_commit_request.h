//
// offset_commit_request.h
// -----------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_COMMIT_REQUEST_H_CC036F24_24FE_4B96_84F6_2164413E458A
#define OFFSET_COMMIT_REQUEST_H_CC036F24_24FE_4B96_84F6_2164413E458A

#include <vector>
#include <libkafka_asio/constants.h>
#include <libkafka_asio/request.h>
#include <libkafka_asio/offset_commit_response.h>

namespace libkafka_asio
{

class OffsetCommitRequest :
  public Request<OffsetCommitRequest>
{
  friend class Request<OffsetCommitRequest>;

  static Int16 ApiKey();

public:
  typedef OffsetCommitResponse ResponseType;
  typedef MutableOffsetCommitResponse MutableResponseType;

  struct TopicPartition
  {
    Int32 partition;
    Int64 offset;
    Int64 timestamp;
    String metadata;
  };

  struct Topic
  {
    typedef std::vector<TopicPartition> TopicPartitionVector;
    String topic_name;
    TopicPartitionVector partitions;
  };

  typedef std::vector<Topic> TopicVector;

  const String& consumer_group() const;

  const TopicVector& topics() const;

  void set_consumer_group(const String& consumer_group);

  void CommitOffset(
    const String& topic_name,
    Int32 partition,
    Int64 offset,
    Int64 timestamp = constants::kDefaultOffsetCommitTimestampNow,
    const String& metadata = "");

  void Clear();

private:
  String consumer_group_;
  TopicVector topics_;
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/offset_commit_request.h>

#endif  // OFFSET_COMMIT_REQUEST_H_CC036F24_24FE_4B96_84F6_2164413E458A
