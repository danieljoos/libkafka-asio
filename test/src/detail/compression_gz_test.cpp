//
// detail/compression_gz_test.cpp
// ------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

using libkafka_asio::Byte;
using libkafka_asio::Bytes;
using libkafka_asio::String;
using libkafka_asio::detail::Compress;
using libkafka_asio::detail::Decompress;

class CompressionGzTest : public ::testing::Test
{
protected:
  Bytes TestCase1Compressed()
  {
    const unsigned char data[32] =
    {
      0x1f, 0x8b, 0x08, 0x08, 0x75, 0xad, 0x3b, 0x55,
      0x00, 0x03, 0x74, 0x65, 0x73, 0x74, 0x00, 0x4b,
      0xcb, 0xcf, 0x4f, 0x4a, 0x2c, 0xe2, 0x02, 0x00,
      0x47, 0x97, 0x2c, 0xb2, 0x07, 0x00, 0x00, 0x00
    };
    return Bytes(new Bytes::element_type(data, data + sizeof(data)));
  }

  Bytes TestCase1Uncompressed()
  {
    static const String data = "foobar\n";
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

  void SkipGzipHeader(Bytes& data)
  {
    ::z_stream gz = {};
    ::gz_header header = {};
    int ret = ::inflateInit2(&gz, 15 + 32);
    gz.next_in = reinterpret_cast< ::Bytef *>(&(*data)[0]);
    gz.avail_in = (::uInt) data->size();
    ret = ::inflateGetHeader(&gz, &header);
    while (ret == Z_OK && !header.done)
    {
      Byte buf;
      gz.next_out = reinterpret_cast< ::Bytef *>(&buf);
      gz.avail_out = 1;
      ret = ::inflate(&gz, Z_BLOCK);
    }
    ::inflateEnd(&gz);
    size_t skip = gz.total_in;
    data->erase(data->begin(), data->begin() + skip);
  }
};

TEST_F(CompressionGzTest, SimpleDecompress)
{
  boost::system::error_code ec;
  using namespace libkafka_asio::constants;
  Bytes result = Decompress(TestCase1Compressed(), kCompressionGZIP, ec);
  Bytes expected_result = TestCase1Uncompressed();
  ASSERT_EQ(libkafka_asio::kErrorSuccess, ec);
  AssertBytesEq(expected_result, result);
}

TEST_F(CompressionGzTest, EmptyDecompress)
{
  {
    Bytes test_data;
    boost::system::error_code ec;
    using namespace libkafka_asio::constants;
    Bytes result = Decompress(test_data, kCompressionGZIP, ec);
    ASSERT_EQ(libkafka_asio::kErrorCompressionFailed, ec);
    ASSERT_TRUE(!result);
  }
  {
    Bytes test_data(new Bytes::element_type());
    boost::system::error_code ec;
    ASSERT_EQ(0, test_data->size());
    using namespace libkafka_asio::constants;
    Bytes result = Decompress(test_data, kCompressionGZIP, ec);
    ASSERT_EQ(libkafka_asio::kErrorCompressionFailed, ec);
    ASSERT_TRUE(!result);
  }
}

TEST_F(CompressionGzTest, SimpleCompress)
{
  boost::system::error_code ec;
  using namespace libkafka_asio::constants;
  Bytes result = Compress(TestCase1Uncompressed(), kCompressionGZIP, ec);
  Bytes expected_result = TestCase1Compressed();
  ASSERT_EQ(libkafka_asio::kErrorSuccess, ec);
  SkipGzipHeader(result);
  SkipGzipHeader(expected_result);
  AssertBytesEq(expected_result, result);
}
