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

typedef boost::int8_t Int8;
typedef boost::int16_t Int16;
typedef boost::int32_t Int32;
typedef boost::int64_t Int64;
typedef boost::uint8_t Byte;
typedef std::string String;
typedef boost::shared_ptr<std::vector<Byte> > Bytes;

}  // namespace libkafka_asio

#endif  // PRIMITIVES_H_2018391E_E5C4_4FB1_8271_CAD5C2C99951
