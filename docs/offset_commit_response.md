
# class `OffsetCommitResponse`

**Header File:** `<libkafka_asio/offset_commit_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka OffsetCommitResponse as described on the
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-OffsetCommitResponse).
An object of this type will be given as response object to the handler function
when invoking an offset commit request.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[OffsetCommitResponse]++-*[OffsetCommitResponse::Topic],
[OffsetCommitResponse::Topic]++-*[OffsetCommitResponse::Partition]"
/>

## Member Functions

### topics

```cpp
const Topics& topics() const
```

Returns the data of this API response object, sorted by topic.

## Types

### Topic

```cpp
struct Topic {
    Partitions partitions;
}
```

* `partitions`:
   The partition objects contained in this topic object.

### Partition

```cpp
struct Partition {
    Int16 error_code;
}
```

* `error_code`:
   Kafka error for this topic partition.

### Topics

```cpp
typedef std::map<String, Topic> Topics
```

Map that associates the offset commit response part of topics to their topic names.

### Partitions

```cpp
typedef std::map<Int32, Partition> Partitions
```

Map that associates a `Partition` object to the partition id.

### OptionalType

```cpp
typedef boost::optional<OffsetCommitResponse> OptionalType
```

A offset-commit response object wrapped using _Boost optional_. Such an object
will be used for offset-commit request handler functions.
