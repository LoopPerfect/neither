#include <iostream>
#include <neither/neither.hpp>
#include <string>
#include <memory>

#include <gtest/gtest.h>
#include <neither/either.hpp>

using namespace neither;
using namespace std::literals::string_literals;

using IntOrStr = Either<int, std::string>;
using StrOrInt = Either<std::string, int>;
using StrOrStr = Either<std::string, std::string>;

TEST(neither, either_join_left) {

  auto i = IntOrStr::leftOf(21);

  auto i2 = i.leftMap([](auto x) { return x * 2; })
                .rightMap([](auto x) { return x.size(); })
                .join();

  ASSERT_TRUE(i2 == 42);
}

TEST(neither, either_join_right) {

  auto s = IntOrStr::rightOf("foo");

  auto i2 = s.leftMap([](auto x) { return x * 2; })
                .rightMap([](auto x) { return x.size(); })
                .rightMap([](auto x) { return x * 10 + 12; })
                .join();

  ASSERT_TRUE(i2 == 42);
}


TEST(neither, either_leftFlatMap) {
  auto s = IntOrStr::leftOf(1);

  auto s2 = s.rightMap([](auto) -> std::string { return "b"; })
    .leftFlatMap([](auto x) { return StrOrStr::leftOf("a"); })
    .join();

  ASSERT_TRUE(s2[0] == 'a');
}

TEST(neither, either_rightFlatMap) {
  auto s = StrOrStr::rightOf("a");

  auto i = s.rightFlatMap([](auto x) { return StrOrInt::rightOf(2); })
    .leftMap([](auto) { return 1; })
    .join();

  ASSERT_TRUE(i == 2);
}

TEST(neither, either_leftMapMove) {
   neither::Either<int, std::string> e = neither::left(1);
   e.leftMap([](auto x) {
     return x;
   }).leftMap([](auto&& x) {
     return 2;
   });
}

TEST(neither, either_rightMapMove) {
   neither::Either<std::string, int> e = neither::right(1);
   e.rightMap([](auto x) {
     return x;
   }).rightMap([](auto&& x) {
     return 2;
   });
}

TEST(neither, either_constructFromUnique) {
   neither::Either<std::unique_ptr<int>, std::string> e =
     neither::left(std::make_unique<int>(1));
}

TEST(neither, either_flatMapToUnique) {
  neither::Either<int, int> e = left(1);

  auto i = e.leftFlatMap([](auto x) {
    return Either<std::unique_ptr<int>, int>::leftOf(
      std::make_unique<int>(x));
  }).leftFlatMap([](auto&& x) {
    return Either<std::unique_ptr<int>, int>::leftOf(
      std::make_unique<int>(1));
  }).leftMap([](auto&& x) {
    return *x;
  }).join();

  ASSERT_TRUE(i == 1);
}


TEST(neither, either_leftMapToUnique) {
  neither::Either<int, int> e = left(1);

  auto u = e.leftFlatMap([](auto x) { return
    Either<std::unique_ptr<int>, int>::leftOf(
      std::make_unique<int>(x));
  }).leftMap([](auto&& x) {
    return std::move(x);
  }).rightFlatMap([](auto&& x) {
    return Either<std::unique_ptr<int>, std::unique_ptr<int>>::rightOf(
      std::make_unique<int>(2));
  }).join();

  ASSERT_TRUE(*u == 1);
}


TEST(neither, either_rightMapToUnique) {
  neither::Either<int, int> e = right(1);

  auto u = e.rightFlatMap([](auto x) { return
    Either<int, std::unique_ptr<int>>::rightOf(
      std::make_unique<int>(x));
  }).rightMap([](auto&& x) {
    return std::move(x);
  }).leftFlatMap([](auto&& x) {
    return Either<std::unique_ptr<int>, std::unique_ptr<int>>::leftOf(
      std::make_unique<int>(2));
  }).join();

  ASSERT_TRUE(*u == 1);
}
