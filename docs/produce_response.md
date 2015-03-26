
class `ProduceResponse`
=======================

**Header File:** `<libkafka_asio/produce_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka ProduceResponse as described on the 
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

### topics
```cpp
const TopicVector& topics() const
```

Returns a reference to the list of topics, for which messages have been 
produced for. See the description of `Topic` type below.


Types
-----

### Topic
```cpp
struct Topic
```

+ `topic_name`:
   Name of this topic.
+ `partitions`:
   Vector of `Topic::Partition`, representing the partitions, for which 
   messages have been created.


### Topic::Partition
```cpp
struct Topic::Partition
```

+ `partition`:
   Number, identifying this topic partition.
+ `error_code`:
   Kafka error code for this partition, if any.
+ `offset`:
   Offset assigned to the first message in the set of messages produced for this
   partition.


### TopicVector
```cpp
typedef std::vector<Topic> TopicVector
```

Set of topics, messages have been produced for.
