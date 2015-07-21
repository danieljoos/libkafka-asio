//
// detail/response_read.h
// ----------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef RESPONSE_READ_H_TODO
#define RESPONSE_READ_H_TODO

#include <iostream>
#include <boost/system/error_code.hpp>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/response.h>
#include <libkafka_asio/message_fwd.h>

namespace libkafka_asio
{

//
// Forward declarations
//

class MutableMetadataResponse;

class MutableProduceResponse;

class MutableFetchResponse;

class MutableOffsetResponse;

class MutableConsumerMetadataResponse;

class MutableOffsetCommitResponse;

class MutableOffsetFetchResponse;

namespace detail
{

Int8 ReadInt8(std::istream& is);

Int16 ReadInt16(std::istream& is);

Int32 ReadInt32(std::istream& is);

Int64 ReadInt64(std::istream& is);

String ReadString(std::istream& is);

void ReadBytes(std::istream& is, Bytes& bytes);

void ReadMessage(std::istream& is,
                 Message& message,
                 boost::system::error_code& ec);

void ReadMessageSet(std::istream& is,
                    MessageSet& message_set,
                    size_t size,
                    boost::system::error_code& ec);

template<typename TMutableResponse>
void ReadResponse(std::istream& is,
                  TMutableResponse& response,
                  boost::system::error_code& ec);

void ReadResponseMessage(std::istream& is,
                         MutableMetadataResponse& response,
                         boost::system::error_code& ec);

void ReadResponseMessage(std::istream& is,
                         MutableProduceResponse& response,
                         boost::system::error_code& ec);

void ReadResponseMessage(std::istream& is,
                         MutableFetchResponse& response,
                         boost::system::error_code& ec);

void ReadResponseMessage(std::istream& is,
                         MutableOffsetResponse& response,
                         boost::system::error_code& ec);

void ReadResponseMessage(std::istream& is,
                         MutableConsumerMetadataResponse& response,
                         boost::system::error_code& ec);

void ReadResponseMessage(std::istream& is,
                         MutableOffsetCommitResponse& response,
                         boost::system::error_code& ec);

void ReadResponseMessage(std::istream& is,
                         MutableOffsetFetchResponse& response,
                         boost::system::error_code& ec);

}  // namespace detail
}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/response_read.h>

#endif  // RESPONSE_READ_H_TODO
