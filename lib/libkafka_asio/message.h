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
#include <libkafka_asio/error.h>
#include <libkafka_asio/message_fwd.h>

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

  // Sets the attributes byte of this message
  void set_attributes(Int8 attributes);

  // Optional message key. Can be NULL (default).
  const Bytes& key() const;

  Bytes& mutable_key();

  // Actual message data as byte array
  const Bytes& value() const;

  Bytes& mutable_value();

  // Compressed messages contain a nested message set
  const MessageSet& nested_message_set() const;

  MessageSet& mutable_nested_message_set();

  // Returns the compression algorithm, used for compressing the message value
  constants::Compression compression() const;

private:
  Int8 attributes_;
  Bytes key_;
  Bytes value_;
  boost::shared_ptr<MessageSet> nested_message_set_;
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

// Compresses the given `MessageSet` object using the specified compression
// algorithm and puts the result as value into a new `Message` object. The
// compression attribute of that object will be set respectively. The created
// `Message` object will be returned by this function.
Message CompressMessageSet(const MessageSet& message_set,
                           constants::Compression compression,
                           boost::system::error_code& ec);

}  // namespace libkafka_asio

#include <libkafka_asio/impl/message.h>

#endif  // MESSAGE_H_7E5FAA53_67B6_4727_BF98_3AECC80F8825
