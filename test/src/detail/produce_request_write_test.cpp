//
// detail/produce_request_write_test.cpp
// -------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

#include "StreamTest.h"

using libkafka_asio::ProduceRequest;
using libkafka_asio::Int32;

class ProduceRequestWriteTest :
  public ::testing::Test,
  public StreamTest
{
protected:
  void SetUp()
  {
    ResetStream();
  }
};

TEST_F(ProduceRequestWriteTest, WriteRequestMessage)
{
  ProduceRequest request;
  request.set_required_acks(1);
  request.set_timeout(100);
  request.AddValue("Foo Bar", "Topic1", 2);
  ASSERT_EQ(1, request.topics().size());
  ASSERT_EQ(1, request.topics()[0].partitions.size());

  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  ASSERT_EQ(1, ReadInt16(*stream));  // RequiredAcks
  ASSERT_EQ(100, ReadInt32(*stream));  // Timeout
  ASSERT_EQ(1, ReadInt32(*stream));  // Topic array size
  ASSERT_STREQ("Topic1", ReadString(*stream).c_str());  // TopicName
  ASSERT_EQ(1, ReadInt32(*stream));  // Partition array size
  ASSERT_EQ(2, ReadInt32(*stream));  // Partition

  // 'MessageSetWireSize' and 'ReadMessageSet' are tested somewhere else, so
  // let's just assume they work correctly.
  Int32 expected_message_set_size
    = MessageSetWireSize(request.topics()[0].partitions[0].messages);
  ASSERT_EQ(expected_message_set_size, ReadInt32(*stream));  // MessageSetSize
  libkafka_asio::MessageSet message_set;
  boost::system::error_code ec;
  ReadMessageSet(*stream, message_set, expected_message_set_size, ec);

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}

TEST_F(ProduceRequestWriteTest, WriteRequestMessage_Empty)
{
  ProduceRequest request;

  libkafka_asio::detail::WriteRequestMessage(request, *stream);

  using namespace libkafka_asio::detail;
  using namespace libkafka_asio::constants;
  ASSERT_EQ(kDefaultProduceRequiredAcks, ReadInt16(*stream));  // RequiredAcks
  ASSERT_EQ(kDefaultProduceTimeout, ReadInt32(*stream));  // Timeout
  ASSERT_EQ(0, ReadInt32(*stream));  // Topic array size

  // Nothing else ...
  ASSERT_EQ(0, streambuf->size());
}
