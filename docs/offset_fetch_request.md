
class `OffsetFetchRequest`
===========================

**Header File:** `<libkafka_asio/offset_fetch_request.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka OffsetFetchRequest as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-OffsetFetchRequest).
Offset fetch requests are used to retrieve an offset value for one or more topic
partitions of a Kafka consumer group. These kinds of requests require Kafka
version 0.8.1.1 or above. Offset fetch requests must be sent to the current
offset coordinator broker, which can be discovered using a ConsumerMetadata 
request.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[OffsetFetchRequest]++-*[Topic], 
[Topic]++-*[Topic::Partition]" 
/>

Member Functions
----------------

### FetchOffset
```cpp
void FetchOffset(const String& topic_name,
                 Int32 partition)
```

Fetch offset data for the given topic partition


### set_consumer_group
```cpp
void set_consumer_group(const String& consumer_group)
```

Set the consumer group to fetch the offset data for.


### topics
```cpp
const TopicVector& topics() const
```

Returns a reference to the list of topics of this offset fetch request. This
method is mainly used internally for getting the request data during the
conversion to the Kafka wire format.


### consumer_group
```cpp
const String& consumer_group() const
```

Returns the consumer group string of this offset-fetch request.


Types
-----

### Topic
```cpp
struct Topic
```

+ `topic_name`:
   Name of the topic to fetch data for.
+ `partitions`:
   Set of partitions of this topic.


### Topic::Partition
```cpp
struct Topic::Partition
```

+ `partition`:
   Number, identifying this topic partition.
+ `offset`:
   Offset that should be committed.
+ `timestamp`:
   Timestamp used for the commit.
+ `metadata`:
   Additional metadata.


### ResponseType
```cpp
typedef OffsetFetchResponse ResponseType
```

Type of the response object of an offset fetch request.


### MutableResponseType
```cpp
typedef MutableOffsetFetchResponse MutableResponseType
```

Type of a mutable response object for a offset fetch request. This type is used
by the library at when reading-in the response from a Kafka server.


### TopicVector
```cpp
typedef std::vector<Topic> TopicVector
```

Vector of topics.
