//
// consumer_metadata_request.h
// ---------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CONSUMER_METADATA_REQUEST_H_4C2E5A42_11B2_4982_87DE_CA3F8B82AF76
#define CONSUMER_METADATA_REQUEST_H_4C2E5A42_11B2_4982_87DE_CA3F8B82AF76

#include <libkafka_asio/request.h>
#include <libkafka_asio/consumer_metadata_response.h>

namespace libkafka_asio
{

// Kafka Offset Commit/Fetch API request implementation:
// ConsumerMetadataRequest
class ConsumerMetadataRequest :
  public Request<ConsumerMetadataRequest>
{
  friend class Request<ConsumerMetadataRequest>;

  static Int16 ApiKey();

public:
  typedef ConsumerMetadataResponse ResponseType;
  typedef MutableConsumerMetadataResponse MutableResponseType;

  const String& consumer_group() const;

  // The consumer-metadata-request will fetch the coordinating broker for the
  // consumer group specified in the request.
  void set_consumer_group(const String& consumer_group);

private:
  String consumer_group_;
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/consumer_metadata_request.h>

#endif  // CONSUMER_METADATA_REQUEST_H_4C2E5A42_11B2_4982_87DE_CA3F8B82AF76
