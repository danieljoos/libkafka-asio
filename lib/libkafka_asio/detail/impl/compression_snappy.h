//
// detail/impl/compression_snappy.h
// --------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef COMPRESSION_SNAPPY_H_071A13C4_C7A3_4B00_847B_0B61A9A761E7
#define COMPRESSION_SNAPPY_H_071A13C4_C7A3_4B00_847B_0B61A9A761E7

#include <algorithm>
#include <iostream>

#include <snappy.h>
#include <libkafka_asio/error.h>
#include <libkafka_asio/detail/bytes_streambuf.h>
#include <libkafka_asio/detail/endian.h>
#include <libkafka_asio/detail/response_read.h>

namespace libkafka_asio
{
namespace detail
{

// Snappy Stream Magic Bytes.
// The Xerial SnappyOutputStream will start with writing these 8 bytes.
inline const Bytes& SnappyCompressionAlgorithm::kSnappyStreamMagic()
{
  static const Byte kMagic[] = {130, 'S', 'N', 'A', 'P', 'P', 'Y', 0};
  static const Bytes kMagicBytes(
    new Bytes::element_type(kMagic, kMagic + sizeof(kMagic)));
  return kMagicBytes;
}

inline Bytes SnappyCompressionAlgorithm::Compress(
  const Bytes& data, boost::system::error_code& ec)
{
  if (!data || data->empty())
  {
    ec = kErrorCompressionFailed;
    return Bytes();
  }
  Bytes result(
    new Bytes::element_type(::snappy::MaxCompressedLength(data->size())));
  const char *raw_input_ptr = reinterpret_cast<const char *>(&(*data)[0]);
  char *raw_output_ptr = reinterpret_cast<char *>(&(*result)[0]);
  size_t result_size = 0;
  ::snappy::RawCompress(raw_input_ptr, data->size(), raw_output_ptr,
                        &result_size);
  result->resize(result_size);
  ec = kErrorSuccess;
  return result;
}

inline Bytes SnappyCompressionAlgorithm::Decompress(
  const Bytes& data, boost::system::error_code& ec)
{
  if (!data || data->empty())
  {
    ec = kErrorCompressionFailed;
    return Bytes();
  }
  bool is_stream = std::equal(kSnappyStreamMagic()->begin(),
                              kSnappyStreamMagic()->end(),
                              data->begin());
  if (is_stream)
  {
    return DecompressStream(data, ec);
  }
  else
  {
    return DecompressChunk(data, ec);
  }
}

inline Bytes SnappyCompressionAlgorithm::DecompressChunk(
  const Bytes& data, boost::system::error_code& ec)
{
  const char *raw_input_ptr = reinterpret_cast<const char *>(&(*data)[0]);
  size_t uncompressed_size = 0;
  if (!::snappy::IsValidCompressedBuffer(raw_input_ptr, data->size()) ||
      !::snappy::GetUncompressedLength(raw_input_ptr, data->size(),
                                       &uncompressed_size))
  {
    ec = kErrorCompressionFailed;
    return Bytes();
  }
  Bytes result(new Bytes::element_type(uncompressed_size));
  char *raw_output_ptr = reinterpret_cast<char *>(&(*result)[0]);
  if (!::snappy::RawUncompress(raw_input_ptr, data->size(), raw_output_ptr))
  {
    ec = kErrorCompressionFailed;
    return Bytes();
  }
  ec = kErrorSuccess;
  return result;
}

inline Bytes SnappyCompressionAlgorithm::DecompressStream(
  const Bytes& data, boost::system::error_code& ec)
{
  // Create intput and output streams
  using boost::asio::streambuf;
  BytesStreambuf input_buffer(data);
  streambuf output_buffer;
  std::istream is(&input_buffer);
  std::ostream os(&output_buffer);
  // The stream header can be ignored
  is.ignore(kSnappyStreamMagic()->size());
  is.ignore(sizeof(Int32));
  is.ignore(sizeof(Int32));
  // Start reading chunks from the input stream, decompress them and write the
  // data to the output stream.
  while (is.good())
  {
    Bytes chunk;
    ReadBytes(is, chunk);
    if (!chunk)
    {
      ec = kErrorCompressionFailed;
      return Bytes();
    }
    chunk = DecompressChunk(chunk, ec);
    if (ec)
    {
      return Bytes();
    }
    os.write(reinterpret_cast<const char *>(&(*chunk)[0]), chunk->size());
    is.peek();
  }
  // Create the result object and copy all data into it
  using boost::asio::buffer_cast;
  using boost::asio::buffer_size;
  streambuf::const_buffers_type output_buffer_data = output_buffer.data();
  Bytes result(new Bytes::element_type(
    buffer_cast<const Bytes::element_type::value_type *>(output_buffer_data),
    buffer_cast<const Bytes::element_type::value_type *>(output_buffer_data) +
    buffer_size(output_buffer_data)
  ));
  ec = kErrorSuccess;
  return result;
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // COMPRESSION_SNAPPY_H_071A13C4_C7A3_4B00_847B_0B61A9A761E7
