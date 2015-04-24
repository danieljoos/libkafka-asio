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


// Compression algorithm template
template<constants::Compression C>
struct CompressionAlgorithm
{
  static Bytes Compress(const Bytes& data,
                        boost::system::error_code& ec);
  static Bytes Decompress(const Bytes& data,
                          boost::system::error_code& ec);
};


}  // namespace detail
}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/compression.h>

#endif  // COMPRESSION_H_D5F6D8BB_5E7B_4A32_8684_2F69BF8D7AD1
