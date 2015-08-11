//
// detail/topic_partition_map.h
// ------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef TOPIC_PARTITION_MAP_H_DD4432F2_3C40_11E5_A151_FEFF819CDC9F
#define TOPIC_PARTITION_MAP_H_DD4432F2_3C40_11E5_A151_FEFF819CDC9F

#include <vector>
#include <libkafka_asio/primitives.h>
#include <libkafka_asio/detail/empty_property.h>

namespace libkafka_asio
{
namespace detail
{

// Abstraction of a topic-partitions data structure.
// Use the template parameters to add more properties to the partition or
// topic.
template<
  typename TPartitionProperties = EmptyProperties,
  typename TTopicProperties = EmptyProperties>
struct TopicPartitionMap :
  public TTopicProperties
{
  struct Partition :
    public TPartitionProperties
  {
    typedef boost::optional<Partition> OptionalType;
  };

  typedef Partition PartitionType;
  typedef TopicPartitionMap<TPartitionProperties,
                              TTopicProperties> TopicType;
  typedef std::map<String, TopicType> MapType;
  typedef boost::optional<
    TopicPartitionMap<TPartitionProperties, TTopicProperties>
  > OptionalType;
  typedef std::map<Int32, Partition> PartitionMap;

  PartitionMap partitions;
};

}  // namespace detail
}  // namespace libkafka_asio

#endif  // TOPIC_PARTITION_MAP_H_DD4432F2_3C40_11E5_A151_FEFF819CDC9F
