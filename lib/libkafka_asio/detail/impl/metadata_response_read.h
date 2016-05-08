//
// detail/impl/metadata_response_read.h
// ------------------------------------
//
// Copyright (c) 2015-2016 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef METADATA_RESPONSE_READ_EBB58854_D8E4_40DE_A2E6_BADBFC9D8DB8
#define METADATA_RESPONSE_READ_EBB58854_D8E4_40DE_A2E6_BADBFC9D8DB8

#include <boost/foreach.hpp>
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
  // error code not used
  (void)ec;
  // Brokers
  response.mutable_brokers().resize(ReadInt32(is));
  BOOST_FOREACH(MetadataResponse::Broker& broker, response.mutable_brokers())
  {
    broker.node_id = ReadInt32(is);
    broker.host = ReadString(is);
    broker.port = ReadInt32(is);
  }

  // Topic Metadata
  int topic_count = ReadInt32(is);
  for (int t = 0; t < topic_count; ++t)
  {
    MetadataResponse::Topics::key_type key;
    MetadataResponse::Topics::mapped_type topic;
    topic.error_code = ReadInt16(is);
    key = ReadString(is);

    int partition_count = ReadInt32(is);
    for (int p = 0; p < partition_count; ++p)
    {
      MetadataResponse::Partitions::key_type key;
      MetadataResponse::Partitions::mapped_type partition;
      partition.error_code = ReadInt16(is);
      key = ReadInt32(is);
      partition.leader = ReadInt32(is);

      Int32 replicas_size = ReadInt32(is);
      for (Int32 k = 0; k < replicas_size; ++k)
      {
        partition.replicas.push_back(ReadInt32(is));
      }

      Int32 isr_size = ReadInt32(is);
      for (Int32 k = 0; k < isr_size; ++k)
      {
        partition.isr.push_back(ReadInt32(is));
      }
      topic.partitions.insert(std::make_pair(key, partition));
    }
    response.mutable_topics().insert(std::make_pair(key, topic));
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // METADATA_RESPONSE_READ_H_EBB58854_D8E4_40DE_A2E6_BADBFC9D8DB8
