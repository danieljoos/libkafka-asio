//
// detail/impl/message_read.h
// --------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef MESSAGE_READ_H_5892DA02_680A_469F_9624_A68F5D3E1FC5
#define MESSAGE_READ_H_5892DA02_680A_469F_9624_A68F5D3E1FC5

#include <boost/asio.hpp>
#include <libkafka_asio/detail/compression.h>
#include <libkafka_asio/detail/bytes_streambuf.h>

namespace libkafka_asio
{
namespace detail
{

inline void ReadMessage(std::istream& is,
                        Message& message,
                        boost::system::error_code& ec)
{
  Int32 crc = ReadInt32(is);
  Int8 magic_byte = ReadInt8(is);
  // discarding results of above reads
  (void)crc;
  (void)magic_byte;

  message.set_attributes(ReadInt8(is));
  ReadBytes(is, message.mutable_key());
  ReadBytes(is, message.mutable_value());
  Bytes data = Decompress(message.value(), message.compression(), ec);
  if (!ec && data && !data->empty())
  {
    BytesStreambuf intermediate_buffer(data);
    std::istream intermediate_is(&intermediate_buffer);
    ReadMessageSet(intermediate_is, message.mutable_nested_message_set(),
                   data->size(), ec);
  }
}

inline void ReadMessageSet(std::istream& is,
                           MessageSet& message_set,
                           size_t size,
                           boost::system::error_code& ec)
{
  size_t read_count = 0;
  while (read_count < size)
  {
    MessageAndOffset message;
    message.set_offset(ReadInt64(is));
    Int32 message_size = ReadInt32(is);
    if ((Int32) (size - read_count) < message_size)
    {
      // Ignore partial messages
      is.seekg(size - read_count, std::ios::cur);
      read_count = size;
    }
    else
    {
      ReadMessage(is, message, ec);
      message_set.push_back(message);
      read_count += sizeof(Int64) + sizeof(Int32) + message_size;
    }
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // MESSAGE_READ_H_5892DA02_680A_469F_9624_A68F5D3E1FC5
