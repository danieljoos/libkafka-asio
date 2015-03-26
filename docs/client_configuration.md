
struct `ClientConfiguration`
============================

The data structure, used for configuring a client object.

### **ClientConfiguration** ()

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


### broker_list
```cpp
BrokerList broker_list
```

List of known Kafka broker servers.


### auto_connect
```cpp
bool auto_connect
```

If set to `true`, the client will try to automatically connect to one of the
known Kafka servers.


Member Functions
----------------

### AddBrokerFromString
```cpp
void AddBrokerFromString(const std::string& string)
```

Add a broker address from string. If the string contains a colon, the part
before the colon is interpreted as hostname and the part after that character
is interpreted as service name. Example: `localhost:9092`.


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


### BrokerList
```cpp
typedef std::vector<BrokerAddress> BrokerList
```

Vector of `BrokerAddress` data structures.
