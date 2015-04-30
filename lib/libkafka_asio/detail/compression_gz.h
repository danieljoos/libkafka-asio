//
// detail/compression_gz.h
// -----------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef COMPRESSION_GZ_H_0B410507_A88B_468B_A1EC_ABC8B9246F62
#define COMPRESSION_GZ_H_0B410507_A88B_468B_A1EC_ABC8B9246F62

#if !defined(LIBKAFKAASIO_NO_COMPRESSION) \
 && !defined(LIBKAFKAASIO_NO_COMPRESSION_GZIP)

#include <libkafka_asio/error.h>
#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{
namespace detail
{

// Compression/Decompression using the GZIP implementation of `zlib`.
struct GZIPCompressionAlgorithm
{
  static Bytes Compress(const Bytes& data, boost::system::error_code& ec);

  static Bytes Decompress(const Bytes& data, boost::system::error_code& ec);
};

// Register the GZIP Algorithm
template<>
struct CompressionPolicy<constants::kCompressionGZIP>
{
  typedef GZIPCompressionAlgorithm Algorithm;
};

}  // namespace detail
}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/compression_gz.h>

#endif  // GZIP compression not disabled?
#endif  // COMPRESSION_GZ_H_0B410507_A88B_468B_A1EC_ABC8B9246F62
