
Kafka Protocol Primitives
=========================

**Header File:** `<libkafka_asio/primitives.h>`

**Namespace:** `libkafka_asio`

Find below the type definitions, used by _libkafka-asio_ to represent the
primitive types of the Kafka protocol as described on the
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-ProtocolPrimitiveTypes).

Types
-----

### typedef boost::int8_t **Int8**

### typedef boost::int16_t **Int16**

### typedef boost::int32_t **Int32**

### typedef boost::int64_t **Int64**

### typedef boost::uint8_t **Byte**

### typedef std::string **String**

### typedef boost::shared_ptr<std::vector<Byte\> \> **Bytes**