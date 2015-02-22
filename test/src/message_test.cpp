//
// message_test.cpp
// ----------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#include <string>
#include <gtest/gtest.h>
#include <libkafka_asio/libkafka_asio.h>

using libkafka_asio::Message;
using libkafka_asio::Bytes;

TEST(MessageTest, FlatCopy)
{
  Message orig;
  std::string test_value = "foo bar";
  orig.mutable_value().reset(
    new Bytes::element_type(test_value.begin(), test_value.end()));
  Message copy(orig, false);
  ASSERT_TRUE(orig.value());
  ASSERT_TRUE(copy.value());
  // Both messages should point to the same value buffer
  ASSERT_EQ(orig.value().get(), copy.value().get());
}

TEST(MessageTest, AssignmentOperator)
{
  Message orig;
  std::string test_value = "foo bar";
  orig.mutable_value().reset(
    new Bytes::element_type(test_value.begin(), test_value.end()));
  Message copy;
  copy = orig;
  ASSERT_TRUE(orig.value());
  ASSERT_TRUE(copy.value());
  // Both messages should point to the same value buffer
  ASSERT_EQ(orig.value().get(), copy.value().get());
}

TEST(MessageTest, DeepCopy)
{
  Message orig;
  {
    std::string test_value = "foo bar";
    orig.mutable_value().reset(
      new Bytes::element_type(test_value.begin(), test_value.end()));
  }
  Message copy(orig, true);
  ASSERT_TRUE(orig.value());
  ASSERT_TRUE(copy.value());
  // Each message should now have it's own value buffer
  ASSERT_NE(orig.value().get(), copy.value().get());
  // But the actual value should be the same (a copy)
  ASSERT_FALSE(orig.value()->empty());
  ASSERT_FALSE(copy.value()->empty());
  std::string test_value1((const char*)&(*orig.value())[0],
                          orig.value()->size());
  std::string test_value2((const char*)&(*copy.value())[0],
                          copy.value()->size());
  ASSERT_STREQ(test_value1.c_str(), test_value2.c_str());
}
