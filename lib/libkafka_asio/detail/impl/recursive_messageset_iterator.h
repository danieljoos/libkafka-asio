//
// detail/impl/recursive_messageset_iterator.h
// -------------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef RECURSIVE_MESSAGESET_ITERATOR_H_8741F430_DE2C_4CD9_9766_854C493DB115
#define RECURSIVE_MESSAGESET_ITERATOR_H_8741F430_DE2C_4CD9_9766_854C493DB115

namespace libkafka_asio
{
namespace detail
{

inline RecursiveMessageSetIterator::RecursiveMessageSetIterator() :
  message_set_(NULL)
{
}

inline RecursiveMessageSetIterator::RecursiveMessageSetIterator(
  const MessageSet& message_set) :
  message_set_(&message_set)
{
  outer_iterator_ = message_set_->begin();
  outer_end_iterator_ = message_set_->end();
  if (outer_iterator_ == outer_end_iterator_)
  {
    Reset();
  }
  else if (!outer_iterator_->nested_message_set().empty())
  {
    inner_iterator_.reset(
      new InnerIteratorType::element_type(
        outer_iterator_->nested_message_set()));
  }
}

inline RecursiveMessageSetIterator::RecursiveMessageSetIterator(
  const RecursiveMessageSetIterator& orig) :
  message_set_(orig.message_set_),
  outer_iterator_(orig.outer_iterator_),
  outer_end_iterator_(orig.outer_end_iterator_),
  inner_iterator_()
{
  if (orig.inner_iterator_)
  {
    inner_iterator_.reset(
      new InnerIteratorType::element_type(*orig.inner_iterator_));
  }
}

inline RecursiveMessageSetIterator& RecursiveMessageSetIterator::operator=(
  const RecursiveMessageSetIterator& rhs)
{
  message_set_ = rhs.message_set_;
  outer_iterator_ = rhs.outer_iterator_;
  outer_end_iterator_ = rhs.outer_end_iterator_;
  if (rhs.inner_iterator_)
  {
    inner_iterator_.reset(
      new InnerIteratorType::element_type(*rhs.inner_iterator_));
  }
  else
  {
    inner_iterator_.reset();
  }
  return *this;
}

inline void RecursiveMessageSetIterator::increment()
{
  if (!message_set_)
  {
    return;
  }
  if (!IsInnerDone())
  {
    // First increase the inner-iterator (if it is still valid)
    ++(*inner_iterator_);
  }

  if (IsInnerDone())
  {
    // If the inner iterator is at the end (or there is no nested message set)
    // then increase the outer iterator
    ++outer_iterator_;
    if (outer_iterator_ == outer_end_iterator_)
    {
      // If the outer iterator reached the end of the message set, we're done
      Reset();
      return;
    }
    if (!outer_iterator_->nested_message_set().empty())
    {
      // Create a new inner iterator, if the new outer has a nested message set
      inner_iterator_.reset(
        new InnerIteratorType::element_type(
          outer_iterator_->nested_message_set()));
    }
  }
}

inline bool RecursiveMessageSetIterator::equal(
  const RecursiveMessageSetIterator& other) const
{
  if (message_set_ && other.message_set_)
  {
    if ((inner_iterator_ && other.inner_iterator_))
    {
      return (message_set_ == other.message_set_) &&
             (message_set_ != NULL) &&
             (outer_iterator_ == other.outer_iterator_) &&
             (*inner_iterator_ == *other.inner_iterator_);
    }
    return (message_set_ == other.message_set_) &&
           (message_set_ != NULL) &&
           (outer_iterator_ == other.outer_iterator_);
  }
  return (message_set_ == other.message_set_);
}

inline const MessageAndOffset& RecursiveMessageSetIterator::dereference() const
{
  if (!IsInnerDone())
  {
    return **inner_iterator_;
  }
  else
  {
    return *outer_iterator_;
  }
}

inline void RecursiveMessageSetIterator::Reset()
{
  message_set_ = NULL;
  inner_iterator_.reset();
}

inline bool RecursiveMessageSetIterator::IsInnerDone() const
{
  return !inner_iterator_ || !inner_iterator_->message_set_;
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // RECURSIVE_MESSAGESET_ITERATOR_H_8741F430_DE2C_4CD9_9766_854C493DB115
