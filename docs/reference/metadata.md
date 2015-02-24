
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

-------------------------------------------------------------------------------


class `MetadataResponse`
========================

**Header File:** `<libkafka_asio/metadata_response.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka Metadata response, as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-MetadataResponse).
An object of this type will be given as response object to the handler function
when invoking a metadata request.

<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[MetadataResponse]++-*[Broker], 
[MetadataResponse]++-*[TopicMetadata], 
[TopicMetadata]++-*[PartitionMetadata]" 
/>

Member Functions
----------------

### OptionalBrokerType **PartitionLeader** (const String& topic, Int32 partition) const

Returns the broker, which is currently acting as leader for the given topic
partition. The function basically looks for the given topic and partition in
the data of this response object and tries to find the broker with `node_id` set
to the value specified in the `leader` field of the partition metadata
structure. The return value of this function is wrapped using _Boost optional_.
It is empty in case the given topic partition or leading broker cannot be found
in the data of this metadata response object.

```cpp
// Assume the response is an argument of the request handler function
MetadataResponse::OptionalType response;

MetadataResponse::OptionalBrokerType leader = response->PartitionLeader("foo", 1);
if (leader)
{
    std::cout << "Found leader: " << leader->host << ":" << leader->port << std::endl;
}
```

### const BrokerVector& **broker** () const

Returns a reference to the received broker metadata. See the `Broker` data
structure below.

### const TopicMetadataVector& **topic_metadata** () const

Returns a reference to the received topic partition metadata. See the
description for `TopicMetadata` and `PartitionMetadata` types below.

Types
-----

### struct **Broker**

+ `node_id`:
   ID of the Kafka broker
+ `host`:
   Kafka broker hostname
+ `port`:
   Kafka broker port
   
### struct **PartitionMetaData**

+ `partition_error_code`:
   Kafka error code for this partition.
+ `partition`:
   Number, identifying this topic partition.
+ `leader`:
   Node ID of the Kafka broker, which is currently acting as leader for this
   partition.
+ `replicas`:
   Set of Kafka broker node IDs that are currently acting as slaves for the
   leader for this partition.
+ `isr`:
   Set of Kafka broker node IDs that are "caught up" to the leader broker.

### struct **TopicMetadata**

+ `topic_error_code`:
   Kafka error code for this topic.
+ `topic_name`:
   Name of this topic.
+ `partition_metadata`:
   Metadata for each requested partition of this topic. This can be empty in
   case of an error for this topic (e.g. in case the topic cannot be found on
   the connected broker).

### typedef boost::optional<MetadataResponse\> **OptionalType**
A metadata response object wrapped using _Boost optional_. Such an object will
be used for metadata request handler functions.

### typedef std::vector<Broker\> **BrokerVector**
Set of brokers.
   
### typedef std::vector<TopicMetadata\> **TopicMetadataVector**
Set of topic metadata structures.

### typedef boost::optional<Broker\> **OptionalBrokerType**
Broker data structure wrapped using _Boost optional_.
   