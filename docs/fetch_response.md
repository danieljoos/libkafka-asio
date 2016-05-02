
# class `FetchResponse`

**Header File:** `<libkafka_asio/fetch_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka FetchResponse as described on the
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-FetchResponse).
An object of this type will be given as response object to the handler function
when invoking a fetch request.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[FetchResponse]++-*[FetchResponse::Topic],
[FetchResponse::Topic]++-*[FetchResponse::Partition]"
/>

## Member Functions

### begin

```cpp
const_iterator begin() const
```

Creates and returns an iterator object that can be used for iterating over all
received messages. This function returns the start iterator object.

```cpp
// Assume the response is an argument of the request handler function
FetchResponse::OptionalType response;

// C++11 range-based for loop
for (auto message : *response)
{
    // Do something with the message...
}
```

```cpp
// STL for_each: Call a function for each received Message
std::for_each(response->begin(), response->end(), &PrintMessage);
```

```cpp
// 'traditional' iteration
FetchResponse::const_iterator iter = response->begin();
FetchResponse::const_iterator end_iter = response->end();
for (; iter != end_iter; ++iter)
{
    // Again, do something...
}
```

### end

```cpp
const_iterator end() const
```

End iterator (see start iterator description above). Similar to default
construction of the `const_iterator` type.

### topics

```cpp
const Topics& topics() const
```

Returns a reference to the set of topics, messages have been received for.

## Types

### Topic

```cpp
struct Topic {
    Partitions partitions;
}
```

* `partitions`:
   Map of partitions of this topic for which message data has been received.

### Partition

```cpp
struct Partition {
    Int16       error_code;
    Int64       highwater_mark_offset;
    MessageSet  messages;
}
```

* `error_code`:
   Kafka error for this topic partition.
* `highwater_mark_offset`:
   Offset at the end of the log for this partition on the server.
* `messages`:
   The fetched messages.

### Topics

```cpp
typedef std::map<String, Topic> Topics
```

Map that associates a `Topic` object to the topic name.

### Partitions

```cpp
typedef std::map<Int32, Partition> Partitions
```

Map that associates a `Partition` object to the partition id.

### const_iterator

```cpp
typedef defail::FetchResponseIterator<TopicVector> const_iterator
```

Constant iterator type, used for iterating over all messages of a fetch
response object. See `FetchResponseIterator` class template for details.

### OptionalType

```cpp
typedef boost::optional<FetchResponse> OptionalType
```

A fetch response object wrapped using _Boost optional_. Such an object will
be used for fetch request handler functions.
