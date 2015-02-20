//
// detail/functional_test.cpp
// --------------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <vector>
#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

using namespace libkafka_asio;
using namespace libkafka_asio::detail;

class FunctionalTest :
  public ::testing::Test
{
protected:
  struct TestTopic
  {
    String topic_name;

    TestTopic()
    {
    }

    TestTopic(const String& name) :
      topic_name(name)
    {
    }
  };

  struct TestPartition
  {
    Int32 partition;

    TestPartition()
    {
    }

    TestPartition(Int32 p) :
      partition(p)
    {
    }
  };

  typedef std::vector<TestTopic> TestTopicVector;
  typedef std::vector<TestPartition> TestPartitionVector;
};

TEST_F(FunctionalTest, FindTopicByNameConst)
{
  TestTopicVector test_topics;
  test_topics.push_back(TestTopic("hello"));
  test_topics.push_back(TestTopic("world"));
  test_topics.push_back(TestTopic("foo"));
  test_topics.push_back(TestTopic("bar"));
  ASSERT_EQ(4, test_topics.size());
  const TestTopicVector& const_test_topics = test_topics;
  TestTopicVector::const_iterator result;
  result = FindTopicByName("foo", const_test_topics);
  ASSERT_NE(result, const_test_topics.end());
  result = FindTopicByName("test", const_test_topics);
  ASSERT_EQ(result, const_test_topics.end());
  ASSERT_EQ(4, test_topics.size());
}

TEST_F(FunctionalTest, FindTopicByName)
{
  TestTopicVector test_topics;
  test_topics.push_back(TestTopic("hello"));
  test_topics.push_back(TestTopic("world"));
  test_topics.push_back(TestTopic("foo"));
  test_topics.push_back(TestTopic("bar"));
  ASSERT_EQ(4, test_topics.size());
  TestTopicVector::iterator result;
  result = FindTopicByName("foo", test_topics);
  ASSERT_NE(result, test_topics.end());
  result = FindTopicByName("test", test_topics);
  ASSERT_NE(result, test_topics.end());
  result = FindTopicByName("yetanother", test_topics, false);
  ASSERT_EQ(result, test_topics.end());
  ASSERT_EQ(5, test_topics.size());
}

TEST_F(FunctionalTest, FindTopicPartitionByNumberConst)
{
  TestPartitionVector test_partitions;
  test_partitions.push_back(TestPartition(1337));
  test_partitions.push_back(TestPartition(42));
  test_partitions.push_back(TestPartition(1));
  test_partitions.push_back(TestPartition(2));
  ASSERT_EQ(4, test_partitions.size());
  const TestPartitionVector& const_test_partitions = test_partitions;
  TestPartitionVector::const_iterator result;
  result = FindTopicPartitionByNumber(2, const_test_partitions);
  ASSERT_NE(result, const_test_partitions.end());
  result = FindTopicPartitionByNumber(9000, const_test_partitions);
  ASSERT_EQ(result, const_test_partitions.end());
  ASSERT_EQ(4, test_partitions.size());
}

TEST_F(FunctionalTest, FindTopicPartitionByNumber)
{
  TestPartitionVector test_partitions;
  test_partitions.push_back(TestPartition(1337));
  test_partitions.push_back(TestPartition(42));
  test_partitions.push_back(TestPartition(1));
  test_partitions.push_back(TestPartition(2));
  ASSERT_EQ(4, test_partitions.size());
  TestPartitionVector::iterator result;
  result = FindTopicPartitionByNumber(2, test_partitions);
  ASSERT_NE(result, test_partitions.end());
  result = FindTopicPartitionByNumber(9000, test_partitions);
  ASSERT_NE(result, test_partitions.end());
  result = FindTopicPartitionByNumber(1234, test_partitions, false);
  ASSERT_EQ(result, test_partitions.end());
  ASSERT_EQ(5, test_partitions.size());
}