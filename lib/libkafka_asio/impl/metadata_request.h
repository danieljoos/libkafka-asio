//
// impl/metadata_request.h
// -----------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef METADATA_REQUEST_H_6A4217F2_F071_46BC_9B26_FC84451D03A5
#define METADATA_REQUEST_H_6A4217F2_F071_46BC_9B26_FC84451D03A5

#include <libkafka_asio/constants.h>

namespace libkafka_asio
{

inline Int16 MetadataRequest::ApiKey()
{
  return constants::kApiKeyMetadataRequest;
}

inline const MetadataRequest::TopicNameVector& MetadataRequest::topic_names()
const
{
  return topic_names_;
}

inline void MetadataRequest::AddTopicName(const String& topic_name)
{
  topic_names_.push_back(topic_name);
}

inline void MetadataRequest::Clear()
{
  topic_names_.clear();
}

}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/metadata_request_write.h>

#endif  // METADATA_REQUEST_H_6A4217F2_F071_46BC_9B26_FC84451D03A5
