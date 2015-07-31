//
// constants.h
// -----------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CONSTANTS_H_D943A30C_46DC_42BD_B60B_B55095C424F1
#define CONSTANTS_H_D943A30C_46DC_42BD_B60B_B55095C424F1

#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{
namespace constants
{

enum ApiKeys
{
  kApiKeyProduceRequest = 0,
  kApiKeyFetchRequest = 1,
  kApiKeyOffsetRequest = 2,
  kApiKeyMetadataRequest = 3,
  kApiKeyOffsetCommitRequest = 8,
  kApiKeyOffsetFetchRequest = 9,
  kApiKeyConsumerMetadataRequest = 10
};

enum Compression
{
  kCompressionNone = 0,
  kCompressionGZIP = 1,
  kCompressionSnappy = 2,
  kCompressionLz4 = 3
};

enum Defaults
{
  kDefaultPartition = 0,
  kDefaultProduceRequiredAcks = 1,
  kDefaultProduceTimeout = 10000,
  kDefaultFetchOffset = 0,
  kDefaultFetchMinBytes = 0,
  kDefaultFetchMaxBytes = 32768,
  kDefaultFetchMaxWaitTime = 0,
  kDefaultOffsetMaxNumberOfOffsets = 1,
  kDefaultOffsetCommitTimestampNow = -1,
  kDefaultCorrelationId = 0,
  kDefaultMessageMaxBytes = 4194304,
  kDefaultSocketTimeout = 60000
};

inline bool DefaultConnectionAutoConnect()
{
  return false;
}

inline const String& DefaultClientId()
{
  static String client_id = "libkafka_asio";
  return client_id;
}

inline const std::string& DefaultKafkaService()
{
  static String service = "9092";
  return service;
}

enum MetadataLeader
{
  kMetadataLeaderUndecided = -1
};

enum OffsetTime
{
  kOffsetTimeLatest = -1,
  kOffsetTimeEarliest = -2
};

}  // namespace constants
}  // namespace libkafka_asio

#endif  // CONSTANTS_H_D943A30C_46DC_42BD_B60B_B55095C424F1
