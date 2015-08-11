//
// detail/request_write.h
// ----------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef REQUEST_WRITE_H_5475991E_0B9F_42A7_97EC_6E5206FB6A6A
#define REQUEST_WRITE_H_5475991E_0B9F_42A7_97EC_6E5206FB6A6A

#include <iostream>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/message_fwd.h>

namespace libkafka_asio
{

//
// Forward declarations
//

class MetadataRequest;

class ProduceRequest;

class FetchRequest;

class OffsetRequest;

class ConsumerMetadataRequest;

class OffsetCommitRequest;

class OffsetFetchRequest;

namespace detail
{

Int32 StringWireSize(const String& str);

Int32 BytesWireSize(const Bytes& bytes);

Int32 MessageWireSize(const Message& message);

Int32 MessageSetWireSize(const MessageSet& message_set);

template<typename TRequest>
Int32 RequestWireSize(const TRequest& request, const String& client_id);

Int32 RequestMessageWireSize(const MetadataRequest& request);

Int32 RequestMessageWireSize(const ProduceRequest& request);

Int32 RequestMessageWireSize(const FetchRequest& request);

Int32 RequestMessageWireSize(const OffsetRequest& request);

Int32 RequestMessageWireSize(const ConsumerMetadataRequest& request);

Int32 RequestMessageWireSize(const OffsetCommitRequest& request);

Int32 RequestMessageWireSize(const OffsetFetchRequest& request);

void WriteInt8(Int8 value, std::ostream& os);

void WriteInt16(Int16 value, std::ostream& os);

void WriteInt32(Int32 value, std::ostream& os);

void WriteInt64(Int64 value, std::ostream& os);

void WriteString(const String& value, std::ostream& os);

void WriteBytes(const Bytes& value, std::ostream& os);

void WriteMessage(const Message& value, std::ostream& os);

void WriteMessageSet(const MessageSet& value, std::ostream& os);

template<typename TRequest>
void WriteRequest(const TRequest& request, const String& client_id,
                  std::ostream& os);

void WriteRequestMessage(const MetadataRequest& request, std::ostream& os);

void WriteRequestMessage(const ProduceRequest& request, std::ostream& os);

void WriteRequestMessage(const FetchRequest& request, std::ostream& os);

void WriteRequestMessage(const OffsetRequest& request, std::ostream& os);

void WriteRequestMessage(const ConsumerMetadataRequest& request,
                         std::ostream& os);

void WriteRequestMessage(const OffsetCommitRequest& request, std::ostream& os);

void WriteRequestMessage(const OffsetFetchRequest& request, std::ostream& os);

}  // namespace detail
}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/request_write.h>

#endif  // REQUEST_WRITE_H_5475991E_0B9F_42A7_97EC_6E5206FB6A6A
