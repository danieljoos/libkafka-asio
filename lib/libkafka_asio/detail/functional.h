//
// detail/functional.h
// -------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef FUNCTIONAL_H_F880D753_2C9F_4CD5_B08D_588BC8013C36
#define FUNCTIONAL_H_F880D753_2C9F_4CD5_B08D_588BC8013C36

#include <functional>

namespace libkafka_asio
{
namespace detail
{

template<typename TTopic>
class IsTopicWithName :
  std::unary_function<TTopic, bool>
{
  typedef IsTopicWithName<TTopic> TMy;
public:
  IsTopicWithName(const String& name) :
    name_(name)
  {
  }

  typename TMy::result_type operator()(const typename TMy::argument_type& other)
  {
    return other.topic_name == name_;
  }

private:
  String name_;
};

template<typename TTopicPartition>
class IsTopicPartition :
  std::unary_function<TTopicPartition, bool>
{
  typedef IsTopicPartition<TTopicPartition> TMy;
public:
  IsTopicPartition(Int32 partition) :
    partition_(partition)
  {
  }

  typename TMy::result_type operator()(const typename TMy::argument_type& other)
  {
    return other.partition == partition_;
  }

private:
  Int32 partition_;
};

template<typename TBroker>
class IsBrokerWithId :
  std::unary_function<TBroker, bool>
{
  typedef IsBrokerWithId<TBroker> TMy;
public:
  IsBrokerWithId(Int32 node_id) :
    node_id_(node_id)
  {
  }

  typename TMy::result_type operator()(const typename TMy::argument_type& other)
  {
    return other.node_id == node_id_;
  }

private:
  Int32 node_id_;
};

// Find an entry with 'topic_name' set to the given topic name inside the
// given const container.
template<typename TContainer>
typename TContainer::const_iterator FindTopicByName(
  const String& topic_name,
  const TContainer& container)
{
  typedef typename TContainer::value_type TTopic;
  typename TContainer::const_iterator result =
    std::find_if(container.begin(), container.end(),
                 IsTopicWithName<TTopic>(topic_name));
  return result;
}

// Find or create an entry with 'topic_name' set to the given topic name inside
// the given mutual container.
template<typename TContainer>
typename TContainer::iterator FindTopicByName(
  const String& topic_name,
  TContainer& container,
  bool create = true)
{
  typedef typename TContainer::value_type TTopic;
  typename TContainer::iterator result =
    std::find_if(container.begin(), container.end(),
                 IsTopicWithName<TTopic>(topic_name));
  if (create && result == container.end())
  {
    result = container.insert(container.end(), TTopic());
    result->topic_name = topic_name;
  }
  return result;
}

// Find an entry with 'partition' set to the given partition number inside the
// given const container.
template<typename TContainer>
typename TContainer::const_iterator FindTopicPartitionByNumber(
  Int32 partition,
  const TContainer& container)
{
  typedef typename TContainer::value_type TPartition;
  typename TContainer::const_iterator result =
    std::find_if(container.begin(), container.end(),
                 IsTopicPartition<TPartition>(partition));
  return result;
}

// Find or create an entry with 'partition' set to the given partition number
// inside the given mutual container.
template<typename TContainer>
typename TContainer::iterator FindTopicPartitionByNumber(
  Int32 partition,
  TContainer& container,
  bool create = true)
{
  typedef typename TContainer::value_type TPartition;
  typename TContainer::iterator result =
    std::find_if(container.begin(), container.end(),
                 IsTopicPartition<TPartition>(partition));
  if (create && result == container.end())
  {
    result = container.insert(container.end(), TPartition());
    result->partition = partition;
  }
  return result;
}

// Find an entry with 'node_id' set to the given broker node id inside the
// given const container.
template<typename TContainer>
typename TContainer::const_iterator FindBrokerById(
  Int32 node_id,
  const TContainer& container)
{
  typedef typename TContainer::value_type TBroker;
  typename TContainer::const_iterator result =
    std::find_if(container.begin(), container.end(),
                 IsBrokerWithId<TBroker>(node_id));
  return result;
}

}  // namespace detail
}  // namespace libkafka_asio

#endif  // FUNCTIONAL_H_F880D753_2C9F_4CD5_B08D_588BC8013C36
