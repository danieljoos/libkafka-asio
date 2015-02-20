#ifndef CLIENT_CONFIGURATION_H_97A5C774_9202_42D5_BD64_05F2293A3ABD
#define CLIENT_CONFIGURATION_H_97A5C774_9202_42D5_BD64_05F2293A3ABD

#include <string>
#include <vector>
#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{

// Client configuration data structure
struct ClientConfiguration
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

  // Client identification string.
  // Default value is 'libkafka_asio'.
  String client_id;

  // List of known Kafka servers
  BrokerList broker_list;

  // Automatically connect to one of the known Kafka servers
  bool auto_connect;

  // Construct using default values
  ClientConfiguration();

  // Add a broker address from string.
  // If the string contain a colon, the part before the colon is interpreted
  // as hostname and the part after that character is interpreted as service
  // name.
  // Example: localhost:9092
  //
  void AddBrokerFromString(const std::string& str);
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/client_configuration.h>

#endif  // CLIENT_CONFIGURATION_H_97A5C774_9202_42D5_BD64_05F2293A3ABD
