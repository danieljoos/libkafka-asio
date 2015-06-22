//
// impl/message.h
// --------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef MESSAGE_H_AFAF4566_F008_41D9_962B_EE74729E4F4F
#define MESSAGE_H_AFAF4566_F008_41D9_962B_EE74729E4F4F

#include <libkafka_asio/detail/impl/message_read.h>
#include <libkafka_asio/detail/impl/message_write.h>

namespace libkafka_asio
{

inline Message::Message() :
  attributes_(0),
  nested_message_set_(new MessageSet())
{
}

inline Message::Message(const Message& orig, bool deep) :
  attributes_(orig.attributes_),
  nested_message_set_(orig.nested_message_set_)
{
  if (deep)
  {
    if (orig.key())
    {
      key_.reset(new Bytes::element_type(*orig.key()));
    }
    if (orig.value())
    {
      value_.reset(new Bytes::element_type(*orig.value()));
    }
    if (orig.nested_message_set_)
    {
      nested_message_set_.reset(new MessageSet(*orig.nested_message_set_));
    }
  }
  else
  {
    key_ = orig.key();
    value_ = orig.value();
    nested_message_set_ = orig.nested_message_set_;
  }
}

inline Message& Message::operator=(const Message& rhs)
{
  attributes_ = rhs.attributes_;
  key_ = rhs.key_;
  value_ = rhs.value_;
  nested_message_set_ = rhs.nested_message_set_;
  return *this;
}

inline Int8 Message::magic_byte() const
{
  return 0;
}

inline Int8 Message::attributes() const
{
  return attributes_;
}

inline void Message::set_attributes(Int8 attributes)
{
  attributes_ = attributes;
}

inline const Bytes& Message::key() const
{
  return key_;
}

inline Bytes& Message::mutable_key()
{
  return key_;
}

inline const Bytes& Message::value() const
{
  return value_;
}

inline Bytes& Message::mutable_value()
{
  return value_;
}

inline const MessageSet& Message::nested_message_set() const
{
  return *nested_message_set_;
}

inline MessageSet& Message::mutable_nested_message_set()
{
  return *nested_message_set_;
}

inline constants::Compression Message::compression() const
{
  return constants::Compression(attributes_ & 0x03);
}

inline MessageAndOffset::MessageAndOffset() :
  offset_(0)
{
}

inline MessageAndOffset::MessageAndOffset(const Message& message,
                                          Int64 offset) :
  Message(message),
  offset_(offset)
{
}

inline Int64 MessageAndOffset::offset() const
{
  return offset_;
}

inline void MessageAndOffset::set_offset(Int64 offset)
{
  offset_ = offset;
}

inline Message CompressMessageSet(const MessageSet& message_set,
                                  constants::Compression compression,
                                  boost::system::error_code& ec)
{
  using boost::asio::buffer;
  using boost::asio::buffer_copy;
  using boost::asio::streambuf;
  using boost::system::error_code;
  using detail::WriteMessageSet;
  using detail::Compress;

  if (compression == constants::kCompressionNone)
  {
    ec = kErrorCompressionFailed;
    return Message();
  }
  streambuf intermediate_buffer;
  std::ostream intermediate_os(&intermediate_buffer);
  WriteMessageSet(message_set, intermediate_os);
  size_t size = intermediate_buffer.size();
  if (size == 0)
  {
    ec = kErrorCompressionFailed;
    return Message();
  }
  intermediate_buffer.commit(size);
  Bytes uncompressed_value(new Bytes::element_type(size));
  buffer_copy(buffer(&(*uncompressed_value)[0], size),
              intermediate_buffer.data());
  Message result;
  result.set_attributes(compression);
  result.mutable_value() = Compress(uncompressed_value, compression, ec);
  if (ec)
  {
    return Message();
  }
  ec = kErrorSuccess;
  return result;
}

}  // namespace libkafka_asio

#endif  // MESSAGE_H_AFAF4566_F008_41D9_962B_EE74729E4F4F
