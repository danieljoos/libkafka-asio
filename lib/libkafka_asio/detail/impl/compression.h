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
  using namespace libkafka_asio::constants;

  switch (compression)
  {
    case kCompressionGZIP:
      return CompressionPolicy<kCompressionGZIP>::
      Algorithm::Compress(data, ec);
    case kCompressionSnappy:
      return CompressionPolicy<kCompressionSnappy>::
      Algorithm::Compress(data, ec);
    case kCompressionLz4:
      return CompressionPolicy<kCompressionLz4>::
      Algorithm::Compress(data, ec);
    case kCompressionNone:
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
  using namespace libkafka_asio::constants;

  switch (compression)
  {
    case kCompressionGZIP:
      return CompressionPolicy<kCompressionGZIP>::
      Algorithm::Decompress(data, ec);
    case kCompressionSnappy:
      return CompressionPolicy<kCompressionSnappy>::
      Algorithm::Decompress(data, ec);
    case kCompressionLz4:
      return CompressionPolicy<kCompressionLz4>::
      Algorithm::Decompress(data, ec);
    case kCompressionNone:
      ec = kErrorSuccess;
      break;
    default:
      ec = kErrorCompressionNotAvailable;
      break;
  }

  return Bytes();
}

inline Bytes FallbackCompressionAlgorithm::Compress(
  const Bytes&, boost::system::error_code& ec)
{
  ec = kErrorCompressionNotAvailable;
  return Bytes();
}

inline Bytes FallbackCompressionAlgorithm::Decompress(
  const Bytes&, boost::system::error_code& ec)
{
  ec = kErrorCompressionNotAvailable;
  return Bytes();
}


}  // namespace detail
}  // namespace libkafka_asio

#endif  // COMPRESSION_H_6B15B598_5C1D_447E_AF74_E541088998FA
