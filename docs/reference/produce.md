
class `ProduceRequest`
======================

**Header File:** `<libkafka_asio/produce_request.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka ProduceRequest as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-ProduceRequest).
Produce requests are used to send data for one or more topic partitions to a
remote Kafka server.

Member Functions
----------------

### void **AddValue** \(const Bytes& value, const String& topic_name, Int32 partition\)

Constructs a `Message` with value set to byte vector given in `value`. The
message will be added to this request object to produce it for the topic
partition with the given `topic_name` and `partition`.
The given value will be copied.

### void **AddValue** \(const String& value, const String& topic_name, Int32 partition\)

Constructs a `Message` with value set to the bytes of the given `value` string.
The message will be added to this request object to produce it for the topic
partition with the given `topic_name` and `partition`.

### void **AddMessage** \(const Message& message, const String& topic_name, Int32 partition\)

Adds a copy of the given message to this produce request. The message will be
produced for the given `topic_name` and `partition`.

### void **AddMessageSet** \(const MessageSet& message_set, const String& topic_name, Int32 partition\)

Copies the given set of message into this produce request. It has the same
effect as calling `AddMessage` for each message inside the given `MessageSet`.
The messages will be produced for the given `topic_name` and `partition`.

### void **Clear** ()

Clears all message data of this produce request.

### void **ClearTopic** (const String& topic_name)

Clears the message data for the topic with the given name.

### bool **ResponseExpected** () const

Returns `true` in case a response is expected for this produce request. (Produce
requests are the only kinds of requests where there is the possibility that
the server will not reply with a response). This function is called internally
after the request was successfully sent to the server and the library needs to
determine, if a response is expected.

### void **set_required_acks** (Int16 required_acks)

Sets the number of acknowledgements that need to be received by the server
before the response for this request is sent. If `0` is specified for this
parameter, the server will not wait for acknowledgements. In this case, no
response will be sent by the server.

### void **set_timeout** (Int32 timeout)

Timeout in milliseconds to wait for required acknowledgements.

Types
-----

### struct **TopicPartition**

+ `partition`:
   Number, identifying this topic partition.
+ `messages`:
   Set of messages to produce for this topic partition

### struct **Topic**

+ `topic_name`:
   Name of the topic to produce messages for.
+ `partitions`:
   Vector of `TopicPartition` objects.   
   
### typedef ProduceResponse **ResponseType**
Type of the response object of a produce request.

### typedef std::vector<Topic> **TopicVector**
Vector of `Topic` objects. Produce requests can produce message for multiple
topics and partitions.

-------------------------------------------------------------------------------

class `ProduceResponse`
=======================

**Header File:** `<libkafka_asio/produce_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka ProduceRequest as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-ProduceResponse).
An object of this type will be given as response object to the handler function
when invoking a produce request and the request expected the server to send a
response (which is not always the case for a ProduceRequest).

Member Functions
----------------

### const TopicVector& **topics** () const

Returns a reference to the list of topics, messages where produced for. See the
description of `Topic` type below.

Types
-----

### struct **TopicPartition**

+ `partition`:
   Number, identifying this topic partition.
+ `error_code`:
   Kafka error code for this partition, if any.
+ `offset`:
   Offset assigned to the first message in the set of messages produced for this
   partition.
   
### struct **Topic**

+ `topic_name`:
   Name of this topic.
+ `partitions`:
   Vector of `TopicPartitions`, representing the partitions, for which messages
   have been created.

### typedef std::vector<Topic\> **TopicVector**

Set of topics, messages have been produced for.
