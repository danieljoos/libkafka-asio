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

#if !defined(LIBKAFKAASIO_NO_COMPRESSION) \
 && !defined(LIBKAFKAASIO_NO_COMPRESSION_GZIP)

#include <zlib.h>
#include <libkafka_asio/constants.h>

namespace libkafka_asio
{
namespace detail
{

template<>
inline Bytes
CompressionAlgorithm<constants::kCompressionGZIP>::Compress(
  const Bytes& data, boost::system::error_code& ec)
{
  ec = kErrorCompressionNotAvailable;
  return Bytes();
}

template<>
inline Bytes
CompressionAlgorithm<constants::kCompressionGZIP>::Decompress(
  const Bytes& data, boost::system::error_code& ec)
{
  ec = kErrorCompressionNotAvailable;
  return Bytes();
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // GZIP compression not disabled?
#endif  // COMPRESSION_GZ_H_4936268F_C651_4E32_A917_8AC05247B3DB
