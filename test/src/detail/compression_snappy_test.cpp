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
using libkafka_asio::detail::Decompress;

TEST(CompressionSnappyTest, SimpleDecompress)
{
  const String expected_result = "HelloWorld";
  String compressed;
  ::snappy::Compress(expected_result.c_str(), expected_result.size(),
                     &compressed);
  Bytes test_data(new Bytes::element_type(compressed.begin(),
                                          compressed.end()));
  ASSERT_FALSE(test_data->empty());
  boost::system::error_code ec;
  using namespace libkafka_asio::constants;
  Bytes result = Decompress(test_data, kCompressionSnappy, ec);
  ASSERT_EQ(libkafka_asio::kErrorSuccess, ec);
  ASSERT_EQ(expected_result.size(), result->size());
  ASSERT_STREQ(expected_result.c_str(),
               String((const char *) &(*result)[0], result->size()).c_str());
}

TEST(CompressionSnappyTest, StreamDecompress)
{
  // Compress this little test string using two chunks
  const String expected_result = "Hello World";
  using boost::asio::streambuf;
  using boost::asio::buffer_cast;
  using boost::asio::buffer_size;
  using libkafka_asio::detail::WriteInt32;
  using libkafka_asio::Int32;
  String compressed1, compressed2;
  ::snappy::Compress(expected_result.c_str(), 5,
                     &compressed1);
  ::snappy::Compress(expected_result.c_str() + 5, expected_result.size() - 5,
                     &compressed2);
  streambuf compressed_streambuf;
  std::iostream compressed_stream(&compressed_streambuf);
  Bytes magic_bytes =
    libkafka_asio::detail::SnappyCompressionAlgorithm::kSnappyStreamMagic();
  compressed_stream.write(reinterpret_cast<const char *>(&(*magic_bytes)[0]),
                          magic_bytes->size());
  WriteInt32(1, compressed_stream);
  WriteInt32(1, compressed_stream);
  WriteInt32(static_cast<Int32>(compressed1.size()), compressed_stream);
  compressed_stream.write(compressed1.c_str(), compressed1.size());
  WriteInt32(static_cast<Int32>(compressed2.size()), compressed_stream);
  compressed_stream.write(compressed2.c_str(), compressed2.size());
  streambuf::const_buffers_type compressed_data =
    compressed_streambuf.data();
  Bytes test_data(new Bytes::element_type(
    buffer_cast<const Bytes::element_type::value_type *>(compressed_data),
    buffer_cast<const Bytes::element_type::value_type *>(compressed_data) +
    buffer_size(compressed_data)
  ));

  ASSERT_FALSE(test_data->empty());
  boost::system::error_code ec;
  using namespace libkafka_asio::constants;
  Bytes result = Decompress(test_data, kCompressionSnappy, ec);
  ASSERT_EQ(libkafka_asio::kErrorSuccess, ec);
  ASSERT_EQ(expected_result.size(), result->size());
  ASSERT_STREQ(expected_result.c_str(),
               String((const char *) &(*result)[0], result->size()).c_str());
}

TEST(CompressionSnappyTest, EmptyDecompress)
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
