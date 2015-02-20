//
// metadata_request.h
// ------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef METADATA_REQUEST_H_07C079EE_94FF_41EB_9CA6_618E552E405F
#define METADATA_REQUEST_H_07C079EE_94FF_41EB_9CA6_618E552E405F

#include <vector>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/request.h>
#include <libkafka_asio/metadata_response.h>

namespace libkafka_asio
{

// Kafka Metadata API request implementation.
// The metadata API can be used to determine information about available topics,
// partitions and brokers.
class MetadataRequest :
  public Request<MetadataRequest>
{
  friend class Request<MetadataRequest>;

  static Int16 ApiKey();

public:
  typedef MetadataResponse ResponseType;
  typedef MutableMetadataResponse MutableResponseType;
  typedef std::vector<String> TopicNameVector;

  const TopicNameVector& topic_names() const;

  // Add a topic to fetch metadata for
  void AddTopicName(const String& topic_name);

  // Removes all topic name entries, added to this request
  void Clear();

private:
  TopicNameVector topic_names_;
};

}  // namespace libkafka_asio

#include <libkafka_asio/impl/metadata_request.h>

#endif  // METADATA_REQUEST_H_07C079EE_94FF_41EB_9CA6_618E552E405F
