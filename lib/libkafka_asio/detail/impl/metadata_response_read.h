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
  // Brokers
  response.mutable_brokers().resize(ReadInt32(is));
  BOOST_FOREACH(MetadataResponse::Broker& broker, response.mutable_brokers())
  {
    broker.node_id = ReadInt32(is);
    broker.host = ReadString(is);
    broker.port = ReadInt32(is);
  }

  // Topic Metadata
  response.mutable_topics().resize(ReadInt32(is));
  BOOST_FOREACH(MetadataResponse::Topic& topic, response.mutable_topics())
  {
    topic.error_code = ReadInt16(is);
    topic.topic_name = ReadString(is);
    topic.partitions.resize(ReadInt32(is));
    BOOST_FOREACH(MetadataResponse::Topic::Partition& partition,
                  topic.partitions)
    {
      partition.error_code = ReadInt16(is);
      partition.partition = ReadInt32(is);
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
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // METADATA_RESPONSE_READ_H_EBB58854_D8E4_40DE_A2E6_BADBFC9D8DB8
