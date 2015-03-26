
class `ConsumerMetadataResponse`
================================

**Header File:** `<libkafka_asio/consumer_metadata_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka ConsumerMetadataResponse as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-ConsumerMetadataResponse).
An object of this type will be given as response object to the handler function
when invoking a ConsumerMetadata request.


Member Functions
----------------

### error_code
```cpp
Int16 error_code() const
```

Returns the error code, received from the Kafka broker. The error code is zero
in case of no error.


### coordinator_id
```cpp
Int32 coordinator_id() const
```

Broker ID of the coordinator for the requested consumer group.


### coordinator_host
```cpp
const String& coordinator_host() const
```

Hostname of the coordinator for the requested consumer group.


### coordinator_port
```cpp
Int32 coordinator_port() const
```

Port number of the coordinator for the requested consumer group.


Types
-----

### OptionalType
```cpp
typedef boost::optional<ConsumerMetadataResponse> OptionalType
```

A consumer metadata response object wrapped using _Boost optional_. Such an 
object will be used for consumer metadata request handler functions.
