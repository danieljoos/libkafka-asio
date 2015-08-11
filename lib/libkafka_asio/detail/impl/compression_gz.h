//
// detail/impl/compression_gz.h
// ----------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef COMPRESSION_GZ_H_4936268F_C651_4E32_A917_8AC05247B3DB
#define COMPRESSION_GZ_H_4936268F_C651_4E32_A917_8AC05247B3DB

#include <zlib.h>

#include <algorithm>

#include <libkafka_asio/constants.h>
#include <libkafka_asio/error.h>
#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{
namespace detail
{

inline Bytes GZIPCompressionAlgorithm::Compress(
  const Bytes& data, boost::system::error_code& ec)
{
  static const size_t kBufferSize = 32 * 1024;
  static const int kGZIPWindowBits = 15 + 16;

  if (!data || data->empty())
  {
    ec = kErrorCompressionFailed;
    return Bytes();
  }
  // Initialize a new zlib stream
  ::z_stream gz = {};
  ::gz_header header = {};
  if (::deflateInit2(&gz, Z_DEFAULT_COMPRESSION, Z_DEFLATED, kGZIPWindowBits,
                     8, Z_DEFAULT_STRATEGY) != Z_OK)
  {
    ec = kErrorCompressionFailed;
    return Bytes();
  }
  gz.next_in = reinterpret_cast< ::Bytef *>(&(*data)[0]);
  gz.avail_in = (::uInt) data->size();
  Bytes out(new Bytes::element_type());
  int ret = Z_OK;
  // Deflate
  while (ret == Z_OK)
  {
    size_t pos = out->size();
    out->resize(out->size() + kBufferSize);
    gz.next_out = reinterpret_cast< ::Bytef *>(&(*out)[pos]);
    gz.avail_out = kBufferSize;
    ret = ::deflate(&gz, Z_FINISH);
  }
  if (gz.total_out < out->size())
  {
    out->resize(gz.total_out);
  }
  ::deflateEnd(&gz);
  if (ret != Z_STREAM_END)
  {
    ec = kErrorCompressionFailed;
    return Bytes();
  }

  ec = kErrorSuccess;
  return out;
}

inline Bytes GZIPCompressionAlgorithm::Decompress(
  const Bytes& data, boost::system::error_code& ec)
{
  static const size_t kBufferSize = 32 * 1024;
  static const int kGZIPWindowBits = 15 + 32;

  if (!data || data->empty())
  {
    ec = kErrorCompressionFailed;
    return Bytes();
  }
  // Initialize a new zlib stream
  ::z_stream gz = {};
  ::gz_header header = {};
  if (::inflateInit2(&gz, kGZIPWindowBits) != Z_OK)
  {
    ec = kErrorCompressionFailed;
    return Bytes();
  }
  gz.next_in = reinterpret_cast< ::Bytef *>(&(*data)[0]);
  gz.avail_in = (::uInt) data->size();
  int ret = Z_OK;
  // Inflate gzip header
  ret = ::inflateGetHeader(&gz, &header);
  Bytes out(new Bytes::element_type());
  // Now, inflate the actual data
  while (ret == Z_OK)
  {
    size_t pos = out->size();
    out->resize(out->size() + kBufferSize);
    gz.next_out = reinterpret_cast< ::Bytef *>(&(*out)[pos]);
    gz.avail_out = kBufferSize;
    ret = ::inflate(&gz, 0);
  }
  if (gz.total_out < out->size())
  {
    out->resize(gz.total_out);
  }
  ::inflateEnd(&gz);
  if (ret != Z_STREAM_END)
  {
    ec = kErrorCompressionFailed;
    return Bytes();
  }

  ec = kErrorSuccess;
  return out;
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // COMPRESSION_GZ_H_4936268F_C651_4E32_A917_8AC05247B3DB
