//
// detail/response_read_test.cpp
// -----------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

#include "StreamTest.h"

using namespace libkafka_asio;
using namespace libkafka_asio::detail;

class ResponseReadTest :
  public ::testing::Test,
  public StreamTest
{
protected:
  virtual void SetUp()
  {
    ResetStream();
  }
};

TEST_F(ResponseReadTest, ReadInt8)
{
  Bytes bytes(new Bytes::element_type(1, 64));
  stream->write((const char *) &(*bytes)[0], bytes->size());
  ASSERT_EQ(64, ReadInt8(*stream));
  // Read again on exhausted stream:
  EXPECT_EQ(0, ReadInt8(*stream));
  EXPECT_FALSE(stream->good());
  EXPECT_EQ(0, ReadInt8(*stream));
  EXPECT_EQ(0, ReadInt8(*stream));
}

TEST_F(ResponseReadTest, ReadInt16)
{
  Bytes bytes(new Bytes::element_type());
  bytes->push_back(0x01);
  bytes->push_back(0x00);
  stream->write((const char *) &(*bytes)[0], bytes->size());
  ASSERT_EQ(256, ReadInt16(*stream));
}

TEST_F(ResponseReadTest, ReadInt32)
{
  Bytes bytes(new Bytes::element_type());
  bytes->push_back(0x00);
  bytes->push_back(0x01);
  bytes->push_back(0x00);
  bytes->push_back(0x00);
  stream->write((const char *) &(*bytes)[0], bytes->size());
  ASSERT_EQ(65536, ReadInt32(*stream));
}

TEST_F(ResponseReadTest, ReadInt64)
{
  Bytes bytes(new Bytes::element_type());
  bytes->push_back(0x00);
  bytes->push_back(0x00);
  bytes->push_back(0x00);
  bytes->push_back(0x01);
  bytes->push_back(0x00);
  bytes->push_back(0x00);
  bytes->push_back(0x00);
  bytes->push_back(0x00);
  stream->write((const char *) &(*bytes)[0], bytes->size());
  ASSERT_EQ(4294967296, ReadInt64(*stream));
}

TEST_F(ResponseReadTest, ReadString)
{
  Bytes bytes(new Bytes::element_type());
  bytes->push_back(0);
  bytes->push_back(3);
  bytes->push_back('f');
  bytes->push_back('o');
  bytes->push_back('o');
  stream->write((const char *) &(*bytes)[0], bytes->size());
  ASSERT_STREQ("foo", ReadString(*stream).c_str());
}

TEST_F(ResponseReadTest, ReadBytes)
{
  Bytes bytes(new Bytes::element_type());
  bytes->push_back(0);
  bytes->push_back(0);
  bytes->push_back(0);
  bytes->push_back(2);
  bytes->push_back(0x01);
  bytes->push_back(0x02);
  stream->write((const char *) &(*bytes)[0], bytes->size());
  Bytes read_bytes;
  ReadBytes(*stream, read_bytes);
  ASSERT_TRUE(static_cast<bool>(read_bytes));
  ASSERT_EQ(2, read_bytes->size());
  ASSERT_EQ(0x01, read_bytes->at(0));
  ASSERT_EQ(0x02, read_bytes->at(1));
}

TEST_F(ResponseReadTest, ReadMessage)
{
  // Extracted this from `Wireshark`, capturing a `kafka-console-producer`
  // request with message value `helloworld`.
  const unsigned char test_data[24] =
  {
      0x6f, 0xff, 0xbb, 0x60, 0x00, 0x00, 0xff, 0xff,
      0xff, 0xff, 0x00, 0x00, 0x00, 0x0a, 0x68, 0x65,
      0x6c, 0x6c, 0x6f, 0x77, 0x6f, 0x72, 0x6c, 0x64
  };
  stream->write((const char*)test_data, sizeof(test_data));
  Message message;
  boost::system::error_code ec;
  ReadMessage(*stream, message, ec);
  ASSERT_EQ(kErrorSuccess, ec);
  ASSERT_FALSE(static_cast<bool>(message.key()));
  ASSERT_TRUE(static_cast<bool>(message.value()));
  std::string message_value((const char*)&(*message.value())[0],
                            message.value()->size());
  ASSERT_STREQ("helloworld", message_value.c_str());
}

TEST_F(ResponseReadTest, ReadMessageSet)
{
  // MessageSet that contains 2 messages: `foo` and `bar`
  const unsigned char test_data[58] =
  {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x11, 0xfa, 0x8b, 0x1b, 0x4c,
      0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
      0x00, 0x03, 0x66, 0x6f, 0x6f, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x11, 0x00, 0x07, 0xf2, 0xc7, 0x00, 0x00, 0xff,
      0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 0x62,
      0x61, 0x72
  };
  stream->write((const char*)test_data, sizeof(test_data));
  MessageSet message_set;
  boost::system::error_code ec;
  ReadMessageSet(*stream, message_set, sizeof(test_data), ec);
  ASSERT_EQ(kErrorSuccess, ec);
  ASSERT_EQ(2, message_set.size());
  ASSERT_TRUE(static_cast<bool>(message_set[0].value()));
  ASSERT_TRUE(static_cast<bool>(message_set[1].value()));
  std::string message_value_0((const char*) &(*message_set[0].value())[0],
                              message_set[0].value()->size());
  std::string message_value_1((const char*) &(*message_set[1].value())[0],
                              message_set[1].value()->size());
  ASSERT_STREQ("foo", message_value_0.c_str());
  ASSERT_STREQ("bar", message_value_1.c_str());
}
