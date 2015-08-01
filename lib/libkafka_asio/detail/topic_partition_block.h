//
// detail/topic_partition_block.h
// ------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef TOPIC_PARTITION_BLOCK_H_6C26878D_A066_464C_AFAE_6C334E046EC8
#define TOPIC_PARTITION_BLOCK_H_6C26878D_A066_464C_AFAE_6C334E046EC8

#include <vector>
#include <boost/optional.hpp>
#include <libkafka_asio/primitives.h>

namespace libkafka_asio
{
namespace detail
{

// Default Properties (empty)
struct EmptyProperties
{
};

// Abstraction of a topic-partitions data structure.
// Use the template parameters to add more properties to the partition or
// topic.
template<
  typename TPartitionProperties = EmptyProperties,
  typename TTopicProperties = EmptyProperties>
struct TopicPartitionBlock :
  public TTopicProperties
{
  struct Partition :
    public TPartitionProperties
  {
    typedef boost::optional<Partition> OptionalType;
    Int32 partition;
  };

  typedef Partition PartitionType;
  typedef TopicPartitionBlock<
    TPartitionProperties, TTopicProperties
  > TopicType;
  typedef std::vector<TopicType> VectorType;
  typedef boost::optional<
    TopicPartitionBlock<TPartitionProperties, TTopicProperties>
  > OptionalType;
  typedef std::vector<Partition> PartitionVector;

  String topic_name;
  PartitionVector partitions;
};

}  // namespace detail
}  // namespace libkafka_asio

#endif  // TOPIC_PARTITION_BLOCK_H_6C26878D_A066_464C_AFAE_6C334E046EC8
