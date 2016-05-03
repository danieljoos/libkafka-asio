//
// impl/offset_commit_response.h
// -----------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef OFFSET_COMMIT_RESPONSE_H_6DDDCEF0_8532_4D06_91EB_EDD87BCB2FB6
#define OFFSET_COMMIT_RESPONSE_H_6DDDCEF0_8532_4D06_91EB_EDD87BCB2FB6

namespace libkafka_asio
{

inline const OffsetCommitResponse::Topics& OffsetCommitResponse::topics() const
{
  return topics_;
}

inline OffsetCommitResponse::Topics&
MutableOffsetCommitResponse::mutable_topics()
{
  return response_.topics_;
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/offset_commit_response_read.h>

#endif  // OFFSET_COMMIT_RESPONSE_H_6DDDCEF0_8532_4D06_91EB_EDD87BCB2FB6
