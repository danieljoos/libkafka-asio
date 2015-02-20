//
// request.h
// ---------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef BASE_REQUEST_H_693E2835_7487_4561_8C4B_590D06336668
#define BASE_REQUEST_H_693E2835_7487_4561_8C4B_590D06336668

#include <libkafka_asio/primitives.h>
#include <libkafka_asio/constants.h>

namespace libkafka_asio
{

// Base request template
template<typename TRequest>
class Request
{
public:
  Request() :
    correlation_id_(constants::kDefaultCorrelationId)
  {
  }

  inline Int16 api_key() const
  {
    return TRequest::ApiKey();
  }

  Int16 api_version() const
  {
    return 0;
  }

  Int32 correlation_id() const
  {
    return correlation_id_;
  }

  // Set the correlation ID. The Kafka server will put this value into the
  // corresponding response message.
  void set_correlation_id(Int32 correlation_id)
  {
    correlation_id_ = correlation_id;
  }

  bool ResponseExpected() const
  {
    return true;
  }

private:
  Int32 correlation_id_;
  String client_id_;
};

}  // namespace libkafka_asio

#endif  // BASE_REQUEST_H_693E2835_7487_4561_8C4B_590D06336668
