//
// detail/request_write_test.cpp
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

class RequestWriteTest :
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

  Bytes ReadEverything()
  {
    Bytes result(new Bytes::element_type());
    while (stream->good())
    {
      char c = 0;
      stream->get(c);
      if (stream->good())
      {
        result->push_back(c);
      }
    }
    return result;
  }

  typedef boost::shared_ptr<boost::asio::streambuf> StreamBufType;
  typedef boost::shared_ptr<std::iostream> StreamType;
  StreamBufType streambuf;
  StreamType stream;
};

TEST_F(RequestWriteTest, StringWireSize)
{
  String test_string1 = "Hello World";
  ASSERT_EQ(2 + 11, StringWireSize(test_string1));
  String test_string2 = "";
  ASSERT_EQ(2 + 0, StringWireSize(test_string2));
}

TEST_F(RequestWriteTest, BytesWireSize)
{
  Bytes test_bytes1;
  ASSERT_EQ(4 + 0, BytesWireSize(test_bytes1));
  Bytes test_bytes2(new Bytes::element_type());
  ASSERT_EQ(4 + 0, BytesWireSize(test_bytes2));
  Bytes test_bytes3(new Bytes::element_type(1024, '\1'));
  ASSERT_EQ(4 + 1024, BytesWireSize(test_bytes3));
}

TEST_F(RequestWriteTest, MessageWireSize)
{
  Message test_message;
  test_message.mutable_key().reset(new Bytes::element_type(1024));
  test_message.mutable_value().reset(new Bytes::element_type(2048));
  size_t expected_size =
    4 +  // Crc
      1 +  // Magic Byte
      1 +  // Attributes
      (4 + 1024) + // Key
      (4 + 2048);  // Value
  ASSERT_EQ(expected_size, MessageWireSize(test_message));
}

TEST_F(RequestWriteTest, MessageSetWireSize)
{
  MessageAndOffset test_message;
  test_message.mutable_key().reset(new Bytes::element_type(1024));
  test_message.mutable_value().reset(new Bytes::element_type(2048));
  MessageSet test_message_set;
  test_message_set.push_back(test_message);
  test_message_set.push_back(test_message);
  test_message_set.push_back(test_message);
  // See test above
  size_t expected_message_size = MessageWireSize(test_message);
  // 3x (offset + msg_size + message)
  size_t expected_size = 3 * (8 + 4 + expected_message_size);
  ASSERT_EQ(expected_size, MessageSetWireSize(test_message_set));
}

TEST_F(RequestWriteTest, RequestMessageWireSize)
{
  MessageAndOffset test_message;
  test_message.mutable_key().reset(new Bytes::element_type(1024));
  test_message.mutable_value().reset(new Bytes::element_type(2048));
  // See test above
  size_t expected_message_size = MessageWireSize(test_message);

  { // Metadata Request
    MetadataRequest req;
    req.AddTopicName("foo");
    req.AddTopicName("bar!");
    size_t expected_size =
      4 +        // Array size
        (2 + 3) +  // 'foo'
        (2 + 4);   // 'bar'
    ASSERT_EQ(expected_size, RequestMessageWireSize(req));
  }
  { // Produce Request
    ProduceRequest req;
    req.AddMessage(test_message, "bar!");
    size_t expected_size =
      2 +  // RequiredAcks
        4 +  // Timeout
        4 +  // Topic Array Size
        (2 + 4) +  // 'bar!'
        4 +  // Partition Array Size
        4 +  // Partition
        4 +  // MessageSetSize
        (8 + 4 + expected_message_size);  // MessageSet
    ASSERT_EQ(expected_size, RequestMessageWireSize(req));
  }
  { // Fetch Request
    FetchRequest req;
    req.FetchTopic("foo", 0);
    size_t expected_size =
      4 +  // ReplicaId
        4 +  // MaxWaitTime
        4 +  // MinBytes
        4 +  // Topic Array Size
        (2 + 3) +  // 'foo'
        4 +  // Partition Array Size
        4 +  // Partition
        8 +  // Fetch Offset
        4;   // MaxBytes
    ASSERT_EQ(expected_size, RequestMessageWireSize(req));
  }
  { // Offset Request
    OffsetRequest req;
    req.FetchTopicOffset("foo", 0, 2500);
    size_t expected_size =
      4 +  // ReplicaId
        4 +  // Topic Array Size
        (2 + 3) +  // 'foo'
        4 +  // Partition Array Size
        4 +  // Partition
        8 +  // Time
        4;   // MaxNumberOfOffsets
    ASSERT_EQ(expected_size, RequestMessageWireSize(req));
  }
}

TEST_F(RequestWriteTest, WriteInt8)
{
  {
    ResetStream();
    WriteInt8(0, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(1, bytes->size());
    ASSERT_EQ(0x00, bytes->front());
  }
  {  // Big-endian
    ResetStream();
    WriteInt8(-1, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(1, bytes->size());
    ASSERT_EQ(0xff, bytes->front());
  }
}

TEST_F(RequestWriteTest, WriteInt16)
{
  {
    ResetStream();
    WriteInt16(0, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(2, bytes->size());
    ASSERT_EQ(0x00, bytes->at(0));
    ASSERT_EQ(0x00, bytes->at(1));
  }
  {
    ResetStream();
    WriteInt16(-1, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(2, bytes->size());
    ASSERT_EQ(0xff, bytes->at(0));
    ASSERT_EQ(0xff, bytes->at(1));
  }
  {  // Big-endian
    ResetStream();
    WriteInt16(256, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(2, bytes->size());
    ASSERT_EQ(0x01, bytes->at(0));
    ASSERT_EQ(0x00, bytes->at(1));
  }
}

TEST_F(RequestWriteTest, WriteInt32)
{
  {
    ResetStream();
    WriteInt32(0, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(4, bytes->size());
    ASSERT_EQ(0x00, bytes->at(0));
    ASSERT_EQ(0x00, bytes->at(1));
    ASSERT_EQ(0x00, bytes->at(2));
    ASSERT_EQ(0x00, bytes->at(3));
  }
  {
    ResetStream();
    WriteInt32(-1, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(4, bytes->size());
    ASSERT_EQ(0xff, bytes->at(0));
    ASSERT_EQ(0xff, bytes->at(1));
    ASSERT_EQ(0xff, bytes->at(2));
    ASSERT_EQ(0xff, bytes->at(3));
  }
  {  // Big-endian
    ResetStream();
    WriteInt32(65536, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(4, bytes->size());
    ASSERT_EQ(0x00, bytes->at(0));
    ASSERT_EQ(0x01, bytes->at(1));
    ASSERT_EQ(0x00, bytes->at(2));
    ASSERT_EQ(0x00, bytes->at(3));
  }
}

TEST_F(RequestWriteTest, WriteInt64)
{
  {
    ResetStream();
    WriteInt64(0, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(8, bytes->size());
    ASSERT_EQ(0x00, bytes->at(0));
    ASSERT_EQ(0x00, bytes->at(1));
    ASSERT_EQ(0x00, bytes->at(2));
    ASSERT_EQ(0x00, bytes->at(3));
    ASSERT_EQ(0x00, bytes->at(4));
    ASSERT_EQ(0x00, bytes->at(5));
    ASSERT_EQ(0x00, bytes->at(6));
    ASSERT_EQ(0x00, bytes->at(7));
  }
  {
    ResetStream();
    WriteInt64(-1, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(8, bytes->size());
    ASSERT_EQ(0xff, bytes->at(0));
    ASSERT_EQ(0xff, bytes->at(1));
    ASSERT_EQ(0xff, bytes->at(2));
    ASSERT_EQ(0xff, bytes->at(3));
    ASSERT_EQ(0xff, bytes->at(4));
    ASSERT_EQ(0xff, bytes->at(5));
    ASSERT_EQ(0xff, bytes->at(6));
    ASSERT_EQ(0xff, bytes->at(7));
  }
  {  // Big-endian
    ResetStream();
    WriteInt64(4294967296, *stream);
    Bytes bytes = ReadEverything();
    ASSERT_TRUE(bytes);
    ASSERT_EQ(8, bytes->size());
    ASSERT_EQ(0x00, bytes->at(0));
    ASSERT_EQ(0x00, bytes->at(1));
    ASSERT_EQ(0x00, bytes->at(2));
    ASSERT_EQ(0x01, bytes->at(3));
    ASSERT_EQ(0x00, bytes->at(4));
    ASSERT_EQ(0x00, bytes->at(5));
    ASSERT_EQ(0x00, bytes->at(6));
    ASSERT_EQ(0x00, bytes->at(7));
  }
}

TEST_F(RequestWriteTest, WriteString)
{
  String test_string = "foo";
  WriteString(test_string, *stream);
  Bytes bytes = ReadEverything();
  ASSERT_TRUE(bytes);
  ASSERT_EQ(2 + 3, bytes->size());
  ASSERT_EQ(0, bytes->at(0));
  ASSERT_EQ(3, bytes->at(1));
  ASSERT_EQ('f', bytes->at(2));
  ASSERT_EQ('o', bytes->at(3));
  ASSERT_EQ('o', bytes->at(4));
}

TEST_F(RequestWriteTest, WriteBytes)
{
  Bytes test_bytes(new Bytes::element_type(3, 0xfe));
  WriteBytes(test_bytes, *stream);
  Bytes bytes = ReadEverything();
  ASSERT_TRUE(bytes);
  ASSERT_EQ(4 + 3, bytes->size());
  ASSERT_EQ(0, bytes->at(0));
  ASSERT_EQ(0, bytes->at(1));
  ASSERT_EQ(0, bytes->at(2));
  ASSERT_EQ(3, bytes->at(3));
  ASSERT_EQ(0xfe, bytes->at(4));
  ASSERT_EQ(0xfe, bytes->at(5));
  ASSERT_EQ(0xfe, bytes->at(6));
}
