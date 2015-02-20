//
// detail/impl/metadata_request_write.h
// ------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef METADATA_REQUEST_WRITE_H_28BB9709_A65B_44C9_91C3_0A380BD76F7D
#define METADATA_REQUEST_WRITE_H_28BB9709_A65B_44C9_91C3_0A380BD76F7D

#include <boost/foreach.hpp>
#include <libkafka_asio/detail/request_write.h>

namespace libkafka_asio
{
namespace detail
{

inline Int32 RequestMessageWireSize(const MetadataRequest& request)
{
  Int32 size = sizeof(Int32);
  BOOST_FOREACH(const String& topic_name, request.topic_names())
  {
    size += StringWireSize(topic_name);
  }
  return size;
}

inline void WriteRequestMessage(const MetadataRequest& request,
                                std::ostream& os)
{
  // Topic Names
  WriteInt32(static_cast<Int32>(request.topic_names().size()), os);
  BOOST_FOREACH(const String& topic_name, request.topic_names())
  {
    WriteString(topic_name, os);
  }
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // METADATA_REQUEST_WRITE_H_28BB9709_A65B_44C9_91C3_0A380BD76F7D
