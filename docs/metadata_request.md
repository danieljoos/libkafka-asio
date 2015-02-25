
class `MetadataRequest`
=======================

**Header File:** `<libkafka_asio/metadata_request.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka TopicMetadata request, as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-TopicMetadataRequest).
Metadata requests can be used to determine information about topics and 
partitions on the connected broker/cluster.
The leader for a topic partition can be retrieved via a metadata request.

Member Functions
----------------

### void **AddTopicName** \(const String& topic_name\)

Adds the given topic to this metadata request. This results in metadata being
fetched for that topic.

```cpp
using libkafka_asio::MetadataRequest;
MetadataRequest req;
req.AddTopicName("foo");
req.AddTopicName("bar");
```

### const TopicNameVector& **topic_names** \(\) const

Returns a reference to the list of topic names of this metadata request. This
method is mainly used internally for getting the request data during the
conversion to the Kafka wire format.

Types
-----

### typedef MetadataResponse **ResponseType**
Type of the response object of a metadata request.

### typedef std::vector<String\> **TopicNameVector**
Vector of topic name strings.
   