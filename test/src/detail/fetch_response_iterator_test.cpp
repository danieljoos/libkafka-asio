//
// detail/fetch_response_iterator_test.cpp
// ---------------------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

using namespace libkafka_asio;
using namespace libkafka_asio::detail;

TEST(FetchResponseIteratorTest, Empty)
{
  FetchResponse test_response;
  ASSERT_EQ(0, std::distance(test_response.begin(), test_response.end()));
  FetchResponse::const_iterator iterator = test_response.begin();
  ASSERT_EQ(test_response.end(), iterator);
  ++iterator;
  ASSERT_EQ(test_response.end(), iterator);
}

TEST(FetchResponseIteratorTest, MultiplePartitionMessages)
{
  MutableFetchResponse test_response_builder;
  FetchResponse::Topic test_topic;
  FetchResponse::Topic::Partition test_partition;
  test_partition.messages.resize(3);
  test_partition.messages[0].set_offset(1);
  test_partition.messages[1].set_offset(2);
  test_partition.messages[2].set_offset(3);
  test_topic.partitions.insert(std::make_pair(0, test_partition));
  test_response_builder.mutable_topics().insert(
    std::make_pair("test", test_topic));
  const FetchResponse& test_response = test_response_builder.response();
  ASSERT_EQ(3, std::distance(test_response.begin(), test_response.end()));
  FetchResponse::const_iterator iterator = test_response.begin();
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(1, iterator->offset());
  ++iterator;
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(2, iterator->offset());
  ++iterator;
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(3, iterator->offset());
  ++iterator;
  ASSERT_EQ(iterator, test_response.end());
}

TEST(FetchResponseIteratorTest, MultiplePartitions)
{
  MutableFetchResponse test_response_builder;
  FetchResponse::Topic test_topic;
  FetchResponse::Topic::Partition test_partition1;
  FetchResponse::Topic::Partition test_partition2;
  FetchResponse::Topic::Partition test_partition3;
  test_partition1.messages.resize(1);
  test_partition1.messages[0].set_offset(1);
  test_partition2.messages.resize(1);
  test_partition2.messages[0].set_offset(2);
  test_partition3.messages.resize(1);
  test_partition3.messages[0].set_offset(3);
  test_topic.partitions.insert(std::make_pair(1, test_partition1));
  test_topic.partitions.insert(std::make_pair(2, test_partition2));
  test_topic.partitions.insert(std::make_pair(3, test_partition3));
  test_response_builder.mutable_topics().insert(
    std::make_pair("test", test_topic));
  const FetchResponse& test_response = test_response_builder.response();
  ASSERT_EQ(3, std::distance(test_response.begin(), test_response.end()));
  FetchResponse::const_iterator iterator = test_response.begin();
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(1, iterator->offset());
  ASSERT_EQ(1, iterator.topic_partition().first);
  ++iterator;
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(2, iterator->offset());
  ASSERT_EQ(2, iterator.topic_partition().first);
  ++iterator;
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(3, iterator->offset());
  ASSERT_EQ(3, iterator.topic_partition().first);
  ++iterator;
  ASSERT_EQ(iterator, test_response.end());
}

TEST(FetchResponseIteratorTest, MultipleTopics)
{
  MutableFetchResponse test_response_builder;
  FetchResponse::Topic test_topic1;
  FetchResponse::Topic test_topic2;
  FetchResponse::Topic test_topic3;
  FetchResponse::Topic::Partition test_partition1;
  FetchResponse::Topic::Partition test_partition2;
  FetchResponse::Topic::Partition test_partition3;
  test_partition1.messages.resize(1);
  test_partition1.messages[0].set_offset(1);
  test_partition2.messages.resize(1);
  test_partition2.messages[0].set_offset(2);
  test_partition3.messages.resize(1);
  test_partition3.messages[0].set_offset(3);
  test_topic1.partitions.insert(std::make_pair(1, test_partition1));
  test_topic2.partitions.insert(std::make_pair(2, test_partition2));
  test_topic3.partitions.insert(std::make_pair(3, test_partition3));
  test_response_builder.mutable_topics().insert(
    std::make_pair("Topic1", test_topic1));
  test_response_builder.mutable_topics().insert(
    std::make_pair("Topic2", test_topic2));
  test_response_builder.mutable_topics().insert(
    std::make_pair("Topic3", test_topic3));
  const FetchResponse& test_response = test_response_builder.response();
  ASSERT_EQ(3, std::distance(test_response.begin(), test_response.end()));
  FetchResponse::const_iterator iterator = test_response.begin();
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(1, iterator->offset());
  ASSERT_EQ(1, iterator.topic_partition().first);
  ASSERT_STREQ("Topic1", iterator.topic().first.c_str());
  ++iterator;
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(2, iterator->offset());
  ASSERT_EQ(2, iterator.topic_partition().first);
  ASSERT_STREQ("Topic2", iterator.topic().first.c_str());
  ++iterator;
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(3, iterator->offset());
  ASSERT_EQ(3, iterator.topic_partition().first);
  ASSERT_STREQ("Topic3", iterator.topic().first.c_str());
  ++iterator;
  ASSERT_EQ(iterator, test_response.end());
}