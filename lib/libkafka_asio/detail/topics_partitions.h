//
// detail/topics_partitions.h
// --------------------------
//
// Copyright (c) 2015-2016 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef TOPICS_PARTITIONS_H_45ED10ED_C3C9_428E_9468_FF1412DB2D22
#define TOPICS_PARTITIONS_H_45ED10ED_C3C9_428E_9468_FF1412DB2D22

namespace libkafka_asio
{
namespace detail
{

// Basic `Topic` structure that consists of a bunch of partitions.
// Can be extended by specifying a properties structure.
template< typename TProperties, typename TPartitions >
struct Topic : TProperties
{
  typedef boost::optional< Topic<TProperties, TPartitions> > OptionalType;
  TPartitions partitions;
};

// Basic `Partition` structure.
// Can be extended by specifying a properties structure.
template< typename TProperties >
struct Partition : TProperties
{
  typedef boost::optional< Partition<TProperties> > OptionalType;
};

// Used by the `TopicsPartitionsVector` to add a `topic_name` property to
// each `Topic`.
template< typename TProperties >
struct VectorTopicProperties : TProperties
{
  String topic_name;
};

// Used by the `TopicsPartitionsVector` to add a `partition` property to
// each `Partition`.
template< typename TProperties >
struct VectorPartitionProperties : TProperties
{
  Int32 partition;
};

// Empty properties structure. Use this in case no properties should be added
// to a certain data structure.
struct EmptyProperties
{
};

//
// Topic-partition data structure using two levels of nested maps. On the top
// level, a `Topic` data structure is mapped to a topic name (string). Each of
// the `Topic` objects contains a bunch of `Partition` objects, mapped to the
// partition id/number.
// Extend the Topic and/or Partition structures by specifying the two template
// parameters.
// The `Type` type definition defines the actual map type.
//
// Example:
// ```
//    struct MyPartitionProps { String info; };
//    typedef TopicsPartitionsMap<
//        EmptyProperties,      // Properties for `Topic`
//        MyPartitionProps      // Properties for `Partition`
//    > MyTopicsPartitionsMap;
//
//    MyTopicsPartitionsMap::Topics my_map;
//    my_map["testtopic"].partitions[0].info = "Hello World";
// ```
//
template<typename TTopicProperties, typename TPartitionProperties>
struct TopicsPartitionsMap
{
  typedef Partition<TPartitionProperties> PartitionType;
  typedef std::map<Int32, PartitionType> PartitionsType;
  typedef Topic<TTopicProperties, PartitionsType> TopicType;
  typedef std::map<String, TopicType> TopicsType;
};

//
// Topic partition data structure using two levels of nested vectors. The top
// level vector contains one or more `Topic` objects, each identified by a
// `topic_name` property (string). Each of the `Topic` objects contains a bunch
// of `Partition` objects, which themselve are identified by a `partition`
// property (int).
// Extend the Topic and/or Partition structures by specifying the two template
// parameters.
// The `Type` type definition defines the actual vector type.
//
// Example:
// ```
//    struct MyTopicProps { Int32 somenumber; };
//    struct MyPartitionProps { String info; };
//    typedef TopicsPartitionsVector<
//        MyTopicProps,         // Properties for `Topic`
//        MyPartitionProps      // Properties for `Partition`
//    > MyTopicsPartitionsVector;
//
//    MyTopicsPartitionsVector::Topics my_vector(1);
//    my_vector[0].topic_name = "test_topic";
//    my_vector[0].somenumber = 123;
//    my_vector[0].partitions.resize(1);
//    my_vector[0].partitions[0].partition = 1;
//    my_vector[0].partitions[0].info = "Hello World";
// ```
//
template<typename TTopicProperties, typename TPartitionProperties>
struct TopicsPartitionsVector
{
  typedef Partition<VectorPartitionProperties<TPartitionProperties> > PartitionType;
  typedef std::vector<PartitionType> PartitionsType;
  typedef Topic<VectorTopicProperties<TTopicProperties>, PartitionsType> TopicType;
  typedef std::vector<TopicType> TopicsType;
};

}  // namespace detail
}  // namespace libkafka_asio

#endif  // TOPICS_PARTITIONS_H_45ED10ED_C3C9_428E_9468_FF1412DB2D22
