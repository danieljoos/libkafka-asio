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

namespace libkafka_asio
{
namespace detail
{

// Iterator over all messages received by a Fetch API request.
template<typename TTopicVector>
class FetchResponseIterator :
  public boost::iterator_facade<
    FetchResponseIterator<TTopicVector>,
    const MessageAndOffset,
    boost::forward_traversal_tag
  >
{
  typedef FetchResponseIterator<TTopicVector> TMy;
public:
  typedef typename TTopicVector::value_type Topic;
  typedef typename Topic::PartitionVector TopicPartitionVector;
  typedef typename TopicPartitionVector::value_type TopicPartition;

  FetchResponseIterator() :
    topics_(NULL)
  {
  }

  explicit FetchResponseIterator(const TTopicVector& topics) :
    topics_(&topics)
  {
    topic_iter_ = topics_->begin();
    while (topic_iter_ != topics_->end())
    {
      topic_partition_iter_ = topic_iter_->partitions.begin();
      while (topic_partition_iter_ != topic_iter_->partitions.end())
      {
        message_iter_ = topic_partition_iter_->messages.begin();
        if (message_iter_ != topic_partition_iter_->messages.end())
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

  typedef typename TTopicVector::const_iterator TopicIterator;
  typedef typename TopicPartitionVector::const_iterator TopicPartitionIterator;
  typedef typename MessageSet::const_iterator MessageIterator;

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
    while (message_iter_ == topic_partition_iter_->messages.end())
    {
      topic_partition_iter_++;
      while (topic_partition_iter_ == topic_iter_->partitions.end())
      {
        topic_iter_++;
        if (topic_iter_ == topics_->end())
        {
          reset();
          return;
        }
        topic_partition_iter_ = topic_iter_->partitions.begin();
      }
      message_iter_ = topic_partition_iter_->messages.begin();
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

  const TTopicVector *topics_;
  TopicIterator topic_iter_;
  TopicPartitionIterator topic_partition_iter_;
  MessageIterator message_iter_;
};

}  // namespace detail
}  // namespace libkafka_asio

#endif  // FETCH_RESPONSE_ITERATOR_H_6E567149_2564_4FF2_BCC4_F0205CB5C344
