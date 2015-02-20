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
#include <boost/detail/endian.hpp>

namespace libkafka_asio
{
namespace detail
{

// Convert the given 64 bit integer to big endian.
inline Int64 be_to_host_64(Int64 ll)
{
#ifdef BOOST_LITTLE_ENDIAN
  ll = (ll >> 56) |
    ((ll << 40) & UINT64_C(0x00FF000000000000)) |
    ((ll << 24) & UINT64_C(0x0000FF0000000000)) |
    ((ll << 8) & UINT64_C(0x000000FF00000000)) |
    ((ll >> 8) & UINT64_C(0x00000000FF000000)) |
    ((ll >> 24) & UINT64_C(0x0000000000FF0000)) |
    ((ll >> 40) & UINT64_C(0x000000000000FF00)) |
    (ll << 56);
#endif
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
