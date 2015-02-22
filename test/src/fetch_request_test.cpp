//
// fetch_request_test.cpp
// ----------------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

class FetchRequestTest :
  public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    ASSERT_EQ(0, request.topics().size());
  }

  libkafka_asio::FetchRequest request;
};

TEST_F(FetchRequestTest, FetchTopic_New)
{
  request.FetchTopic("mytopic", 1, 2);
  ASSERT_EQ(1, request.topics().size());
  ASSERT_EQ(1, request.topics()[0].partitions.size());
  ASSERT_STREQ("mytopic", request.topics()[0].topic_name.c_str());
  ASSERT_EQ(1, request.topics()[0].partitions[0].partition);
  ASSERT_EQ(2, request.topics()[0].partitions[0].fetch_offset);
  ASSERT_EQ(libkafka_asio::constants::kDefaultFetchMaxBytes,
            request.topics()[0].partitions[0].max_bytes);
}

TEST_F(FetchRequestTest, FetchTopic_Override)
{
  request.FetchTopic("mytopic", 1, 2);
  ASSERT_EQ(1, request.topics().size());
  ASSERT_EQ(1, request.topics()[0].partitions.size());
  ASSERT_EQ(2, request.topics()[0].partitions[0].fetch_offset);
  request.FetchTopic("mytopic", 1, 4);
  ASSERT_EQ(1, request.topics().size());
  ASSERT_EQ(1, request.topics()[0].partitions.size());
  ASSERT_EQ(4, request.topics()[0].partitions[0].fetch_offset);
}

TEST_F(FetchRequestTest, FetchTopic_MultiplePartitions)
{
  request.FetchTopic("mytopic", 0, 2);
  request.FetchTopic("mytopic", 1, 4);
  ASSERT_EQ(1, request.topics().size());
  ASSERT_EQ(2, request.topics()[0].partitions.size());
  ASSERT_EQ(2, request.topics()[0].partitions[0].fetch_offset);
  ASSERT_EQ(4, request.topics()[0].partitions[1].fetch_offset);
}

TEST_F(FetchRequestTest, FetchTopic_MultipleTopics)
{
  request.FetchTopic("foo", 0, 2);
  request.FetchTopic("bar", 1, 4);
  ASSERT_EQ(2, request.topics().size());
  ASSERT_EQ(1, request.topics()[0].partitions.size());
  ASSERT_EQ(1, request.topics()[1].partitions.size());
  ASSERT_EQ(2, request.topics()[0].partitions[0].fetch_offset);
  ASSERT_EQ(4, request.topics()[1].partitions[0].fetch_offset);
}
