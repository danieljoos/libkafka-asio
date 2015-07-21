//
// detail/compression.h
// --------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef COMPRESSION_H_D5F6D8BB_5E7B_4A32_8684_2F69BF8D7AD1
#define COMPRESSION_H_D5F6D8BB_5E7B_4A32_8684_2F69BF8D7AD1

#include <libkafka_asio/constants.h>
#include <libkafka_asio/error.h>
#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{
namespace detail
{

// Compress the given bytes with the specified compression algorithm.
// If the something goes wrong or the compression algorithm is not available,
// the error code will be set correspondingly.
Bytes Compress(const Bytes& data,
               constants::Compression compression,
               boost::system::error_code& ec);


// De-compress the given bytes using the specified compression algorithm.
// If there was an error or the compression algorithm is not available, the
// given error code will be set.
Bytes Decompress(const Bytes& data,
                 constants::Compression compression,
                 boost::system::error_code& ec);


// Fallback implementation, which will be used in case an algorithm was disabled
// and is therefore not available. It simply sets a respective error code and
// returns an empty value.
struct FallbackCompressionAlgorithm
{
  static Bytes Compress(const Bytes&, boost::system::error_code& ec);

  static Bytes Decompress(const Bytes&, boost::system::error_code& ec);
};


// Compression algorithm policy template.
// Specific algorithms should specialize this template and define the
// 'Algorithm' type.
template<constants::Compression C>
struct CompressionPolicy
{
  typedef FallbackCompressionAlgorithm Algorithm;
};

}  // namespace detail
}  // namespace libkafka_asio

#include <libkafka_asio/detail/compression_gz.h>
#include <libkafka_asio/detail/compression_snappy.h>
#include <libkafka_asio/detail/impl/compression.h>

#endif  // COMPRESSION_H_D5F6D8BB_5E7B_4A32_8684_2F69BF8D7AD1
