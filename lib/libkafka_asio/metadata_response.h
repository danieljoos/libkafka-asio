//
// metadata_response.h
// -------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef METADATA_RESPONSE_H_3EEE6475_7990_4611_B8E6_5CA255FB9791
#define METADATA_RESPONSE_H_3EEE6475_7990_4611_B8E6_5CA255FB9791

#include <vector>
#include <boost/optional.hpp>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/response.h>

namespace libkafka_asio
{

class MutableMetadataResponse;

// Kafka Metadata API response
class MetadataResponse :
  public Response<MetadataResponse>
{
  friend class MutableMetadataResponse;

public:

  struct Broker
  {
    Int32 node_id;
    String host;
    Int32 port;
  };

  struct PartitionMetaData
  {
    typedef std::vector<Int32> ReplicasVector;
    typedef std::vector<Int32> IsrVector;

    Int16 partition_error_code;
    Int32 partition;
    Int32 leader;
    ReplicasVector replicas;
    IsrVector isr;
  };

  struct TopicMetadata
  {
    typedef std::vector<PartitionMetaData> PartitionMetadataVector;

    Int16 topic_error_code;
    String topic_name;
    PartitionMetadataVector partition_metadata;
  };

  typedef std::vector<Broker> BrokerVector;
  typedef std::vector<TopicMetadata> TopicMetadataVector;
  typedef boost::optional<Broker> OptionalBrokerType;

  const BrokerVector& broker() const;

  const TopicMetadataVector& topic_metadata() const;

  OptionalBrokerType PartitionLeader(const String& topic,
                                     Int32 partition) const;

private:
  BrokerVector broker_;
  TopicMetadataVector topic_metadata_;
};

class MutableMetadataResponse :
  public MutableResponse<MetadataResponse>
{
public:
  MetadataResponse::BrokerVector& mutable_broker();

  MetadataResponse::TopicMetadataVector& mutable_topic_metadata();
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/metadata_response.h>

#endif  // METADATA_RESPONSE_H_3EEE6475_7990_4611_B8E6_5CA255FB9791
