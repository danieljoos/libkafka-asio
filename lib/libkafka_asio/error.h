//
// error.h
// -------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef ERROR_H_20BBD26A_1B33_4F9E_94E9_5989EC105D5D
#define ERROR_H_20BBD26A_1B33_4F9E_94E9_5989EC105D5D

#include <boost/system/error_code.hpp>

#ifndef BOOST_SYSTEM_NOEXCEPT
#define BOOST_SYSTEM_NOEXCEPT
#endif

namespace libkafka_asio
{

enum ClientError
{
  kErrorSuccess = 0,
  kErrorAlreadyConnected,
  kErrorNotConnected,
  kErrorNoBroker,
  kErrorInProgress,
  kErrorCompressionNotAvailable,
  kErrorCompressionFailed
};

enum KafkaError
{
  kErrorNoError = 0,
  kErrorUnknown = -1,
  kErrorOffsetOutOfRange = 1,
  kErrorInvalidMessage = 2,
  kErrorUnknownTopicOrPartition = 3,
  kErrorInvalidMessageSize = 4,
  kErrorLeaderNotAvailable = 5,
  kErrorNotLeaderForPartition = 6,
  kErrorRequestTimedOut = 7,
  kErrorBrokerNotAvailable = 8,
  kErrorReplicaNotAvailable = 9,
  kErrorMessageSizeTooLarge = 10,
  kErrorStaleControllerEpochCode = 11,
  kErrorOffsetMetadataTooLargeCode = 12,
  kErrorOffsetLoadInProgressCode = 14,
  kErrorConsumerCoordinatorNotAvailableCode = 15,
  kErrorNotCoordinatorForConsumerCode = 16
};

class ClientErrorCategory :
  public boost::system::error_category
{
public:
  inline const char *name() const BOOST_SYSTEM_NOEXCEPT
  {
    return "libkafka_asio::ClientError";
  }

  inline std::string message(int value) const
  {
    switch (value)
    {
      case kErrorAlreadyConnected:
        return "Client is already connected to remote host";
      case kErrorNotConnected:
        return "Client is not connected";
      case kErrorNoBroker:
        return "No broker found";
      case kErrorInProgress:
        return "Another operation is still in progress for this client";
      case kErrorCompressionNotAvailable:
        return "The specified compression algorithm is not available!";
      case kErrorCompressionFailed:
        return "Data compression failure!";
      default:
        return "libkafka_asio client error";
    }
  }

  inline static const boost::system::error_category& Instance()
  {
    static ClientErrorCategory instance;
    return instance;
  }
};

class KafkaErrorCategory :
  public boost::system::error_category
{
public:
  inline const char *name() const BOOST_SYSTEM_NOEXCEPT
  {
    return "libkafka_asio::KafkaError";
  }

  inline std::string message(int value) const
  {
    switch (value)
    {
      case kErrorNoError:
        return "No error";
      case kErrorUnknown:
        return "Unexpected server error";
      case kErrorOffsetOutOfRange:
        return "The requested offset is outside the range of offsets "
          "maintained by the server for the given topic/partition.";
      case kErrorInvalidMessage:
        return "message content does not match its CRC.";
      case kErrorUnknownTopicOrPartition:
        return "Topic or partition does not exist on this broker.";
      case kErrorInvalidMessageSize:
        return "The message has a negative size";
      case kErrorLeaderNotAvailable:
        return "There is currently no leader for this partition and hence it "
          "is unavailable for writes.";
      case kErrorNotLeaderForPartition:
        return "Message was sent to a replica that is not the leader for this"
          " partition. Client metadata is out of date.";
      case kErrorRequestTimedOut:
        return "Request exceeded the user-specified time limit";
      case kErrorBrokerNotAvailable:
        return "Broker not available.";
      case kErrorReplicaNotAvailable:
        return "Replica was expected on this broker but is not available.";
      case kErrorMessageSizeTooLarge:
        return "Message was too large";
      case kErrorStaleControllerEpochCode:
        return "Internal error StaleControllerEpochCode";
      case kErrorOffsetMetadataTooLargeCode:
        return "Specified string larger than configured maximum for offset "
          "metadata";
      case kErrorOffsetLoadInProgressCode:
        return "Offset fetch request is still loading offsets";
      case kErrorConsumerCoordinatorNotAvailableCode:
        return "Offsets topic has not yet been created.";
      case kErrorNotCoordinatorForConsumerCode:
        return "Request was for a consumer group that is not coordinated by "
          "this broker.";
      default:
        return "libkafka_asio kafka error";
    }
  }

  inline static const boost::system::error_category& Instance()
  {
    static KafkaErrorCategory instance;
    return instance;
  }
};

//
// boost::system integration code following:
//

inline boost::system::error_code make_error_code(ClientError e)
{
  return boost::system::error_code(
    static_cast<int>(e), ClientErrorCategory::Instance());
}

inline boost::system::error_code make_error_code(KafkaError e)
{
  return boost::system::error_code(
    static_cast<int>(e), KafkaErrorCategory::Instance());
}

}  // namespace libkafka_asio

namespace boost
{
namespace system
{

template<>
struct is_error_code_enum<libkafka_asio::ClientError>
{
  static const bool value = true;
};

template<>
struct is_error_code_enum<libkafka_asio::KafkaError>
{
  static const bool value = true;
};

}  // namespace system
}  // namespace boost

#endif  // ERROR_H_20BBD26A_1B33_4F9E_94E9_5989EC105D5D
