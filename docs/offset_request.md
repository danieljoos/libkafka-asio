
# class `OffsetRequest`

**Header File:** `<libkafka_asio/offset_request.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka OffsetRequest as described on the
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-OffsetRequest).
Offset requests are used to get the valid range of offsets for a topic partition
from a Kafka server. They must be send to the Kafka broker, which is currently
acting as leader for that topic partition.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[OffsetRequest]++-*[OffsetRequest::Topic],
[OffsetRequest::Topic]++-*[OffsetRequest::Partition]"
/>

## Member Functions

### FetchTopicOffset

```cpp
void FetchTopicOffset(const String& topic_name,
                      Int32 partition,
                      Int64 time,
                      Int32 max_number_of_offsets)
```

Fetches offset information for the given topic partition. The optional `time`
parameter can be used to ask for messages before a certain time in the past.
Time must be specified in milliseconds. Two special values exist for this
parameters:

* `libkafka_asio::constants::kOffsetTimeLatest` (`-1`) (Default) and
* `libkafka_asio::constants::kOffsetTimeEarliest` (`-2`).

The maximum number of offsets to fetch can optionally be specified as well.

```cpp
OffsetRequest request;

// Fetch latest offset for topic 'foo' partition 0
request.FetchOffset("foo", 0);
```

### Clear

```cpp
void Clear()
```

Clears all entries of this request for fetching offsets of topic partitions.

### replica_id

```cpp
Int32 replica_id() const
```

Always returns `-1`.

### topics

```cpp
const Topics& topics() const
```

Returns a reference to the list of topics of this offset request. This
method is mainly used internally for getting the request data during the
conversion to the Kafka wire format.

## Types

### Topic

```cpp
struct Topic {
    String      topic_name;
    Partitions  partitions;
}
```

* `topic_name`:
   Name of the topic to fetch data for.
* `partitions`:
   Set of partitions of this topic to fetch offset data for.

### Partition

```cpp
struct Partition {
    Int32   partition;
    Int64   time;
    Int32   max_number_of_offsets;
}
```

* `partition`:
   Number, identifying this topic partition.
* `time`:
   Time in milliseconds before current timestamp (see explanation above).
* `max_number_of_offsets`:
   The maximum number of offsets to fetch for this topic partition.

### Topics

```cpp
typedef std::vector<Topic> Topics
```

Vector of `Topic` objects.

### Partitions

```cpp
typedef std::vector<Partition> Partitions
```

Vector of `Partition` objects.

### ResponseType

```cpp
typedef OffsetResponse ResponseType
```

Type of the response object of an offset request.

### MutableResponseType

```cpp
typedef MutableOffsetResponse MutableResponseType
```

Type of a mutable response object for a offset request. This type is used by
the library at when reading-in the response from a Kafka server.
