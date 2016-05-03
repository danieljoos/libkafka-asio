
# class `OffsetCommitRequest`

**Header File:** `<libkafka_asio/offset_commit_request.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka OffsetCommitRequest as described on the
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-OffsetCommitRequest).
Offset commit requests are used to commit an offset value for one or more topic
partitions of a Kafka consumer group. These kinds of requests require Kafka
version 0.8.1.1 or above. Offset commit requests must be sent to the current
offset coordinator broker, which can be discovered using a ConsumerMetadata
request.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[OffsetCommitRequest]++-*[OffsetCommitRequest::Topic],
[OffsetCommitRequest::Topic]++-*[OffsetCommitRequest::Partition]"
/>

## Member Functions

### CommitOffset

```cpp
void CommitOffset(const String& topic_name,
                  Int32 partition,
                  Int64 offset,
                  Int64 timestamp,
                  const String& metadata)
```

Adds an entry to this request to commit offset data for the given
topic-partition using the optionally specified timestamp.
If the special value `constants::kDefaultOffsetCommitTimestampNow` (`-1`) is
given for the `timestamp` parameter, then the broker sets the time stamp to
the receive time before committing the offset data.

### Clear

```cpp
void Clear()
```

Clears this request by removing all added commit-offset entries.

### set_consumer_group

```cpp
void set_consumer_group(const String& consumer_group)
```

Set the consumer group to commit the offset data for.

### consumer_group

```cpp
const String& consumer_group() const
```

Returns the consumer group string used by this request.

### topics

```cpp
const Topics& topics() const
```

Returns a reference to the list of topics of this offset commit request. This
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
   Set of partitions of this topic.

### Partition

```cpp
struct Partition {
    Int64   offset;
    Int64   timestamp;
    String  metadata;
    Int32   partition;
}
```

* `partition`:
   Number, identifying this topic partition.
* `offset`:
   Offset that should be committed.
* `timestamp`:
   Timestamp used for the commit.
* `metadata`:
   Additional metadata.

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
typedef OffsetCommitResponse ResponseType
```

Type of the response object of an offset commit request.

### MutableResponseType

```cpp
typedef MutableOffsetCommitResponse MutableResponseType
```

Type of a mutable response object for a offset commit request. This type is used
by the library at when reading-in the response from a Kafka server.
