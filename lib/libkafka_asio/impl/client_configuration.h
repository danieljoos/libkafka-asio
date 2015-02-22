#ifndef CLIENT_CONFIGURATION_H_0435D34D_97AC_4D67_8E3A_DEBAAC218C4D
#define CLIENT_CONFIGURATION_H_0435D34D_97AC_4D67_8E3A_DEBAAC218C4D

#include <libkafka_asio/constants.h>

namespace libkafka_asio
{

inline ClientConfiguration::ClientConfiguration() :
  message_max_bytes(constants::kDefaultMessageMaxBytes),
  socket_timeout(constants::kDefaultSocketTimeout),
  client_id(constants::DefaultClientId()),
  auto_connect(constants::DefaultClientAutoConnect())
{
}

inline void ClientConfiguration::AddBrokerFromString(const std::string& str)
{
  if (str.empty())
  {
    return;
  }
  BrokerAddress broker;
  std::string::size_type delimiter_position = str.find(':');
  if (delimiter_position != std::string::npos &&
    delimiter_position > 0 &&
    delimiter_position < str.size() - 1)
  {
    broker.hostname = str.substr(0, delimiter_position);
    broker.service = str.substr(delimiter_position + 1);
  }
  else
  {
    broker.hostname = str.substr(0, delimiter_position);
    broker.service = constants::DefaultKafkaService();
  }
  if (broker.hostname.empty() || broker.service.empty())
  {
    return;
  }
  broker_list.push_back(broker);
}

}  // namespace libkafka_asio

#endif  // CLIENT_CONFIGURATION_H_0435D34D_97AC_4D67_8E3A_DEBAAC218C4D
