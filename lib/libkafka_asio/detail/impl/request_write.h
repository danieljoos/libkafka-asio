//
// detail/impl/request_write.h
// ---------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef REQUEST_WRITE_H_5A99E292_8486_40A6_8B1B_90D78026F803
#define REQUEST_WRITE_H_5A99E292_8486_40A6_8B1B_90D78026F803

#include <libkafka_asio/detail/endian.h>

namespace libkafka_asio
{
namespace detail
{

inline Int32 StringWireSize(const String& str)
{
  return static_cast<Int32>(sizeof(Int16) + str.size());
}

inline Int32 BytesWireSize(const Bytes& bytes)
{
  Int32 size = sizeof(Int32);
  if (bytes)
  {
    size += static_cast<Int32>(bytes->size());
  }
  return size;
}

template< typename TRequest >
Int32 RequestWireSize(const TRequest& request, const String& client_id)
{
  return
    sizeof(Int16) +  // ApiKey
    sizeof(Int16) +  // ApiVersion
    sizeof(Int32) +  // CorrelationId
    detail::StringWireSize(client_id) +  // ClientId
    RequestMessageWireSize(request);  // RequestMessage
}

inline void WriteInt8(Int8 value, std::ostream& os)
{
  os.write( reinterpret_cast<char*>(&value), sizeof(Int8) );
}

inline void WriteInt16(Int16 value, std::ostream& os)
{
  value = htons(value);
  os.write( reinterpret_cast<char*>(&value), sizeof(Int16) );
}

inline void WriteInt32(Int32 value, std::ostream& os)
{
  value = htonl(value);
  os.write( reinterpret_cast<char*>(&value), sizeof(Int32) );
}

inline void WriteInt64(Int64 value, std::ostream& os)
{
  value = host_to_be_64(value);
  os.write( reinterpret_cast<char*>(&value), sizeof(Int64) );
}

inline void WriteString(const String& value, std::ostream& os)
{
  WriteInt16(static_cast<Int16>(value.size()), os);
  os.write( value.c_str(), value.size() );
}

inline void WriteBytes(const Bytes& value, std::ostream& os)
{
  if (!value)
  {
    WriteInt32(-1, os);
    return;
  }
  WriteInt32(static_cast<Int32>(value->size()), os);
  if (!value->empty())
  {
    os.write(reinterpret_cast<const char*>(&(*value)[0]), value->size());
  }
}

template< typename TRequest >
void WriteRequest(const TRequest& request, const String& client_id,
                  std::ostream& os)
{
  WriteInt32(RequestWireSize(request, client_id), os);
  WriteInt16(request.api_key(), os);
  WriteInt16(request.api_version(), os);
  WriteInt32(request.correlation_id(), os);
  WriteString(client_id, os);
  WriteRequestMessage(request, os);
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // REQUEST_WRITE_H_5A99E292_8486_40A6_8B1B_90D78026F803
