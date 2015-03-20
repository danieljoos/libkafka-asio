//
// detail/offset_fetch_request_write_test.cpp
// ------------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

#include "StreamTest.h"

using libkafka_asio::OffsetFetchRequest;

class OffsetFetchRequestWriteTest :
  public ::testing::Test,
  public StreamTest
{
protected:
  void SetUp()
  {
    ResetStream();
  }
};

TEST_F(OffsetFetchRequestWriteTest, WriteRequestMessage)
{
  OffsetFetchRequest request;
  request.set_consumer_group("TestGroup");
  request.FetchOffset("Topic1", 0);
  request.FetchOffset("Topic1", 1);
  request.FetchOffset("Topic2", 1);
  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  ASSERT_STREQ("TestGroup", ReadString(*stream).c_str());  // ConsumerGroup
  ASSERT_EQ(2, ReadInt32(*stream));  // Topic array size
  ASSERT_STREQ("Topic1", ReadString(*stream).c_str());  // TopicName
  ASSERT_EQ(2, ReadInt32(*stream));  // Partition array size
  ASSERT_EQ(0, ReadInt32(*stream));  // Partition
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition
  ASSERT_STREQ("Topic2", ReadString(*stream).c_str());  // TopicName
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition array size
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}

TEST_F(OffsetFetchRequestWriteTest, WriteRequestMessage_Empty)
{
  OffsetFetchRequest request;
  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  ASSERT_STREQ("", ReadString(*stream).c_str());  // ConsumerGroup
  ASSERT_EQ(0, ReadInt32(*stream));  // Topic array size

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}
