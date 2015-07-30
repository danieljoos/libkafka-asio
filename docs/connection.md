
class `Client`
==============

**Header File:** `<libkafka_asio/client.h>`

**Namespace:** `libkafka_asio`

The client class does the actual interaction with a Kafka server. Use it to
connect to such a server and send asynchronous requests to it. The client uses
_Boost Asio_ for the TCP-based communication.

### Client
```cpp
Client(boost::asio::io_service& io_service, 
       const Configuration& configuration)
```

Constructs a new client object. All communication to the Kafka server will be
scheduled on the given `io_service` object.


### ~Client
```cpp
~Client()
```

A possible open connection will be closed on destruction of the client object.
All pending asynchronous operations will be cancelled and the respective handler
functions will be called with an `operation_aborted` error.


Member Functions
----------------

### AsyncConnect (overload 1 of 2)
```cpp
void AsyncConnect(const std::string& host,
                  const std::string& service,
                  const ConnectionHandlerType& handler)
```

Asynchronously connects to the Kafka server, identified by the given `hostname`
and `service` (port or service string, e.g. see `/etc/services` under Linux).
The given handler function object will be called on success as well as on error.
The function always returns immediately.

The signature of the handler function must be:
```cpp
void handler(
    const Client::ErrorCodeType& error
);
```

Example:

```cpp
boost::asio::io_service ios;
Client cl(ios);
cl.AsyncConnect("localhost", "9092", [](const Client::ErrorCodeType& error) {
    if (error) {
        std::cerr << "Failed to connect!" << std::endl;
        return;
    }
    std::cout << "Connected!" << std::endl;
});
```


### AsyncConnect (overload 2 of 2)
```cpp
void AsyncConnect(const ConnectionHandlerType& handler)
```

Tries to connect to the brokers, specified in the configuration given to this
client object. If no such broker address was configured, the handler function
will be scheduled with `ErrorNoBroker`.
Connection attempts will be made in the sequence, the broker addresses were
added to the configuration.
The function always returns immediately.

The signature of the handler function must be:
```cpp
void handler(
    const Client::ErrorCodeType& error
);
```

Example:

```cpp
boost::asio::io_service ios;
Client::Configuration conf;
conf.auto_connect = true;
conf.AddBrokerFromString("localhost:9092");
conf.AddBrokerFromString("example.org:9092");
Client cl(ios);
cl.AsyncConnect([](const Client::ErrorCodeType& error) {
    if (error) {
        std::cerr << "Failed to connect!" << std::endl;
        return;
    }
    std::cout << "Connected!" << std::endl;
});
```


### AsyncRequest
```cpp
template<typename TRequest>
void AsyncRequest (const TRequest& request,
                   const typename Handler<TRequest>::Type& handler)
```

Asynchronously sends the given request to the connected Kafka server. The given
handler function object will be called on success as well as on error condition.

If this client object is not in `connected` state, the handler function will be
scheduled with `ErrorNotConnected`. If the `auto-connect` option was enabled in
the configuration, this function will try to connect to one of the brokers,
specified in the configuration (See function `AsyncConnect(handler)`).

The function always returns immediately. The signature of the handler function
must be:

```cpp
void handler(
    const Client::ErrorCodeType& error,
    const Response::OptionalType& response
);
```

The type of the optional response object, handed to the handler function,
depends on the given request type. It will be determined using the response type
specified in the request type (e.g. `FetchRequest::ResponseType`).

Example:

```cpp
boost::asio::io_service ios;
Client::Configuration conf;
conf.auto_connect = true;
conf.AddBrokerFromString("localhost:9092");
conf.AddBrokerFromString("example.org:9092");
Client cl(ios);
MetadataRequest request;
client.AsyncRequest(request, [](const Client::ErrorCodeType& error,
                                const MetadataResponse::OptionalType& response) {
    if (error) {
        std::cerr << "Error!" << std::endl;
        return;
    }
    std::cout << "Got Metadata!" << std::endl;
});
```


### Close
```cpp
void Close()
```

Closes the connection to the Kafka server. All asynchronous operations will be
cancelled immediately with an `operation_aborted` error.


Types
-----

### Configuration
```cpp
typedef ClientConfiguration Configuration
```

Client configuration type.


### ErrorCodeType
```cpp
typedef boost::system::error_code ErrorCodeType
```

Error code type.


### ConnectionHandlerType
```cpp
typedef boost::function<void(const ErrorCodeType&)> ConnectionHandlerType
```

Handler type for connection attempts.


### Handler<TRequest\>::Type
```cpp
template<typename TRequest> Handler<TRequest>::Type
```

Handler type for asynchronous requests.
