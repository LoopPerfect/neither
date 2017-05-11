#include <gtest/gtest.h>
#include <neither/maybe.hpp>

using namespace neither;

TEST(neither, maybe_with_value) {
  auto x = maybe<int>(42);
  ASSERT_TRUE(x.get(0) == 42);
}

TEST(neither, maybe_without_value) {
  auto x = maybe<int>();
  ASSERT_TRUE(x.get(0) == 0);
}

TEST(neither, maybe_map_value) {
  auto x = maybe<int>(4);
  ASSERT_TRUE(x.map([](auto x) { return 4 * 10 + 2; }).get(0) == 42);
}

TEST(neither, maybe_flatMap_value) {
  auto x = maybe<int>(0);
  ASSERT_TRUE(x.flatMap([](auto x) { return maybe(42); }).get(0) == 42);
}

TEST(neither, maybe_flatMap_no_value) {
  auto x = maybe<int>(42);
  ASSERT_TRUE(x.flatMap([](auto x) { return maybe<int>(); }).get(1) == 1);
}
