#ifndef CONNECTION_CONFIGURATION_H_0435D34D_97AC_4D67_8E3A_DEBAAC218C4D
#define CONNECTION_CONFIGURATION_H_0435D34D_97AC_4D67_8E3A_DEBAAC218C4D

#include <libkafka_asio/constants.h>
#include <boost/lexical_cast.hpp>

namespace libkafka_asio
{

inline ConnectionConfiguration::ConnectionConfiguration() :
  message_max_bytes(constants::kDefaultMessageMaxBytes),
  socket_timeout(constants::kDefaultSocketTimeout),
  client_id(constants::DefaultConnectionId()),
  auto_connect(constants::DefaultConnectionAutoConnect())
{
}

inline void ConnectionConfiguration::AddBrokerFromString(const std::string& str)
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

template< typename T >
inline void ConnectionConfiguration::AddBroker(const T& broker)
{
  AddBroker(broker.host, broker.port);
}

template<typename Tx, typename Ty>
inline void ConnectionConfiguration::AddBroker(const Tx& hostname,
                                           const Ty& service)
{
  BrokerAddress broker_address;
  broker_address.hostname = boost::lexical_cast<String>(hostname);
  broker_address.service = boost::lexical_cast<String>(service);
  AddBroker(broker_address);
}

inline void ConnectionConfiguration::AddBroker(
  const ConnectionConfiguration::BrokerAddress& broker_address)
{
  broker_list.push_back(broker_address);
}

}  // namespace libkafka_asio

#endif  // CONNECTION_CONFIGURATION_H_0435D34D_97AC_4D67_8E3A_DEBAAC218C4D
