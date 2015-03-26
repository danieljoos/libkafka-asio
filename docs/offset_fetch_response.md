
class `OffsetFetchResponse`
============================

**Header File:** `<libkafka_asio/offset_fetch_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka OffsetFetchResponse as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-OffsetFetchResponse).
An object of this type will be given as response object to the handler function
when invoking an offset fetch request.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[OffsetFetchResponse]++-*[Topic], 
[Topic]++-*[Topic::Partition]" 
/>


Member Functions
----------------

### topics
```cpp
const TopicVector& topics() const
```

Returns a list of topics of this response


Types
-----

### Topic
```cpp
struct Topic
```

+ `topic_name`:
   Name of the topic to fetch data for.
+ `partition_offsets`:
   Set of partitions of this topic for which offset data has been received.


### Topic::Partition
```cpp
struct Topic::Partition
```

+ `partition`:
   Number, identifying this topic partition.
+ `error_code`:
   Kafka error for this topic partition.
+ `offset`:
   The offset data, stored for this topic partition
+ `metadata`:
   The metadata string, stored for this topic partition


### TopicVector
```cpp
typedef std::vector<Topic> TopicVector
```

Vector of `Topic` structures.


### OptionalType
```cpp
typedef boost::optional<OffsetFetchResponse> OptionalType
```

A offset-fetch response object wrapped using _Boost optional_. Such an object 
will be used for offset-fetch request handler functions.
