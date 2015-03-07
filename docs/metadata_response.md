
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
[MetadataResponse]++-*[Topic], 
[Topic]++-*[Topic::Partition]" 
/>

Member Functions
----------------

### Broker::OptionalType **PartitionLeader** (const String& topic, Int32 partition) const

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

MetadataResponse::Broker::OptionalType leader = response->PartitionLeader
("foo", 1);
if (leader)
{
    std::cout << "Found leader: " << leader->host << ":" << leader->port << std::endl;
}
```

### const BrokerVector& **brokers** () const

Returns a reference to the set of received broker metadata. See the `Broker` 
data structure below.

### const TopicVector& **topics** () const

Returns a reference to the received topic partition metadata. See the
description for `Topic` and `Topic::Partition` types below.

Types
-----

### struct **Broker**

+ `node_id`:
   ID of the Kafka broker
+ `host`:
   Kafka broker hostname
+ `port`:
   Kafka broker port
   
### struct **Topic::Partition**

+ `error_code`:
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

### struct **Topic**

+ `error_code`:
   Kafka error code for this topic.
+ `topic_name`:
   Name of this topic.
+ `partitions`:
   Metadata for each requested partition of this topic. This can be empty in
   case of an error for this topic (e.g. in case the topic cannot be found on
   the connected broker).

### typedef boost::optional<MetadataResponse\> **OptionalType**
A metadata response object wrapped using _Boost optional_. Such an object will
be used for metadata request handler functions.

### typedef std::vector<Broker\> **BrokerVector**
Set of brokers.
   
### typedef std::vector<Topic\> **TopicVector**
Set of topic metadata structures.

### typedef boost::optional<Broker\> **Broker::OptionalType**
Broker data structure wrapped using _Boost optional_.
   