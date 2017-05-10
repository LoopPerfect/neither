#include <iostream>
#include <neither/either.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace neither;
using namespace std::literals::string_literals;
using IntOrStr = Either<int, std::string>;

TEST(neither, join_left) {

  auto i = IntOrStr::leftOf(21) ;

  auto i2 = i
    .leftMap([](auto x){ return x*2; })
    .rightMap([](auto x){ return x.size(); })
    .join();

  ASSERT_TRUE(i2 == 42);
}


TEST(neither, join_right) {

  auto s = IntOrStr::rightOf("ads") ;

  auto i2 = s
    .leftMap([](auto x){ return x*2; })
    .rightMap([](auto x){ return x.size(); })
    .rightMap([](auto x){ return x*10 + 12; })
    .join();

  ASSERT_TRUE(i2 == 42);
}


