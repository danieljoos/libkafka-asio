//
// offset_response.h
// -----------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_RESPONSE_H_23EABE67_36C3_4D9E_8C37_4C8E916DE537
#define OFFSET_RESPONSE_H_23EABE67_36C3_4D9E_8C37_4C8E916DE537

#include <vector>
#include <boost/optional.hpp>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/response.h>
#include <libkafka_asio/detail/topics_partitions.h>

namespace libkafka_asio
{

class MutableOffsetResponse;

// Kafka Offset API response
class OffsetResponse :
  public Response<OffsetResponse>
{
  friend class MutableOffsetResponse;

  struct PartitionProperties
  {
    typedef std::vector<Int64> OffsetVector;
    Int16 error_code;
    OffsetVector offsets;
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

  const Topics& topics() const;

  // Search for offset data inside this response object for the given topic
  // and partition. If no such data can be found, the return value is empty.
  Partition::OptionalType TopicPartitionOffset(const String& topic_name,
                                               Int32 partition) const;

private:
  Topics topics_;
};

class MutableOffsetResponse :
  public MutableResponse<OffsetResponse>
{
public:
  OffsetResponse::Topics& mutable_topics();
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/offset_response.h>

#endif  // OFFSET_RESPONSE_H_23EABE67_36C3_4D9E_8C37_4C8E916DE537
