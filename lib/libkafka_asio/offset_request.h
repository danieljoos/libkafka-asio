//
// offset_request.h
// ----------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_REQUEST_H_1AF16555_D1BD_466F_8853_BDA1F88D978A
#define OFFSET_REQUEST_H_1AF16555_D1BD_466F_8853_BDA1F88D978A

#include <vector>
#include <libkafka_asio/constants.h>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/request.h>
#include <libkafka_asio/offset_response.h>
#include <libkafka_asio/detail/topics_partitions.h>

namespace libkafka_asio
{

// Kafka Offset API request implementation
class OffsetRequest :
  public Request<OffsetRequest>
{
  friend class Request<OffsetRequest>;

  static Int16 ApiKey();

  struct PartitionProperties
  {
    Int64 time;
    Int32 max_number_of_offsets;
  };

  typedef detail::TopicsPartitionsVector<
    detail::EmptyProperties,
    PartitionProperties
  > TopicsPartitions;

public:
  typedef OffsetResponse ResponseType;
  typedef MutableOffsetResponse MutableResponseType;
  typedef TopicsPartitions::TopicType Topic;
  typedef TopicsPartitions::PartitionType Partition;
  typedef TopicsPartitions::TopicsType Topics;
  typedef TopicsPartitions::PartitionsType Partitions;

  Int32 replica_id() const;

  const Topics& topics() const;

  // Fetch offset information for the given topic-partition.
  // The (optional) `time` parameter can be used to ask for messages before
  // a certain time in the past (in milliseconds). Two special values exist for
  // this parameter:
  //   `libkafka_asio::constants::kOffsetTimeLatest`    (-1) (Default)
  //   `libkafka_asio::constants::kOffsetTimeEarliest`  (-2)
  //
  void FetchTopicOffset(const String& topic_name,
                        Int32 partition,
                        Int64 time = constants::kOffsetTimeLatest,
                        Int32 max_number_of_offsets
                        = constants::kDefaultOffsetMaxNumberOfOffsets);

  // Clears all entries for fetching topic-partitions
  void Clear();

private:
  Topics topics_;
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/offset_request.h>

#endif  // OFFSET_REQUEST_H_1AF16555_D1BD_466F_8853_BDA1F88D978A
