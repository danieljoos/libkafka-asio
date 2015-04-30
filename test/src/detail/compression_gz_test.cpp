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

TEST(CompressionGzTest, SimpleDecompress)
{
  const String expected_result = "foobar\n";
  const unsigned char compressed[32] =
    {
      0x1f, 0x8b, 0x08, 0x08, 0x75, 0xad, 0x3b, 0x55,
      0x00, 0x03, 0x74, 0x65, 0x73, 0x74, 0x00, 0x4b,
      0xcb, 0xcf, 0x4f, 0x4a, 0x2c, 0xe2, 0x02, 0x00,
      0x47, 0x97, 0x2c, 0xb2, 0x07, 0x00, 0x00, 0x00
    };
  Bytes test_data(new Bytes::element_type(compressed,
                                          compressed + sizeof(compressed)));

  boost::system::error_code ec;
  using namespace libkafka_asio::constants;
  Bytes result = Decompress(test_data, kCompressionGZIP, ec);

  ASSERT_EQ(libkafka_asio::kErrorSuccess, ec);
  ASSERT_EQ(expected_result.size(), result->size());
  ASSERT_STREQ(expected_result.c_str(), (const char *) &(*result)[0]);
}

TEST(CompressionGzTest, EmptyDecompress)
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
