
# class `ConsumerMetadataRequest`

**Header File:** `<libkafka_asio/consumer_metadata_request.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka ConsumerMetadataRequest as described on the
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-ConsumerMetadataRequest).
ConsumerMetadata requests are used to discover the current offset coordinator
of a Kafka consumer group.

## Member Functions

### set_consumer_group

```cpp
void set_consumer_group(const String& consumer_group)
```

The request will try to discover the offset coordinator for the consumer group
specified.

## Types

### ResponseType

```cpp
typedef ConsumerMetadataResponse ResponseType
```

Type of the response object of a ConsumerMetadata request.

### MutableResponseType

```cpp
typedef MutableConsumerMetadataResponse MutableResponseType
```

Type of a mutable response object for a consumer metadata request. This type is
used by the library at when reading-in the response from a Kafka server.
