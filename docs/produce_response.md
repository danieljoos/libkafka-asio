
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
[ProduceResponse]++-*[ProduceResponse::Topic],
[ProduceResponse::Topic]++-*[ProduceResponse::Partition]"
/>

Member Functions
----------------

### topics
```cpp
const Topics& topics() const
```

Returns a reference to the list of topics, for which messages have been
produced for. See the description of `Topic` type below.


Types
-----

### Topic
```cpp
struct Topic {
    Partitions partitions;
}
```

+ `partitions`:
   Map of `Partition` objects, for which messages have been produced.


### Partition
```cpp
struct Partition {
    Int16   error_code;
    Int64   offset;
}
```

+ `error_code`:
   Kafka error code for this partition, if any.
+ `offset`:
   Offset assigned to the first message in the set of messages produced for this
   partition.


### Topics
```cpp
typedef std::map<String, Topic> Topics
```

Map that associates the offset response part of topics to their topic names.


### Partitions
```cpp
typedef std::map<Int32, Partition> Partitions
```

Map that associates a `Partition` object to the partition id.


### OptionalType
```cpp
typedef boost::optional<ProduceResponse> OptionalType
```

A produce response object wrapped using _Boost optional_. Such an object will
be used for produce request handler functions.
