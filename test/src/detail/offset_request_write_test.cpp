//
// detail/offset_request_write_test.cpp
// ------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

#include "StreamTest.h"

using libkafka_asio::OffsetRequest;

class OffsetRequestWriteTest :
  public ::testing::Test,
  public StreamTest
{
protected:
  void SetUp()
  {
    ResetStream();
  }
};

TEST_F(OffsetRequestWriteTest, WriteRequestMessage)
{
  OffsetRequest request;
  request.FetchTopicOffset("Topic1", 1);
  request.FetchTopicOffset("Topic2", 3, -2, 5);
  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  using namespace libkafka_asio::constants;
  ASSERT_EQ(-1, ReadInt32(*stream));  // ReplicaId
  ASSERT_EQ(2, ReadInt32(*stream));  // Topic array size

  ASSERT_STREQ("Topic1", ReadString(*stream).c_str());  // TopicName
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition array size
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition
  ASSERT_EQ(kOffsetTimeLatest, ReadInt64(*stream));  // Time
  ASSERT_EQ(kDefaultOffsetMaxNumberOfOffsets, ReadInt32(*stream));

  ASSERT_STREQ("Topic2", ReadString(*stream).c_str());  // TopicName
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition array size
  ASSERT_EQ(3, ReadInt32(*stream));  // Partition
  ASSERT_EQ(-2, ReadInt64(*stream));  // Time
  ASSERT_EQ(5, ReadInt32(*stream));  // MaxNumberOfOffsets

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}

TEST_F(OffsetRequestWriteTest, WriteRequestMessage_Empty)
{
  OffsetRequest request;
  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  using namespace libkafka_asio::constants;
  ASSERT_EQ(-1, ReadInt32(*stream));  // ReplicaId
  ASSERT_EQ(0, ReadInt32(*stream));  // Topic array size

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}