//
// impl/consumer_metadata_response.h
// ---------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CONSUMER_METADATA_RESPONSE_H_9DC3DEAB_1C5B_4B67_A8F5_FBB39CB8F065
#define CONSUMER_METADATA_RESPONSE_H_9DC3DEAB_1C5B_4B67_A8F5_FBB39CB8F065

namespace libkafka_asio
{

inline Int16 ConsumerMetadataResponse::error_code() const
{
  return error_code_;
}

inline Int32 ConsumerMetadataResponse::coordinator_id() const
{
  return coordinator_id_;
}

inline const String& ConsumerMetadataResponse::coordinator_host() const
{
  return coordinator_host_;
}

inline Int32 ConsumerMetadataResponse::coordinator_port() const
{
  return coordinator_port_;
}

inline void MutableConsumerMetadataResponse::set_error_code(
  Int16 error_code)
{
  response_.error_code_ = error_code;
}

inline void MutableConsumerMetadataResponse::set_coordinator_id(
  Int32 coordinator_id)
{
  response_.coordinator_id_ = coordinator_id;
}

inline void MutableConsumerMetadataResponse::set_coordinator_host(
  const String& coordinator_host)
{
  response_.coordinator_host_ = coordinator_host;
}

inline void MutableConsumerMetadataResponse::set_coordinator_port(
  Int32 coordinator_port)
{
  response_.coordinator_port_ = coordinator_port;
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/consumer_metadata_response_read.h>

#endif  // CONSUMER_METADATA_RESPONSE_H_9DC3DEAB_1C5B_4B67_A8F5_FBB39CB8F065
