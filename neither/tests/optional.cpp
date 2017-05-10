#include <neither/optional.hpp>
#include <gtest/gtest.h>


using namespace neither;

TEST(neither, optional_with_value) {
  auto x = optional<int>(42);
  ASSERT_TRUE(x.get(0) == 42);
}


TEST(neither, optional_without_value) {
  auto x = optional<int>();
  ASSERT_TRUE(x.get(0) == 0);
}

TEST(neither, optional_map_value) {
  auto x = optional<int>(4);
  ASSERT_TRUE(
    x.map([](auto x){ return 4*10+2;}).get(0) == 42
  );
}

TEST(neither, optional_flatMap_value) {
  auto x = optional<int>(0);
  ASSERT_TRUE(
    x.flatMap([](auto x){ return optional(42);}).get(0) == 42
  );
}

TEST(neither, optional_flatMap_no_value) {
  auto x = optional<int>(42);
  ASSERT_TRUE(
    x.flatMap([](auto x){ return optional<int>();}).get(1) == 1
  );
}


