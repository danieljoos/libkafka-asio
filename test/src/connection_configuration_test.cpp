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
    ASSERT_EQ(0, configuration.broker_list.size());
  }

  ConnectionConfiguration configuration;
};

TEST_F(ConnectionConfigurationTest, AddBrokerFromString_Empty)
{
  configuration.AddBrokerFromString("");
  ASSERT_EQ(0, configuration.broker_list.size());
}

TEST_F(ConnectionConfigurationTest, AddBrokerFromString)
{
  configuration.AddBrokerFromString("localhost:1234");
  ASSERT_EQ(1, configuration.broker_list.size());
  ASSERT_STREQ("localhost", configuration.broker_list[0].hostname.c_str());
  ASSERT_STREQ("1234", configuration.broker_list[0].service.c_str());
}

TEST_F(ConnectionConfigurationTest, AddBrokerFromString_NoService)
{
  configuration.AddBrokerFromString("localhost");
  ASSERT_EQ(1, configuration.broker_list.size());
  ASSERT_STREQ("localhost", configuration.broker_list[0].hostname.c_str());
  // Default Kafka Service:
  ASSERT_STREQ("9092", configuration.broker_list[0].service.c_str());
}

TEST_F(ConnectionConfigurationTest, AddBrokerFromString_Colon)
{
  configuration.AddBrokerFromString(":");
  ASSERT_EQ(0, configuration.broker_list.size());
}