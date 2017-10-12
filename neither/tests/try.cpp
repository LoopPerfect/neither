#include <gtest/gtest.h>
#include <neither/neither.hpp>

TEST(neither, try_and_fail) {

  using namespace neither;

  auto e = Try<int>([] {
    throw 42;
    return 0;
  });

  auto result = e.join([](auto x) { return x; }, [](auto x) { return 1; });

  ASSERT_TRUE(result == 42);
  ASSERT_TRUE(e.left().hasValue);
  ASSERT_TRUE(!e.right().hasValue);
}

TEST(neither, try_and_suceed) {

  using namespace neither;

  auto e = Try<int>([] { return 42; });

  auto result = e.join([](auto x) { return 1; }, [](auto x) { return x; });

  ASSERT_TRUE(result == 42);
  ASSERT_TRUE(!e.left().hasValue);
  ASSERT_TRUE(e.right().hasValue);
}
