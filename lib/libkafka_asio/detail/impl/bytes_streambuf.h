//
// detail/impl/bytes_streambuf.h
// -----------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef BYTES_STREAMBUF_H_F7347ACB_448C_4507_924D_2A0C7A512E79
#define BYTES_STREAMBUF_H_F7347ACB_448C_4507_924D_2A0C7A512E79

namespace libkafka_asio
{
namespace detail
{

inline BytesStreambuf::BytesStreambuf(Bytes data) :
  data_(data)
{
  if (data_ && !data->empty())
  {
    char_type *buffer_begin = reinterpret_cast<char_type *>(&(*data_)[0]);
    char_type *buffer_end = buffer_begin + data->size();
    setg(buffer_begin, buffer_begin, buffer_end);
  }
}

inline Bytes BytesStreambuf::data() const
{
  return data_;
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // BYTES_STREAMBUF_H_F7347ACB_448C_4507_924D_2A0C7A512E79
