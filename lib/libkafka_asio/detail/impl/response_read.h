//
// detail/impl/response_read.h
// ---------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef REQUEST_READ_H_2C4DEC07_68A7_48A3_A19E_4ECDE2AF19D9
#define REQUEST_READ_H_2C4DEC07_68A7_48A3_A19E_4ECDE2AF19D9

#include <boost/asio.hpp>
#include <libkafka_asio/detail/endian.h>
#include <libkafka_asio/detail/compression.h>
#include <libkafka_asio/detail/bytes_streambuf.h>

namespace libkafka_asio
{
namespace detail
{

inline Int8 ReadInt8(std::istream& is)
{
  Int8 result = 0;
  is.read(reinterpret_cast<char *>(&result), sizeof(Int8));
  return result;
}

inline Int16 ReadInt16(std::istream& is)
{
  Int16 result = 0;
  is.read(reinterpret_cast<char *>(&result), sizeof(Int16));
  result = ntohs(result);
  return result;
}

inline Int32 ReadInt32(std::istream& is)
{
  Int32 result = 0;
  is.read(reinterpret_cast<char *>(&result), sizeof(Int32));
  result = ntohl(result);
  return result;
}

inline Int64 ReadInt64(std::istream& is)
{
  Int64 result = 0;
  is.read(reinterpret_cast<char *>(&result), sizeof(Int64));
  result = be_to_host_64(result);
  return result;
}

inline String ReadString(std::istream& is)
{
  Int16 length = ReadInt16(is);
  if (length > 0)
  {
    String result(length, '\0');
    is.read(&result[0], length);
    return result;
  }
  return "";
}

inline void ReadBytes(std::istream& is, Bytes& bytes)
{
  Int32 length = ReadInt32(is);
  if (length > 0)
  {
    bytes.reset(new Bytes::element_type(length, 0));
    is.read(reinterpret_cast<char *>(&(*bytes)[0]), length);
  }
}

inline void ReadMessage(std::istream& is, Message& message)
{
  Int32 crc = ReadInt32(is);
  Int8 magic_byte = ReadInt8(is);
  Int8 attributes = ReadInt8(is);
  ReadBytes(is, message.mutable_key());
  ReadBytes(is, message.mutable_value());

  message.SetCompression(attributes);
  boost::system::error_code ec;
  Bytes data = Decompress(message.value(), message.compression(), ec);
  if (!ec && data && !data->empty())
  {
    BytesStreambuf intermediate_buffer(data);
    std::istream intermediate_is(&intermediate_buffer);
    ReadMessageSet(intermediate_is, message.mutable_nested_message_set(),
                   data->size());
  }

}

inline void ReadMessageSet(std::istream& is,
                           MessageSet& message_set,
                           size_t size)
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
      ReadMessage(is, message);
      message_set.push_back(message);
      read_count += sizeof(Int64) + sizeof(Int32) + message_size;
    }
  }
}

template<typename TMutableResponse>
void ReadResponse(std::istream& is,
                  TMutableResponse& response,
                  boost::system::error_code& ec)
{
  response.set_correlation_id(ReadInt32(is));
  ReadResponseMessage(is, response, ec);
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // REQUEST_READ_H_2C4DEC07_68A7_48A3_A19E_4ECDE2AF19D9
