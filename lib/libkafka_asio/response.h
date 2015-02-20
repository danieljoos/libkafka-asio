//
// response.h
// ----------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef RESPONSE_H_96E99D03_30D1_4F88_B5E8_002B7CF6A32E
#define RESPONSE_H_96E99D03_30D1_4F88_B5E8_002B7CF6A32E

#include <boost/optional.hpp>
#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{

template<typename TResponse>
class MutableResponse;

// Base response template
template<typename TResponse>
class Response
{
  friend class MutableResponse<TResponse>;

public:
  typedef boost::optional<TResponse> OptionalType;

  Int32 correlation_id() const
  {
    return correlation_id_;
  }

private:
  Int32 correlation_id_;
};

template<typename TResponse>
class MutableResponse
{
public:
  void set_correlation_id(Int32 correlation_id)
  {
    response_.correlation_id_ = correlation_id;
  }

  const TResponse& response() const
  {
    return response_;
  }

protected:
  TResponse response_;
};

}  // namespace libkafka_asio

#endif  // RESPONSE_H_96E99D03_30D1_4F88_B5E8_002B7CF6A32E
