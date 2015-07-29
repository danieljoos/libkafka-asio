#ifndef CONNECTION_CONFIGURATION_H_97A5C774_9202_42D5_BD64_05F2293A3ABD
#define CONNECTION_CONFIGURATION_H_97A5C774_9202_42D5_BD64_05F2293A3ABD

#include <string>
#include <vector>
#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{

// Connection configuration data structure
struct ConnectionConfiguration
{
  // Broker address configuration data structure
  struct BrokerAddress
  {
    std::string hostname;
    std::string service;
  };

  // List of broker address configurations
  typedef std::vector<BrokerAddress> BrokerList;

  // Maximum number of bytes to transmit for messages.
  // Default value is 4194304 (4 MB).
  Int32 message_max_bytes;

  // Timeout (in milliseconds) for socket operations.
  // Default value is 60000 (1 min).
  unsigned int socket_timeout;

  // Connection identification string.
  // Default value is 'libkafka_asio'.
  String client_id;

  // List of known Kafka servers
  BrokerList broker_list;

  // Automatically connect to one of the known Kafka servers
  bool auto_connect;

  // Construct using default values
  ConnectionConfiguration();

  // Add a broker address from string.
  // If the string contain a colon, the part before the colon is interpreted
  // as hostname and the part after that character is interpreted as service
  // name.
  // Example: localhost:9092
  //
  void AddBrokerFromString(const std::string& str);

  // Add a broker from using the given data structure.
  // The broker's address is determined by the fields:
  //     - host
  //     - port
  // A lexical cast is done on both fields.
  template< typename T >
  void AddBroker(const T& broker);

  // Add a broker using the given hostname and service parameter.
  // Both will be casted to string using a lexical_cast.
  template<typename Tx, typename Ty>
  void AddBroker(const Tx& hostname, const Ty& service);

  // Add an existing BrokerAddress data structure.
  // This is the same as simply pushing the given object to the broker_list.
  void AddBroker(const BrokerAddress& broker_address);
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/connection_configuration.h>

#endif  // CONNECTION_CONFIGURATION_H_97A5C774_9202_42D5_BD64_05F2293A3ABD
