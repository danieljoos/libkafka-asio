//
// impl/fetch_response.h
// ---------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef FETCH_RESPONSE_H_DB84F3DB_3BBE_4D08_A577_F6B7C81552D1
#define FETCH_RESPONSE_H_DB84F3DB_3BBE_4D08_A577_F6B7C81552D1

namespace libkafka_asio
{

inline const FetchResponse::Topics& FetchResponse::topics() const
{
  return topics_;
}

inline FetchResponse::Topics& MutableFetchResponse::mutable_topics()
{
  return response_.topics_;
}

inline FetchResponse::const_iterator FetchResponse::begin() const
{
  return const_iterator(topics_);
}

inline FetchResponse::const_iterator FetchResponse::end() const
{
  return const_iterator();
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/fetch_response_read.h>

#endif  // FETCH_RESPONSE_H_DB84F3DB_3BBE_4D08_A577_F6B7C81552D1
