//
// detail/metadata_request_write_test.cpp
// --------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

#include "StreamTest.h"

using libkafka_asio::MetadataRequest;

class MetadataRequestWriteTest :
  public ::testing::Test,
  public StreamTest
{
protected:
  void SetUp()
  {
    ResetStream();
  }
};

TEST_F(MetadataRequestWriteTest, WriteRequestMessage)
{
  MetadataRequest request;
  request.AddTopicName("Foo");
  request.AddTopicName("Bar");
  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  ASSERT_EQ(2, ReadInt32(*stream));  // Topic array size
  ASSERT_STREQ("Foo", ReadString(*stream).c_str());
  ASSERT_STREQ("Bar", ReadString(*stream).c_str());

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}

TEST_F(MetadataRequestWriteTest, WriteRequestMessage_Empty)
{
  MetadataRequest request;
  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  ASSERT_EQ(0, ReadInt32(*stream));  // Topic array size

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}
