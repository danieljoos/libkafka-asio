
class `OffsetCommitResponse`
============================

**Header File:** `<libkafka_asio/offset_commit_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka OffsetCommitResponse as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-OffsetCommitResponse).
An object of this type will be given as response object to the handler function
when invoking an offset commit request.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[OffsetCommitResponse]++-*[Topic], 
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


### TopicVector
```cpp
typedef std::vector<Topic> TopicVector
```

Vector of `Topic` structures.


### OptionalType
```cpp
typedef boost::optional<OffsetCommitResponse> OptionalType
```

A offset-commit response object wrapped using _Boost optional_. Such an object 
will be used for offset-commit request handler functions.
