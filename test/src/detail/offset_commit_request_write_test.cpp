//
// detail/offset_commit_request_write_test.cpp
// -------------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

#include "StreamTest.h"

using libkafka_asio::OffsetCommitRequest;

class OffsetCommitRequestWriteTest :
  public ::testing::Test,
  public StreamTest
{
protected:
  void SetUp()
  {
    ResetStream();
  }
};

TEST_F(OffsetCommitRequestWriteTest, WriteRequestMessage)
{
  OffsetCommitRequest request;
  request.set_consumer_group("TestConsumerGroup");
  request.CommitOffset("Topic1", 4, 1234, 88888888, "my metadata");
  request.CommitOffset("Topic1", 5, 5678, 99999999);
  request.CommitOffset("Topic2", 0, 1234);

  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  using namespace libkafka_asio::constants;
  ASSERT_STREQ("TestConsumerGroup", ReadString(*stream).c_str());
  ASSERT_EQ(2, ReadInt32(*stream));  // Topic array size

  ASSERT_STREQ("Topic1", ReadString(*stream).c_str());  // TopicName
  ASSERT_EQ(2, ReadInt32(*stream));  // Partition array size
  ASSERT_EQ(4, ReadInt32(*stream));  // Partition
  ASSERT_EQ(1234, ReadInt64(*stream));  // Offset
  ASSERT_EQ(88888888, ReadInt64(*stream));  // Timestamp
  ASSERT_STREQ("my metadata", ReadString(*stream).c_str());  // Metadata
  ASSERT_EQ(5, ReadInt32(*stream));  // Partition
  ASSERT_EQ(5678, ReadInt64(*stream));  // Offset
  ASSERT_EQ(99999999, ReadInt64(*stream));  // Timestamp
  ASSERT_STREQ("", ReadString(*stream).c_str());  // Metadata

  ASSERT_STREQ("Topic2", ReadString(*stream).c_str());  // TopicName
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition array size
  ASSERT_EQ(0, ReadInt32(*stream));  // Partition
  ASSERT_EQ(1234, ReadInt64(*stream));  // Offset
  ASSERT_EQ(kDefaultOffsetCommitTimestampNow, ReadInt64(*stream));  // Timestamp
  ASSERT_STREQ("", ReadString(*stream).c_str());  // Metadata

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}

TEST_F(OffsetCommitRequestWriteTest, WriteRequestMessage_Empty)
{
  OffsetCommitRequest request;
  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  ASSERT_STREQ("", ReadString(*stream).c_str());  // ConsumerGroup
  ASSERT_EQ(0, ReadInt32(*stream));  // Topic array size

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}
