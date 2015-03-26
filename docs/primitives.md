
Kafka Protocol Primitives
=========================

**Header File:** `<libkafka_asio/primitives.h>`

**Namespace:** `libkafka_asio`

Find below the type definitions, used by _libkafka-asio_ to represent the
primitive types of the Kafka protocol as described on the
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-ProtocolPrimitiveTypes).

Types
-----

### Int8
```cpp
typedef boost::int_t<8>::exact Int8
```

### Int16
```cpp
typedef boost::int_t<16>::exact Int16
```

### Int32
```cpp
typedef boost::int_t<32>::exact Int32
```

### Int64
```cpp
typedef boost::int_t<64>::exact Int64
```

### Byte
```cpp
typedef boost::uint_t<8>::exact Byte
```

### String
```cpp
typedef std::string String
```

### Bytes
```cpp
typedef boost::shared_ptr<std::vector<Byte> > Bytes
```
