//
// consumer_metadata_response.h
// ----------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CONSUMER_METADATA_RESPONSE_H_426CF245_E113_4A70_A614_247BF83944DE
#define CONSUMER_METADATA_RESPONSE_H_426CF245_E113_4A70_A614_247BF83944DE

#include <libkafka_asio/primitives.h>
#include <libkafka_asio/response.h>

namespace libkafka_asio
{

class MutableConsumerMetadataResponse;

// Kafka Offset Commit/Fetch API response implementation:
// ConsumerMetadataResponse
class ConsumerMetadataResponse :
  public Response<ConsumerMetadataResponse>
{
  friend class MutableConsumerMetadataResponse;

public:

  Int16 error_code() const;

  Int32 coordinator_id() const;

  const String& coordinator_host() const;

  Int32 coordinator_port() const;

private:
  Int16 error_code_;
  Int32 coordinator_id_;
  String coordinator_host_;
  Int32 coordinator_port_;
};

class MutableConsumerMetadataResponse :
  public MutableResponse<ConsumerMetadataResponse>
{
public:

  void set_error_code(Int16 error_code);

  void set_coordinator_id(Int32 coordinator_id);

  void set_coordinator_host(const String& coordinator_host);

  void set_coordinator_port(Int32 coordinator_port);
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/consumer_metadata_response.h>

#endif  // CONSUMER_METADATA_RESPONSE_H_426CF245_E113_4A70_A614_247BF83944DE
