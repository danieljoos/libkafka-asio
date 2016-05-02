
# class `OffsetFetchResponse`

**Header File:** `<libkafka_asio/offset_fetch_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka OffsetFetchResponse as described on the
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-OffsetFetchResponse).
An object of this type will be given as response object to the handler function
when invoking an offset fetch request.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[OffsetFetchResponse]++-*[OffsetFetchResponse::Topic],
[OffsetFetchResponse::Topic]++-*[OffsetFetchResponse::Partition]"
/>

## Member Functions

### topics

```cpp
const Topics& topics() const
```

Returns a list of topics of this response

## Types

### Topic

```cpp
struct Topic {
    Partitions partitions;
}
```

* `partitions`:
   Set of partitions of this topic for which consumer group offset data has been
   received.

### Partition

```cpp
struct Partition {
    Int64   offset;
    String  metadata;
    Int16   error_code;
}
```

* `offset`:
   The offset data, stored for this topic partition
* `metadata`:
   The metadata string, stored for this topic partition
* `error_code`:
   Kafka error for this topic partition.

### Topics

```cpp
typedef std::map<String, Topic> Topics
```

Map that associates the offset fetch response part of topics to their topic names.

### Partitions

```cpp
typedef std::map<Int32, Partition> Partitions
```

Map that associates a `Partition` object to the partition id.

### OptionalType

```cpp
typedef boost::optional<OffsetFetchResponse> OptionalType
```

A offset-fetch response object wrapped using _Boost optional_. Such an object
will be used for offset-fetch request handler functions.
