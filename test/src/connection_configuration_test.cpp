//
// connection_configuration_test.cpp
// -----------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>
#include "../../lib/libkafka_asio/connection_configuration.h"

using namespace libkafka_asio;

class ConnectionConfigurationTest :
  public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    ASSERT_FALSE(configuration.broker_address);
  }

  ConnectionConfiguration configuration;
};

TEST_F(ConnectionConfigurationTest, SetBrokerFromString_Empty)
{
  configuration.SetBrokerFromString("");
  ASSERT_FALSE(configuration.broker_address);
}

TEST_F(ConnectionConfigurationTest, SetBrokerFromString)
{
  configuration.SetBrokerFromString("localhost:1234");
  ASSERT_STREQ("localhost", configuration.broker_address->hostname.c_str());
  ASSERT_STREQ("1234", configuration.broker_address->service.c_str());
}

TEST_F(ConnectionConfigurationTest, SetBrokerFromString_NoService)
{
  configuration.SetBrokerFromString("localhost");
  ASSERT_STREQ("localhost", configuration.broker_address->hostname.c_str());
  // Default Kafka Service:
  ASSERT_STREQ("9092", configuration.broker_address->service.c_str());
}

TEST_F(ConnectionConfigurationTest, SetBrokerFromString_Colon)
{
  configuration.SetBrokerFromString(":");
  ASSERT_FALSE(configuration.broker_address);
}