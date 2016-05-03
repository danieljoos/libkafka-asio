//
// metadata_response.h
// -------------------
//
// Copyright (c) 2015-2016 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef METADATA_RESPONSE_H_3EEE6475_7990_4611_B8E6_5CA255FB9791
#define METADATA_RESPONSE_H_3EEE6475_7990_4611_B8E6_5CA255FB9791

#include <vector>
#include <boost/optional.hpp>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/response.h>
#include <libkafka_asio/detail/topics_partitions.h>

namespace libkafka_asio
{

class MutableMetadataResponse;

// Kafka Metadata API response
class MetadataResponse :
  public Response<MetadataResponse>
{
  friend class MutableMetadataResponse;

  struct PartitionProperties
  {
    typedef std::vector<Int32> ReplicasVector;
    typedef std::vector<Int32> IsrVector;
    Int16 error_code;
    Int32 leader;
    ReplicasVector replicas;
    IsrVector isr;
  };

  struct TopicProperties
  {
    Int16 error_code;
  };

  typedef detail::TopicsPartitionsMap<
    TopicProperties,
    PartitionProperties
  > TopicsPartitions;

public:
  struct Broker
  {
    typedef boost::optional<Broker> OptionalType;
    Int32 node_id;
    String host;
    Int32 port;
  };
  typedef std::vector<Broker> BrokerVector;
  typedef TopicsPartitions::TopicType Topic;
  typedef TopicsPartitions::PartitionType Partition;
  typedef TopicsPartitions::TopicsType Topics;
  typedef TopicsPartitions::PartitionsType Partitions;

  const BrokerVector& brokers() const;

  const Topics& topics() const;

  Broker::OptionalType PartitionLeader(const String& topic,
                                       Int32 partition) const;

private:
  BrokerVector brokers_;
  Topics topics_;
};

class MutableMetadataResponse :
  public MutableResponse<MetadataResponse>
{
public:
  MetadataResponse::BrokerVector& mutable_brokers();

  MetadataResponse::Topics& mutable_topics();
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/metadata_response.h>

#endif  // METADATA_RESPONSE_H_3EEE6475_7990_4611_B8E6_5CA255FB9791
