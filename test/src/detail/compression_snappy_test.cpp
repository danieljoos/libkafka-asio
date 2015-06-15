//
// detail/compression_snappy_test.cpp
// ----------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <snappy.h>
#include <libkafka_asio/libkafka_asio.h>

using libkafka_asio::Byte;
using libkafka_asio::Bytes;
using libkafka_asio::String;
using libkafka_asio::detail::Compress;
using libkafka_asio::detail::Decompress;

class CompressionSnappyTest : public ::testing::Test
{
protected:
  Bytes TestCase1Compressed()
  {
    static const unsigned char data[11] =
    {
      0x24, 0x14, 0x52, 0x45, 0x50, 0x45, 0x41, 0x54,
      0x76, 0x06, 0x00
    };
    return Bytes(new Bytes::element_type(data, data + sizeof(data)));
  }

  Bytes TestCase2Compressed()
  {
    const unsigned char data[39] =
    {
      0x82, 0x53, 0x4e, 0x41, 0x50, 0x50, 0x59, 0x00,
      0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
      0x00, 0x00, 0x00, 0x07, 0x05, 0x10, 0x48, 0x65,
      0x6c, 0x6c, 0x6f, 0x00, 0x00, 0x00, 0x08, 0x06,
      0x14, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64
    };
    return Bytes(new Bytes::element_type(data, data + sizeof(data)));
  }

  Bytes TestCase1Uncompressed()
  {
    static const String data = "REPEATREPEATREPEATREPEATREPEATREPEAT";
    return Bytes(new Bytes::element_type(data.begin(), data.end()));
  }

  Bytes TestCase2Uncompressed()
  {
    static const String data = "Hello World";
    return Bytes(new Bytes::element_type(data.begin(), data.end()));
  }

  void AssertBytesEq(const Bytes& expected, const Bytes& actual)
  {
    ASSERT_TRUE(expected);
    ASSERT_TRUE(actual);
    ASSERT_EQ(expected->size(), actual->size());
    for (size_t i = 0; i < actual->size(); ++i)
    {
      ASSERT_EQ(expected->at(i), actual->at(i));
    }
  }
};

TEST_F(CompressionSnappyTest, SimpleDecompress)
{
  boost::system::error_code ec;
  using namespace libkafka_asio::constants;
  Bytes result = Decompress(TestCase1Compressed(), kCompressionSnappy, ec);
  Bytes expected_result = TestCase1Uncompressed();
  ASSERT_EQ(libkafka_asio::kErrorSuccess, ec);
  AssertBytesEq(expected_result, result);
}

TEST_F(CompressionSnappyTest, StreamDecompress)
{
  boost::system::error_code ec;
  using namespace libkafka_asio::constants;
  Bytes result = Decompress(TestCase2Compressed(), kCompressionSnappy, ec);
  Bytes expected_result = TestCase2Uncompressed();
  ASSERT_EQ(libkafka_asio::kErrorSuccess, ec);
  AssertBytesEq(expected_result, result);
}

TEST_F(CompressionSnappyTest, EmptyDecompress)
{
  {
    Bytes test_data;
    boost::system::error_code ec;
    using namespace libkafka_asio::constants;
    Bytes result = Decompress(test_data, kCompressionSnappy, ec);
    ASSERT_EQ(libkafka_asio::kErrorCompressionFailed, ec);
    ASSERT_TRUE(!result);
  }
  {
    Bytes test_data(new Bytes::element_type());
    boost::system::error_code ec;
    using namespace libkafka_asio::constants;
    Bytes result = Decompress(test_data, kCompressionSnappy, ec);
    ASSERT_EQ(libkafka_asio::kErrorCompressionFailed, ec);
    ASSERT_TRUE(!result);
  }
}

TEST_F(CompressionSnappyTest, SimpleCompress)
{
  boost::system::error_code ec;
  using namespace libkafka_asio::constants;
  Bytes result = Compress(TestCase1Uncompressed(), kCompressionSnappy, ec);
  Bytes expected_result = TestCase1Compressed();
  ASSERT_EQ(libkafka_asio::kErrorSuccess, ec);
  AssertBytesEq(expected_result, result);
}
