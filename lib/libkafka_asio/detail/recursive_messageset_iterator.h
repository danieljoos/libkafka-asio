//
// detail/recursive_messageset_iterator.h
// --------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef RECURSIVE_MESSAGESET_ITERATOR_H_FDB00A65_7998_46B4_AF03_CCBB481582E2
#define RECURSIVE_MESSAGESET_ITERATOR_H_FDB00A65_7998_46B4_AF03_CCBB481582E2

#include <boost/iterator/iterator_facade.hpp>
#include <boost/scoped_ptr.hpp>
#include <libkafka_asio/message.h>

namespace libkafka_asio
{
namespace detail
{

class RecursiveMessageSetIterator :
  public boost::iterator_facade<
    RecursiveMessageSetIterator,
    const MessageAndOffset,
    boost::forward_traversal_tag
  >
{
public:

  // Create a new empty and uninitialized iterator. Such objects can be used
  // as end-iterator.
  RecursiveMessageSetIterator();

  // Create a new object, recursively iterating over the given MessageSet.
  // The MessageSet MUST always have a longer lifetime than the iterator object.
  explicit RecursiveMessageSetIterator(const MessageSet& message_set);

  // Copy the given iterator object. The resulting iterator will iterate over
  // the same MessageSet. The outer- as well as the inner-iterator will be
  // copied as well. So one can continue to work with the original iterator
  // object without affecting the new one and vice-versa.
  RecursiveMessageSetIterator(const RecursiveMessageSetIterator& orig);

  // Assign the given iterator to this one by copying it. The same rules apply
  // as for the copy-constructor above.
  RecursiveMessageSetIterator& operator=(
    const RecursiveMessageSetIterator& rhs);

private:
  friend class boost::iterator_core_access;

  typedef boost::scoped_ptr<RecursiveMessageSetIterator> InnerIteratorType;

  void increment();

  bool equal(const RecursiveMessageSetIterator& other) const;

  const MessageAndOffset& dereference() const;

  void Reset();

  bool IsInnerDone() const;

  // Pointer to the MessageSet we're iterating through
  const MessageSet *message_set_;

  // Outer iterators: Normal iterators on the given MessageSet
  MessageSet::const_iterator outer_iterator_;
  MessageSet::const_iterator outer_end_iterator_;

  // Inner iterator: If the Message, currently pointed to by the outer iterator
  // has a nested MessageSet, the inner iterator is used to walk through it.
  InnerIteratorType inner_iterator_;
};

}  // namespace detail
}  // namespace libkafka_asio

#include <libkafka_asio/detail/impl/recursive_messageset_iterator.h>

#endif  // RECURSIVE_MESSAGESET_ITERATOR_H_FDB00A65_7998_46B4_AF03_CCBB481582E2
