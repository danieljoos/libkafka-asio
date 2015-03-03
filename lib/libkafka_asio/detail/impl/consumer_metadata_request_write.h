//
// detail/impl/consumer_metadata_request_write.h
// ---------------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CONSUMER_METADATA_REQUEST_WRITE_H_C58C7485_93D9_4595_9166_34C4CEC79445
#define CONSUMER_METADATA_REQUEST_WRITE_H_C58C7485_93D9_4595_9166_34C4CEC79445

#include <libkafka_asio/detail/request_write.h>

namespace libkafka_asio
{
namespace detail
{

inline Int32 RequestMessageWireSize(const ConsumerMetadataRequest& request)
{
  return StringWireSize(request.consumer_group());
}

inline void WriteRequestMessage(const ConsumerMetadataRequest& request,
                                std::ostream& os)
{
  WriteString(request.consumer_group(), os);
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // CONSUMER_METADATA_REQUEST_WRITE_H_C58C7485_93D9_4595_9166_34C4CEC79445
