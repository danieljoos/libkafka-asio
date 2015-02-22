//
// error_test.cpp
// --------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

using namespace libkafka_asio;

TEST(ErrorTest, ClientErrorCategory)
{
  using boost::system::error_code;
  error_code error = error_code(kErrorAlreadyConnected);
  ASSERT_STREQ("libkafka_asio::ClientError", error.category().name());
  error = error_code(kErrorNotConnected);
  ASSERT_STREQ("libkafka_asio::ClientError", error.category().name());
  error = error_code(kErrorInProgress);
  ASSERT_STREQ("libkafka_asio::ClientError", error.category().name());
  error = error_code(kErrorNoBroker);
  ASSERT_STREQ("libkafka_asio::ClientError", error.category().name());
  // Boost system should be able to generate an error message:
  ASSERT_STREQ("No broker found", boost::system::system_error(error).what());
}

TEST(ErrorTest, KafkaErrorCategory)
{
  using boost::system::error_code;
  // 'Testing' two errors should suffice here
  error_code error = error_code(kErrorNoError);
  ASSERT_STREQ("libkafka_asio::KafkaError", error.category().name());
  error = error_code(kErrorMessageSizeTooLarge);
  ASSERT_STREQ("libkafka_asio::KafkaError", error.category().name());
  // Boost system should be able to generate an error message:
  ASSERT_STREQ("Message was too large",
               boost::system::system_error(error).what());
}