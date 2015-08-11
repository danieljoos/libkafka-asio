//
// detail/endian.h
// ---------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef ENDIAN_H_6E481133_65F6_495C_A384_557C81B0C628
#define ENDIAN_H_6E481133_65F6_495C_A384_557C81B0C628

#include <libkafka_asio/primitives.h>
#include <boost/asio.hpp>
#include <boost/detail/endian.hpp>

namespace libkafka_asio
{
namespace detail
{

// Convert the given 64 bit integer to big endian.
inline Int64 be_to_host_64(Int64 ll)
{
#ifdef BOOST_LITTLE_ENDIAN
  ll = (((uint64_t) htonl((uint32_t) ((ll << 32) >> 32))) << 32) |
       (uint32_t) htonl((uint32_t) (ll >> 32));
#endif  // LITTLE_ENDIAN
  return ll;
}

// Convert the 64 Bit integer back to node specific endianess, which might be
// little endian.
inline Int64 host_to_be_64(Int64 ll)
{
  return be_to_host_64(ll);
}

}  // detail
}  // libkafka_asio

#endif  // ENDIAN_H_6E481133_65F6_495C_A384_557C81B0C628
