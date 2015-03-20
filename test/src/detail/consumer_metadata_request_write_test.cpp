//
// detail/consumer_metadata_request_write_test.cpp
// -----------------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

#include "StreamTest.h"

using libkafka_asio::ConsumerMetadataRequest;

class ConsumerMetadataRequestWriteTest :
  public ::testing::Test,
  public StreamTest
{
protected:
  void SetUp()
  {
    ResetStream();
  }
};

TEST_F(ConsumerMetadataRequestWriteTest, WriteRequestMessage)
{
  ConsumerMetadataRequest request;
  request.set_consumer_group("TestConsumerGroup");
  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  ASSERT_STREQ("TestConsumerGroup", ReadString(*stream).c_str());

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}

TEST_F(ConsumerMetadataRequestWriteTest, WriteRequestMessage_Empty)
{
  ConsumerMetadataRequest request;
  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  ASSERT_STREQ("", ReadString(*stream).c_str());

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}
