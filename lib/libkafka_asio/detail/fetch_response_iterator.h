//
// detail/fetch_response_iterator.h
// --------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef FETCH_RESPONSE_ITERATOR_H_6E567149_2564_4FF2_BCC4_F0205CB5C344
#define FETCH_RESPONSE_ITERATOR_H_6E567149_2564_4FF2_BCC4_F0205CB5C344

#include <boost/iterator/iterator_facade.hpp>
#include <libkafka_asio/message.h>
#include <libkafka_asio/detail/recursive_messageset_iterator.h>

namespace libkafka_asio
{
namespace detail
{

// Iterator over all messages received by a Fetch API request.
template<typename TTopicMap>
class FetchResponseIterator :
  public boost::iterator_facade<
    FetchResponseIterator<TTopicMap>,
    const MessageAndOffset,
    boost::forward_traversal_tag
  >
{
  typedef FetchResponseIterator<TTopicMap> TMy;
public:
  typedef typename TTopicMap::value_type Topic;
  typedef typename TTopicMap::mapped_type::PartitionMap TopicPartitionMap;
  typedef typename TopicPartitionMap::value_type TopicPartition;

  FetchResponseIterator() :
    topics_(NULL)
  {
  }

  explicit FetchResponseIterator(const TTopicMap& topics) :
    topics_(&topics)
  {
    topic_iter_ = topics_->begin();
    while (topic_iter_ != topics_->end())
    {
      topic_partition_iter_ = topic_iter_->second.partitions.begin();
      while (topic_partition_iter_ != topic_iter_->second.partitions.end())
      {
        message_iter_ =
          RecursiveMessageSetIterator(topic_partition_iter_->second.messages);
        if (message_iter_ != RecursiveMessageSetIterator())
        {
          // Found a message to start with
          return;
        }
        topic_partition_iter_++;
      }
      topic_iter_++;
    }
    // Found nothing...
    reset();
  }

  const Topic& topic() const
  {
    return *topic_iter_;
  }

  const TopicPartition& topic_partition() const
  {
    return *topic_partition_iter_;
  }

private:
  friend class boost::iterator_core_access;

  typedef typename TTopicMap::const_iterator TopicIterator;
  typedef typename TopicPartitionMap::const_iterator TopicPartitionIterator;

  void reset()
  {
    topics_ = NULL;
  }

  void increment()
  {
    if (!topics_)
    {
      return;
    }
    message_iter_++;
    while (message_iter_ == RecursiveMessageSetIterator())
    {
      topic_partition_iter_++;
      while (topic_partition_iter_ == topic_iter_->second.partitions.end())
      {
        topic_iter_++;
        if (topic_iter_ == topics_->end())
        {
          reset();
          return;
        }
        topic_partition_iter_ = topic_iter_->second.partitions.begin();
      }
      message_iter_ =
        RecursiveMessageSetIterator(topic_partition_iter_->second.messages);
    }
  }

  bool equal(const TMy& other) const
  {
    if (topics_)
    {
      return other.topics_ == topics_ &&
        other.message_iter_ == message_iter_;
    }
    return other.topics_ == topics_;
  }

  const MessageAndOffset& dereference() const
  {
    return *message_iter_;
  }

  const TTopicMap *topics_;
  TopicIterator topic_iter_;
  TopicPartitionIterator topic_partition_iter_;
  RecursiveMessageSetIterator message_iter_;
};

}  // namespace detail
}  // namespace libkafka_asio

#endif  // FETCH_RESPONSE_ITERATOR_H_6E567149_2564_4FF2_BCC4_F0205CB5C344
