//
// detail/fetch_request_write_test.cpp
// ---------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

#include "StreamTest.h"

using libkafka_asio::FetchRequest;
using libkafka_asio::Bytes;
using libkafka_asio::Int32;

class FetchRequestWriteTest :
  public ::testing::Test,
  public StreamTest
{
protected:
  void SetUp()
  {
    ResetStream();
  }
};

TEST_F(FetchRequestWriteTest, WriteRequestMessage)
{
  FetchRequest request;
  request.set_max_wait_time(100);
  request.set_min_bytes(1);
  request.FetchTopic("Topic1", 0, 123);
  request.FetchTopic("Topic2", 1, 456, 1024);

  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  ASSERT_EQ(-1, ReadInt32(*stream));  // ReplicaId
  ASSERT_EQ(100, ReadInt32(*stream));  // MaxWaitTime
  ASSERT_EQ(1, ReadInt32(*stream));  // MinBytes
  ASSERT_EQ(2, ReadInt32(*stream));  // Topic array size

  ASSERT_STREQ("Topic1", ReadString(*stream).c_str());  // TopicName
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition array size
  ASSERT_EQ(0, ReadInt32(*stream));  // Partition 0
  ASSERT_EQ(123, ReadInt64(*stream));  // FetchOffset 123
  ASSERT_EQ(libkafka_asio::constants::kDefaultFetchMaxBytes,
            ReadInt32(*stream));  // MaxBytes (default)

  ASSERT_STREQ("Topic2", ReadString(*stream).c_str());  // TopicName
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition array size
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition 1
  ASSERT_EQ(456, ReadInt64(*stream));  // FetchOffset 456
  ASSERT_EQ(1024, ReadInt32(*stream));  // MaxBytes 1024

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}

TEST_F(FetchRequestWriteTest, WriteRequestMessage_Empty)
{
  FetchRequest request;

  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  ASSERT_EQ(-1, ReadInt32(*stream));  // ReplicaId
  ASSERT_EQ(0, ReadInt32(*stream));  // MaxWaitTime
  ASSERT_EQ(0, ReadInt32(*stream));  // MinBytes
  ASSERT_EQ(0, ReadInt32(*stream));  // Topic array size

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}
