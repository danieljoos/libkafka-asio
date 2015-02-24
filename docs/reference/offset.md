
class `OffsetRequest`
=====================

**Header File:** `<libkafka_asio/offset_request.h>`

**Namespace:** `libkafka_asio`

Implementation of the Kafka OffsetRequest as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-OffsetRequest).
Offset requests are used to get the valid range of offsets for a topic partition
from a Kafka server. They must be send to the Kafka broker, which is currently
acting as leader for that topic partition.

Member Functions
----------------

