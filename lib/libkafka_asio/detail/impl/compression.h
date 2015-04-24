//
// detail/impl/compression.h
// -------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef COMPRESSION_H_6B15B598_5C1D_447E_AF74_E541088998FA
#define COMPRESSION_H_6B15B598_5C1D_447E_AF74_E541088998FA

#include <libkafka_asio/constants.h>
#include <libkafka_asio/error.h>

namespace libkafka_asio
{
namespace detail
{

inline Bytes Compress(const Bytes& data,
                      constants::Compression compression,
                      boost::system::error_code& ec)
{
  typedef CompressionAlgorithm<constants::kCompressionGZIP> AlgorithmGZIP;
  typedef CompressionAlgorithm<constants::kCompressionSnappy> AlgorithmSnappy;
  typedef CompressionAlgorithm<constants::kCompressionLz4> AlgorithmLz4;

  switch (compression)
  {
    case constants::kCompressionGZIP:
      return AlgorithmGZIP::Compress(data, ec);
    case constants::kCompressionSnappy:
      return AlgorithmSnappy::Compress(data, ec);
    case constants::kCompressionLz4:
      return AlgorithmLz4::Compress(data, ec);
    case constants::kCompressionNone:
      ec = kErrorSuccess;
      break;
    default:
      ec = kErrorCompressionNotAvailable;
      break;
  }
  return Bytes();
}

inline Bytes Decompress(const Bytes& data,
                        constants::Compression compression,
                        boost::system::error_code& ec)
{
  typedef CompressionAlgorithm<constants::kCompressionGZIP> AlgorithmGZIP;
  typedef CompressionAlgorithm<constants::kCompressionSnappy> AlgorithmSnappy;
  typedef CompressionAlgorithm<constants::kCompressionLz4> AlgorithmLz4;

  switch (compression)
  {
    case constants::kCompressionGZIP:
      return AlgorithmGZIP::Decompress(data, ec);
    case constants::kCompressionSnappy:
      return AlgorithmSnappy::Decompress(data, ec);
    case constants::kCompressionLz4:
      return AlgorithmLz4::Decompress(data, ec);
    case constants::kCompressionNone:
      ec = kErrorSuccess;
      break;
    default:
      ec = kErrorCompressionNotAvailable;
      break;
  }
  return Bytes();
}

// Fallback implementation
template<constants::Compression C>
inline Bytes CompressionAlgorithm<C>::Compress(const Bytes& data,
                                               boost::system::error_code& ec)
{
  ec = kErrorCompressionNotAvailable;
  return Bytes();
}

// Fallback implementation
template<constants::Compression C>
inline Bytes CompressionAlgorithm<C>::Decompress(const Bytes& data,
                                                 boost::system::error_code& ec)
{
  ec = kErrorCompressionNotAvailable;
  return Bytes();
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // COMPRESSION_H_6B15B598_5C1D_447E_AF74_E541088998FA
