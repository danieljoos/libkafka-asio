
class `FetchRequest`
======================

**Header File:** `<libkafka_asio/fetch_request.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka FetchRequest as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-FetchRequest).
Fetch requests are used to get chunks of data for one or more topic partitions
from a Kafka server.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[FetchRequest]++-*[Topic], 
[Topic]++-*[TopicPartition]" 
/>

Member Functions
----------------

### FetchTopic
```cpp
void FetchTopic(const String& topic_name, 
                Int32 partition, 
                Int64 fetch_offset, 
                Int32 max_bytes)
```

Fetch data for the specified topic partition. If such entry already exists in
this request, it gets overridden. Optionally, the offset to start the fetch
operation from, as well as the maximum number of bytes to fetch, can be
specified.

```cpp
FetchRequest request;

// Fetch messages beginning with offset 1337 from 'foo' partition 0
request.FetchTopic("foo", 0, 1337);

// Fetch messages beginning with offset 0 of 'bar' partition 1
request.FetchTopic("bar", 1);
```


### Clear
```cpp
void Clear()
```

Clears this fetch request by removing all topic partition entries.


### set_max_wait_time
```cpp
void set_max_wait_time(Int32 max_wait_time)
```

Sets the maximum time to wait for message data to become available on the
server. This option can be used in combination with the `min_bytes` parameter.
The timeout must be specified in milliseconds.


### set_min_bytes
```cpp
void set_min_bytes(Int32 min_bytes)
```

Sets the minimum number of bytes to wait for on the server side. If this is set
to `0`, the server won't wait at all. If set to `1`, the server waits until
at least 1 byte of the requested topic partition data is available or the 
specified timeout occurs.

```cpp
FetchRequest request;
request.set_max_wait_time(100);
request.set_min_bytes(1);
```


### topics
```cpp
const TopicVector& topics() const
```

Returns a reference to the list of topics of this fetch request. This
method is mainly used internally for getting the request data during the
conversion to the Kafka wire format.


Types
-----

### Topic
```cpp
struct Topic
```

+ `topic_name`:
   Name of the topic to fetch data for.
+ `partitions`:
   Set of partitions of this topic to fetch data for.


### Topic::Partition
```cpp
struct Topic::Partition
```

+ `partition`:
   Number, identifying this topic partition.
+ `fetch_offset`:
   Offset to begin this fetch from.
+ `max_bytes`:
   Maximum amount of bytes to include in the message set for this topic
   partition.


### ResponseType
```cpp
typedef FetchResponse ResponseType
```

Type of the response object of a fetch request.


### MutableResponseType
```cpp
typedef MutableFetchResponse MutableResponseType
```

Type of a mutable response object for a fetch request. This type is used by 
the library at when reading-in the response from a Kafka server.


### TopicVector
```cpp
typedef std::vector<Topic> TopicVector
```

Vector of topics to fetch data for.
