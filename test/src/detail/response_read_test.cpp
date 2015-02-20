//
// detail/response_read_test.cpp
// -----------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <iostream>
#include <gtest/gtest.h>
#include <boost/asio/streambuf.hpp>
#include <boost/shared_ptr.hpp>
#include <libkafka_asio/libkafka_asio.h>

using namespace libkafka_asio;
using namespace libkafka_asio::detail;

class ResponseReadTest :
  public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    ResetStream();
  }

  void ResetStream()
  {
    streambuf.reset(new boost::asio::streambuf());
    stream.reset(new std::iostream(streambuf.get()));
  }

  typedef boost::shared_ptr<boost::asio::streambuf> StreamBufType;
  typedef boost::shared_ptr<std::iostream> StreamType;
  StreamBufType streambuf;
  StreamType stream;
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
  ASSERT_TRUE(read_bytes);
  ASSERT_EQ(2, read_bytes->size());
  ASSERT_EQ(0x01, read_bytes->at(0));
  ASSERT_EQ(0x02, read_bytes->at(1));
}
