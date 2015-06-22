//
// detail/impl/message_write.h
// --------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef MESSAGE_WRITE_H_201F6605_6810_441C_9F25_47D8D669A771
#define MESSAGE_WRITE_H_201F6605_6810_441C_9F25_47D8D669A771

#include <boost/asio.hpp>
#include <boost/crc.hpp>
#include <boost/foreach.hpp>
#include <libkafka_asio/detail/request_write.h>

namespace libkafka_asio
{
namespace detail
{

inline Int32 MessageWireSize(const Message& message)
{
  return
    sizeof(Int32) +  // Crc
    sizeof(Int8) +   // MagicByte
    sizeof(Int8) +   // Attributes
    BytesWireSize( message.key() ) +  // Key
    BytesWireSize( message.value() );  // Value
}

inline Int32 MessageSetWireSize(const MessageSet& message_set)
{
  Int32 size = 0;
  BOOST_FOREACH(const MessageAndOffset &message, message_set)
  {
    size +=
      sizeof(Int64) +  // Offset
      sizeof(Int32) +  // MessageSize
      MessageWireSize(message);  // Message
  }
  return size;
}

inline void WriteMessage(const Message& value, std::ostream& os)
{
  using boost::asio::buffer_cast;

  // Write everything (except crc) to an intermediate buffer
  boost::asio::streambuf intermediate_buffer;
  std::ostream intermediate_os(&intermediate_buffer);
  WriteInt8(value.magic_byte(), intermediate_os);
  WriteInt8(value.attributes(), intermediate_os);
  WriteBytes(value.key(), intermediate_os);
  WriteBytes(value.value(), intermediate_os);

  size_t size = intermediate_buffer.size();
  intermediate_buffer.commit(size);

  // Calculate crc
  boost::crc_32_type crc;
  crc.process_bytes(buffer_cast<void const*>(intermediate_buffer.data()), size);

  // Write to the real stream
  WriteInt32(crc.checksum(), os);
  os.write(buffer_cast<const char*>(intermediate_buffer.data()), size);
}

inline void WriteMessageSet(const MessageSet& value, std::ostream& os)
{
  BOOST_FOREACH(const MessageAndOffset &message, value)
  {
    WriteInt64(message.offset(), os);
    WriteInt32(MessageWireSize(message), os);
    WriteMessage(message, os);
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // MESSAGE_WRITE_H_201F6605_6810_441C_9F25_47D8D669A771
