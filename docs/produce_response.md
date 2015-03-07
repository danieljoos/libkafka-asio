
class `ProduceResponse`
=======================

**Header File:** `<libkafka_asio/produce_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka ProduceRequest as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-ProduceResponse).
An object of this type will be given as response object to the handler function
when invoking a produce request and the request expected the server to send a
response (which is not always the case for a ProduceRequest).

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[ProduceResponse]++-*[Topic], 
[Topic]++-*[Topic::Partition]" 
/>

Member Functions
----------------

### const TopicVector& **topics** () const

Returns a reference to the list of topics, messages where produced for. See the
description of `Topic` type below.

Types
-----

### struct **Topic::Partition**

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
   Vector of `Topic::Partition`, representing the partitions, for which 
   messages have been created.

### typedef std::vector<Topic\> **TopicVector**

Set of topics, messages have been produced for.
