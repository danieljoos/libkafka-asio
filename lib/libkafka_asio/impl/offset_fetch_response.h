//
// impl/offset_fetch_response.h
// ----------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_FETCH_RESPONSE_H_DAC75554_54A7_424A_984C_56EA3C50749E
#define OFFSET_FETCH_RESPONSE_H_DAC75554_54A7_424A_984C_56EA3C50749E

namespace libkafka_asio
{

inline const OffsetFetchResponse::Topics& OffsetFetchResponse::topics() const
{
  return topics_;
}

inline OffsetFetchResponse::Topics& MutableOffsetFetchResponse::mutable_topics()
{
  return response_.topics_;
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/offset_fetch_response_read.h>

#endif  // OFFSET_FETCH_RESPONSE_H_DAC75554_54A7_424A_984C_56EA3C50749E
