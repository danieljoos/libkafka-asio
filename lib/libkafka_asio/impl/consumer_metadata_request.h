//
// impl/consumer_metadata_request.h
// --------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CONSUMER_METADATA_REQUEST_H_9A89DB2D_7214_4D88_94EA_8A0E93822A06
#define CONSUMER_METADATA_REQUEST_H_9A89DB2D_7214_4D88_94EA_8A0E93822A06

#include <libkafka_asio/constants.h>

namespace libkafka_asio
{

inline Int16 ConsumerMetadataRequest::ApiKey()
{
  return constants::kApiKeyConsumerMetadataRequest;
}

inline const String& ConsumerMetadataRequest::consumer_group() const
{
  return consumer_group_;
}

inline void ConsumerMetadataRequest::set_consumer_group(
  const String& consumer_group)
{
  consumer_group_ = consumer_group;
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/consumer_metadata_request_write.h>

#endif  // CONSUMER_METADATA_REQUEST_H_9A89DB2D_7214_4D88_94EA_8A0E93822A06
