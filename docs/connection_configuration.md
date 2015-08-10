
struct `ConnectionConfiguration`
============================

The data structure, used for configuring a connection object.

### **ConnectionConfiguration** ()

Construct the configuration using default values.

Member Fields
-------------

### message_max_bytes
```cpp
Int32 message_max_bytes
```

Maximum number of bytes to transmit for messages. The default value is 4 MB.


### socket_timeout
```cpp
unsigned int socket_timeout
```

Timeout in milliseconds for socket operations. The default value is 1 minute.


### client_id
```cpp
String client_id
```

Client identification string. The default value is `libkafka_asio`.


### broker_address
```cpp
BrokerAddress::OptionalType broker_address
```

The optional broker address, used in case `auto_connect` is enabled.


### auto_connect
```cpp
bool auto_connect
```

If set to `true`, the connection will try to automatically connect to one of the
known Kafka servers.


Member Functions
----------------

### SetBrokerFromString
```cpp
void SetBrokerFromString(const std::string& string)
```

Set the auto-connect broker address from string. If the string contains a 
colon, the part before the colon is interpreted as hostname and the part 
after that character is interpreted as service name. Example: `localhost:9092`.


Types
-----

### BrokerAddress
```cpp
struct BrokerAddress
```

+ `hostname`:
   Broker hostname
+ `service`:
   Broker service (e.g. the port number).


### BrokerAddress::OptionalType
```cpp
typedef boost::optional<BrokerAddress> OptionalType
```

Optional broker address.
