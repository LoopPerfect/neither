#include <neither/neither.hpp>
#include <gtest/gtest.h>

TEST(neither, lift_maybes) {

  using namespace neither;

  auto maybeSum = lift([](auto x, auto y){ return x+y;});

  auto sum1 = maybeSum(
    maybe(5),
    maybe(7)
  );

  auto sum2 = maybeSum(
    maybe<int>(),
    maybe(7)
  );


  ASSERT_TRUE(sum1.hasValue && sum1.value == 12);
  ASSERT_TRUE(!sum2.hasValue);
}
