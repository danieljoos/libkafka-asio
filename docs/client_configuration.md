
struct `ClientConfiguration`
============================

The data structure, used for configuring a client object.

### **ClientConfiguration** ()

Construct the configuration using default values.

Member Fields
-------------

### Int32 **message_max_bytes**

Maximum number of bytes to transmit for messages. The default value is 4 MB.

### unsigned int **socket_timeout**

Timeout in milliseconds for socket operations. The default value is 1 minute.

### String **client_id**

Client identification string. The default value is `libkafka_asio`.

### BrokerList **broker_list**

List of known Kafka broker servers.

### bool **auto_connect**

If set to `true`, the client will try to automatically connect to one of the
known Kafka servers.

Member Functions
----------------

### void **AddBrokerFromString** (const std::string& string)

Add a broker address from string. If the string contains a colon, the part
before the colon is interpreted as hostname and the part after that character
is interpreted as service name. Example: `localhost:9092`.

Types
-----

### struct **BrokerAddress**

+ `hostname`:
   Broker hostname
+ `service`:
   Broker service (e.g. the port number).
   
### typedef std::vector<BrokerAddress\> **BrokerList**

Vector of `BrokerAddress` data structures.