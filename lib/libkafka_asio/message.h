//
// message.h
// ---------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef MESSAGE_H_7E5FAA53_67B6_4727_BF98_3AECC80F8825
#define MESSAGE_H_7E5FAA53_67B6_4727_BF98_3AECC80F8825

#include <vector>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/constants.h>

namespace libkafka_asio
{

// Kafka Message Data Structure
class Message
{
public:
  // Create a new and empty message object
  Message();

  // Copy from the given message object. If the `deep` parameter is set to
  // `true`, the byte arrays for `key` and `value` will be copied as well.
  Message(const Message& orig, bool deep = false);

  // Copy from the given message object
  Message& operator=(const Message& rhs);

  // Magic byte is always zero
  Int8 magic_byte() const;

  // Attributes bitset. The lowest 2 bits indicate the used compression
  // algorithm.
  Int8 attributes() const;

  // Optional message key. Can be NULL (default).
  const Bytes& key() const;

  Bytes& mutable_key();

  // Actual message data as byte array
  const Bytes& value() const;

  Bytes& mutable_value();

  // Enable message compression using the specified compression algorithm.
  void SetCompression(constants::Compression compression_type);

private:
  Int8 attributes_;
  Bytes key_;
  Bytes value_;
};

// Message data structure with an additional offset
class MessageAndOffset :
  public Message
{
public:
  MessageAndOffset();

  MessageAndOffset(const Message& message, Int64 offset);

  Int64 offset() const;

  void set_offset(Int64 offset);

private:
  Int64 offset_;
};

// Kafka MessageSet data structure
typedef std::vector<MessageAndOffset> MessageSet;

}  // namespace libkafka_asio

#include <libkafka_asio/impl/message.h>

#endif  // MESSAGE_H_7E5FAA53_67B6_4727_BF98_3AECC80F8825
