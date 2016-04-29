//
// produce_request.h
// -----------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef PRODUCE_REQUEST_H_89764979_B97B_47D2_A715_974D015A5D1C
#define PRODUCE_REQUEST_H_89764979_B97B_47D2_A715_974D015A5D1C

#include <vector>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/message.h>
#include <libkafka_asio/request.h>
#include <libkafka_asio/produce_response.h>
#include <libkafka_asio/detail/functional.h>
#include <libkafka_asio/detail/topics_partitions.h>

namespace libkafka_asio
{

// Kafka Produce API request implementation
class ProduceRequest :
  public Request<ProduceRequest>
{
  friend class Request<ProduceRequest>;

  static Int16 ApiKey();

  struct PartitionProperties
  {
    MessageSet messages;
  };

  typedef detail::TopicsPartitionsVector<
    detail::EmptyProperties,
    PartitionProperties
  > TopicsPartitions;

public:
  typedef ProduceResponse ResponseType;
  typedef MutableProduceResponse MutableResponseType;
  typedef TopicsPartitions::TopicType Topic;
  typedef TopicsPartitions::PartitionType Partition;
  typedef TopicsPartitions::TopicsType Topics;
  typedef TopicsPartitions::PartitionsType Partitions;

  ProduceRequest();

  Int16 required_acks() const;

  Int32 timeout() const;

  const Topics& topics() const;

  // Number of acknowledgments that need to be received by the server before
  // the response for this request is sent.
  // If zero (0) is specified for this parameter, the server will no wait for
  // acknowledgements. In this case, no response will be sent by the server.
  void set_required_acks(Int16 required_acks);

  // Time out (in milliseconds) to wait for required acknowledgements
  void set_timeout(Int32 timeout);

  // Add a byte array value for the given topic-partition in this produce
  // request.
  void AddValue(const Bytes& value,
                const String& topic_name,
                Int32 partition = 0);

  // Add a string value for the given topic-partition in this produce request.
  void AddValue(const String& value,
                const String& topic_name,
                Int32 partition = 0);

  // Add a copy of the given message to the specified topic-partition in this
  // produce request.
  void AddMessage(const Message& message,
                  const String& topic_name,
                  Int32 partition = 0);

  // Copy the given set of messages (for the given topic-partition), into this
  // produce request.
  void AddMessageSet(const MessageSet& message_set,
                     const String& topic_name,
                     Int32 partition = 0);

  // Clear all message data of this request
  void Clear();

  // Clear the message data for the Topic with the given name.
  void ClearTopic(const String& topic_name);

  // Clear the message data for the given topic-partition.
  void ClearTopicPartition(const String& topic_name, Int32 partition);

  // Return true, if this request expects a response from the Kafka server
  bool ResponseExpected() const;

private:
  typedef detail::IsTopicWithName<Topic> IsTopicWithName;
  typedef detail::IsTopicPartition<Partition> IsTopicPartition;

  Int16 required_acks_;
  Int32 timeout_;
  Topics topics_;
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/produce_request.h>

#endif  // PRODUCE_REQUEST_H_89764979_B97B_47D2_A715_974D015A5D1C
