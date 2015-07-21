//
// detail/compression_snappy.h
// ---------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef COMPRESSION_SNAPPY_H_1C60D9AB_C9C7_45B1_8458_A6C55A1AF72F
#define COMPRESSION_SNAPPY_H_1C60D9AB_C9C7_45B1_8458_A6C55A1AF72F

#if !defined(LIBKAFKAASIO_NO_COMPRESSION) \
 && !defined(LIBKAFKAASIO_NO_COMPRESSION_SNAPPY)

#include <libkafka_asio/constants.h>
#include <libkafka_asio/error.h>
#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{
namespace detail
{

// Kafka message compression and decompression using the Snappy compression
// algorithm. It also implements the decompression of Xerial SnappyOutputStream
// data.
struct SnappyCompressionAlgorithm
{
  // Xerial Snappy OutputStream Magic Bytes
  static const Bytes& kSnappyStreamMagic();

  // Compress the given data using Snappy
  static Bytes Compress(const Bytes& data, boost::system::error_code& ec);

  // Decompress the given data using Snappy
  static Bytes Decompress(const Bytes& data, boost::system::error_code& ec);

private:
  // Decompress the given chunk
  static Bytes DecompressChunk(const Bytes& data,
                               boost::system::error_code& ec);

  // Interprets the given data as Snappy stream (xerial OutputStream format)
  static Bytes DecompressStream(const Bytes& data,
                                boost::system::error_code& ec);
};

// Register the algorithm
template<>
struct CompressionPolicy<constants::kCompressionSnappy>
{
  typedef SnappyCompressionAlgorithm Algorithm;
};

}  // namespace detail
}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/compression_snappy.h>

#endif  // Snappy compression not disabled?
#endif  // COMPRESSION_SNAPPY_H_1C60D9AB_C9C7_45B1_8458_A6C55A1AF72F
