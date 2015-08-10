#ifndef CONNECTION_CONFIGURATION_H_97A5C774_9202_42D5_BD64_05F2293A3ABD
#define CONNECTION_CONFIGURATION_H_97A5C774_9202_42D5_BD64_05F2293A3ABD

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{

// Connection configuration data structure
struct ConnectionConfiguration
{
  // Broker address configuration data structure
  struct BrokerAddress
  {
    typedef boost::optional<BrokerAddress> OptionalType;
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

  // Automatically connect to one of the known Kafka servers
  bool auto_connect;

  // The broker address, used for auto-connect
  BrokerAddress::OptionalType broker_address;

  // Construct using default values
  ConnectionConfiguration();

  // Set the broker address using the given string.
  // If the string contain a colon, the part before the colon is interpreted
  // as hostname and the part after that character is interpreted as service
  // name.
  // Example: localhost:9092
  //
  void SetBrokerFromString(const std::string& str);

  // Set the broker address using the given data structure.
  // The broker's address is determined by the fields:
  //     - host
  //     - port
  // A lexical cast is done on both fields.
  template<typename T>
  void SetBroker(const T& broker);

  // Set the broker address using the given hostname and service parameter.
  // Both will be casted to string using a lexical_cast.
  template<typename Tx, typename Ty>
  void SetBroker(const Tx& hostname, const Ty& service);

};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/connection_configuration.h>

#endif  // CONNECTION_CONFIGURATION_H_97A5C774_9202_42D5_BD64_05F2293A3ABD
