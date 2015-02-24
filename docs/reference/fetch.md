
class `FetchRequest`
======================

**Header File:** `<libkafka_asio/fetch_request.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka FetchRequest as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-FetchRequest).
Fetch requests are used to get chunks of data for one or more topic partitions
from a Kafka server.

Member Functions
----------------

### void **FetchTopic** (const String& topic_name, Int32 partition, Int64 fetch_offset, Int32 max_bytes)

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

### void **Clear** ()

Clears this fetch request by removing all topic partition entries.

### void **set_max_wait_time** (Int32 max_wait_time)

Sets the maximum time to wait for message data to become available on the
server. This option can be used in combination with the `min_bytes` parameter.
The timeout must be specified in milliseconds.

### void **set_min_bytes** (Int32 min_bytes)

Sets the minimum number of bytes to wait for on the server side. If this is set
to `0`, the server won't wait at all. If set to `1`, the server waits until
at least 1 byte of the requested topic partition data is available or the 
specified timeout occurs.

```cpp
FetchRequest request;
request.set_max_wait_time(100);
request.set_min_bytes(1);
```

### const TopicVector& **topics** \(\) const

Returns a reference to the list of topics of this fetch request. This
method is mainly used internally for getting the request data during the
conversion to the Kafka wire format.

Types
-----

### struct **TopicPartition**

+ `partition`:
   Number, identifying this topic partition.
+ `fetch_offset`:
   Offset to begin this fetch from.
+ `max_bytes`:
   Maximum amount of bytes to include in the message set for this topic
   partition.
   
### struct **Topic**

+ `topic_name`:
   Name of the topic to fetch data for.
+ `partitions`:
   Set of partitions of this topic to fetch data for.

### typedef FetchResponse **ResponseType**
Type of the response object of a fetch request.

### typedef std::vector<Topic\> **TopicVector**
Vector of topics to fetch data for.

-------------------------------------------------------------------------------

class `FetchResponse`
=====================

**Header File:** `<libkafka_asio/fetch_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka FetchResponse as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-FetchResponse).
An object of this type will be given as response object to the handler function
when invoking a fetch request.

Member Functions
----------------

### const_iterator **begin** () const

Start iterator, used for iterating over all received messages.

```cpp
// Assume the response is an argument of the request handler function
FetchResponse::OptionalType response;

// C++11 range-based for loop
for (auto message : *response)
{
    // Do something with the message...
}

// STL for_each
std::for_each(response->begin(), response->end(), &PrintMessage);

// 'traditional' iteration
FetchResponse::const_iterator iter = response->begin();
FetchResponse::const_iterator end_iter = response->end();
for (; iter != end_iter; ++iter)
{
    // Again do something...
}
```

### const_iterator **end** () const

End iterator (see start iterator description above). Similar to default
construction of the `const_iterator` type.

### const TopicVector& **topics** () const

Returns a reference to the set of topics, messages have been received for.

Types
-----

### struct **TopicPartition**

+ `partition`:
   Number, identifying this topic partition.
+ `error_code`:
   Kafka error for this topic partition.
+ `highwater_mark_offset`:
   Offset at the end of the log for this partition on the server.
   
### struct **Topic**

+ `topic_name`:
   Name of this topic.
+ `partitions`:
   Set of partitions of this topic for which message data has been received.

### typedef std::vector<Topic\> **TopicVector**

Vector of `Topic` structures.

### typedef defail::FetchResponseIterator<TopicVector> **const_iterator**

Constant iterator type, used for iterating over all messages of a fetch
response object. See `FetchResponseIterator` class template for details.
