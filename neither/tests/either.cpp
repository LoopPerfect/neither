#include <gtest/gtest.h>
#include <iostream>
#include <neither/either.hpp>
#include <string>

using namespace neither;
using namespace std::literals::string_literals;
using IntOrStr = Either<int, std::string>;
using StrOrInt = Either<std::string, int>;
using StrOrStr = Either<std::string, std::string>;


TEST(neither, join_left) {

  auto i = IntOrStr::leftOf(21);

  auto i2 = i.leftMap([](auto x) { return x * 2; })
                .rightMap([](auto x) { return x.size(); })
                .join();

  ASSERT_TRUE(i2 == 42);
}

TEST(neither, join_right) {

  auto s = IntOrStr::rightOf("foo");

  auto i2 = s.leftMap([](auto x) { return x * 2; })
                .rightMap([](auto x) { return x.size(); })
                .rightMap([](auto x) { return x * 10 + 12; })
                .join();

  ASSERT_TRUE(i2 == 42);
}


TEST(neither, leftFlatMap) {
  auto s = IntOrStr::leftOf(1);

  auto s2 = s.rightMap([](auto) -> std::string { return "b"; })
    .leftFlatMap([](auto x) { return StrOrStr::leftOf("a"); })
    .join();

  ASSERT_TRUE(s2[0] == 'a');
}

TEST(neither, rightFlatMap) {
  auto s = StrOrStr::rightOf("a");

  auto i = s.rightFlatMap([](auto x) { return StrOrInt::rightOf(2); })
    .leftMap([](auto) { return 1; })
    .join();

  ASSERT_TRUE(i == 2);
}
