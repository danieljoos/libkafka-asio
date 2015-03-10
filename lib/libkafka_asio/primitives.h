//
// primitives.h
// ------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef PRIMITIVES_H_2018391E_E5C4_4FB1_8271_CAD5C2C99951
#define PRIMITIVES_H_2018391E_E5C4_4FB1_8271_CAD5C2C99951

#include <string>
#include <vector>
#include <boost/integer.hpp>
#include <boost/shared_ptr.hpp>

namespace libkafka_asio
{

//
// The Kafka protocol primitives, as described in the Kafka wiki.
//

typedef boost::int_t<8>::exact Int8;
typedef boost::int_t<16>::exact Int16;
typedef boost::int_t<32>::exact Int32;
typedef boost::int_t<64>::exact Int64;
typedef boost::uint_t<8>::exact Byte;
typedef std::string String;
typedef boost::shared_ptr<std::vector<Byte> > Bytes;

}  // namespace libkafka_asio

#endif  // PRIMITIVES_H_2018391E_E5C4_4FB1_8271_CAD5C2C99951
