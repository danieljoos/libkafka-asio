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
  test_response_builder.mutable_topics().resize(1);
  FetchResponse::Topic& test_topic = test_response_builder.mutable_topics()[0];
  test_topic.partitions.resize(1);
  FetchResponse::TopicPartition& test_partition = test_topic.partitions[0];
  test_partition.messages.resize(3);
  test_partition.messages[0].set_offset(1);
  test_partition.messages[1].set_offset(2);
  test_partition.messages[2].set_offset(3);
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
  test_response_builder.mutable_topics().resize(1);
  FetchResponse::Topic& test_topic = test_response_builder.mutable_topics()[0];
  test_topic.partitions.resize(3);
  FetchResponse::TopicPartition& test_partition1 = test_topic.partitions[0];
  FetchResponse::TopicPartition& test_partition2 = test_topic.partitions[1];
  FetchResponse::TopicPartition& test_partition3 = test_topic.partitions[2];
  test_partition1.partition = 1;
  test_partition1.messages.resize(1);
  test_partition1.messages[0].set_offset(1);
  test_partition2.partition = 2;
  test_partition2.messages.resize(1);
  test_partition2.messages[0].set_offset(2);
  test_partition3.partition = 3;
  test_partition3.messages.resize(1);
  test_partition3.messages[0].set_offset(3);
  const FetchResponse& test_response = test_response_builder.response();
  ASSERT_EQ(3, std::distance(test_response.begin(), test_response.end()));
  FetchResponse::const_iterator iterator = test_response.begin();
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(1, iterator->offset());
  ASSERT_EQ(1, iterator.topic_partition().partition);
  ++iterator;
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(2, iterator->offset());
  ASSERT_EQ(2, iterator.topic_partition().partition);
  ++iterator;
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(3, iterator->offset());
  ASSERT_EQ(3, iterator.topic_partition().partition);
  ++iterator;
  ASSERT_EQ(iterator, test_response.end());
}

TEST(FetchResponseIteratorTest, MultipleTopics)
{
  MutableFetchResponse test_response_builder;
  test_response_builder.mutable_topics().resize(3);
  FetchResponse::Topic& test_topic1 = test_response_builder.mutable_topics()[0];
  FetchResponse::Topic& test_topic2 = test_response_builder.mutable_topics()[1];
  FetchResponse::Topic& test_topic3 = test_response_builder.mutable_topics()[2];
  test_topic1.partitions.resize(1);
  test_topic1.topic_name = "Topic1";
  test_topic2.partitions.resize(1);
  test_topic2.topic_name = "Topic2";
  test_topic3.partitions.resize(1);
  test_topic3.topic_name = "Topic3";
  FetchResponse::TopicPartition& test_partition1 = test_topic1.partitions[0];
  FetchResponse::TopicPartition& test_partition2 = test_topic2.partitions[0];
  FetchResponse::TopicPartition& test_partition3 = test_topic3.partitions[0];
  test_partition1.partition = 1;
  test_partition1.messages.resize(1);
  test_partition1.messages[0].set_offset(1);
  test_partition2.partition = 2;
  test_partition2.messages.resize(1);
  test_partition2.messages[0].set_offset(2);
  test_partition3.partition = 3;
  test_partition3.messages.resize(1);
  test_partition3.messages[0].set_offset(3);
  const FetchResponse& test_response = test_response_builder.response();
  ASSERT_EQ(3, std::distance(test_response.begin(), test_response.end()));
  FetchResponse::const_iterator iterator = test_response.begin();
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(1, iterator->offset());
  ASSERT_EQ(1, iterator.topic_partition().partition);
  ASSERT_STREQ("Topic1", iterator.topic().topic_name.c_str());
  ++iterator;
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(2, iterator->offset());
  ASSERT_EQ(2, iterator.topic_partition().partition);
  ASSERT_STREQ("Topic2", iterator.topic().topic_name.c_str());
  ++iterator;
  ASSERT_NE(iterator, test_response.end());
  ASSERT_EQ(3, iterator->offset());
  ASSERT_EQ(3, iterator.topic_partition().partition);
  ASSERT_STREQ("Topic3", iterator.topic().topic_name.c_str());
  ++iterator;
  ASSERT_EQ(iterator, test_response.end());
}