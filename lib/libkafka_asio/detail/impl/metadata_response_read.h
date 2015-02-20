//
// detail/impl/metadata_response_read.h
// ------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef METADATA_RESPONSE_READ_EBB58854_D8E4_40DE_A2E6_BADBFC9D8DB8
#define METADATA_RESPONSE_READ_EBB58854_D8E4_40DE_A2E6_BADBFC9D8DB8

#include <libkafka_asio/primitives.h>
#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream& is,
                                MutableMetadataResponse& response,
                                boost::system::error_code& ec)
{
  // Brokers
  Int32 broker_size = ReadInt32(is);
  for (Int32 i = 0; i < broker_size; ++i)
  {
    MetadataResponse::Broker broker;
    broker.node_id = ReadInt32(is);
    broker.host = ReadString(is);
    broker.port = ReadInt32(is);
    response.mutable_broker().push_back(broker);
  }

  // Topic Metadata
  Int32 topic_metadata_size = ReadInt32(is);
  for (Int32 i = 0; i < topic_metadata_size; ++i)
  {
    MetadataResponse::TopicMetadata topic_metadata;
    topic_metadata.topic_error_code = ReadInt16(is);
    topic_metadata.topic_name = ReadString(is);

    Int32 partition_metadata_size = ReadInt32(is);
    for (Int32 j = 0; j < partition_metadata_size; ++j)
    {
      MetadataResponse::PartitionMetaData partition_meta_data;
      partition_meta_data.partition_error_code = ReadInt16(is);
      partition_meta_data.partition = ReadInt32(is);
      partition_meta_data.leader = ReadInt32(is);

      Int32 replicas_size = ReadInt32(is);
      for (Int32 k = 0; k < replicas_size; ++k)
      {
        partition_meta_data.replicas.push_back(ReadInt32(is));
      }

      Int32 isr_size = ReadInt32(is);
      for (Int32 k = 0; k < isr_size; ++k)
      {
        partition_meta_data.isr.push_back(ReadInt32(is));
      }

      topic_metadata.partition_metadata.push_back(partition_meta_data);
    }

    response.mutable_topic_metadata().push_back(topic_metadata);
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // METADATA_RESPONSE_READ_H_EBB58854_D8E4_40DE_A2E6_BADBFC9D8DB8
