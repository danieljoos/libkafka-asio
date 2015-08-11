//
// detail/bytes_streambuf.h
// ------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef BYTES_STREAMBUF_H_3909DFCD_867A_4A2C_A494_3AAAE9A751E2
#define BYTES_STREAMBUF_H_3909DFCD_867A_4A2C_A494_3AAAE9A751E2

#include <streambuf>
#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{
namespace detail
{

class BytesStreambuf :
  public std::streambuf
{
public:
  explicit BytesStreambuf(Bytes data);

  Bytes data() const;

private:
  Bytes data_;
};

}  // namespace detail
}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/bytes_streambuf.h>

#endif  // BYTES_STREAMBUF_H_3909DFCD_867A_4A2C_A494_3AAAE9A751E2
