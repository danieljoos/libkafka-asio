//
// detail/impl/consumer_metadata_response_read.h
// ---------------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CONSUMER_METADATA_RESPONSE_READ_H_68DED213_C18C_4A39_8B5C_B15BB55E9EF8
#define CONSUMER_METADATA_RESPONSE_READ_H_68DED213_C18C_4A39_8B5C_B15BB55E9EF8

#include <libkafka_asio/primitives.h>
#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadResponseMessage(std::istream& is,
                                MutableConsumerMetadataResponse& response,
                                boost::system::error_code& ec)
{
  response.set_error_code(ReadInt16(is));
  response.set_coordinator_id(ReadInt32(is));
  response.set_coordinator_host(ReadString(is));
  response.set_coordinator_port(ReadInt32(is));
  if (response.response().error_code())
  {
    ec = (KafkaError) response.response().error_code();
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // CONSUMER_METADATA_RESPONSE_READ_H_68DED213_C18C_4A39_8B5C_B15BB55E9EF8
