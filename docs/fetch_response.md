
class `FetchResponse`
=====================

**Header File:** `<libkafka_asio/fetch_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka FetchResponse as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-FetchResponse).
An object of this type will be given as response object to the handler function
when invoking a fetch request.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[FetchResponse]++-*[Topic], 
[Topic]++-*[TopicPartition]" 
/>


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
