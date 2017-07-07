#ifndef NEITHER_EITHER_HPP
#define NEITHER_EITHER_HPP

#include <neither/maybe.hpp>
#include <type_traits>

namespace neither {

template<class T>
constexpr T max(T x, T y) {
  return x>y ? x : y;
}

template<class T>
struct Left {
  T const value;
};

template<class T>
constexpr Left<T> left(T const& x) {
  return {x};
}

template<class T>
struct Right {
  T const value;
};

template<class T>
constexpr Right<T> right(T const& x) {
  return {x};
}


template<class L, class R>
struct Either;


template<class L, class R>
auto ensureEither ( Either<L,R> const& e) -> Either<L,R> {
  return e;
}

template<class L, class R>
auto ensureEitherRight ( Either<L,R> const& e, R) -> Either<L, R> {
  return e;
}


template<class L, class R>
auto ensureEitherLeft ( Either<L,R> const& e, L) -> Either<L, R> {
  return e;
}







template<class L, class R>
struct Either {

  union {
    L leftValue;
    R rightValue;
  };

  bool const isLeft = 0;

  constexpr Either( Left<L> const& l )
    : leftValue{l.value}
    , isLeft(1)
  {}

  constexpr Either( Right<R> const& r )
    : rightValue{r.value}
    , isLeft(0)
  {}

  constexpr Either( Either<L, R> const& e )
    : isLeft(e.isLeft) {
    if(isLeft) {
      new (&leftValue)L(e.leftValue);
    } else {
      new (&rightValue)R(e.rightValue);
    }
  }

  ~Either() {
    if(isLeft) {
      leftValue.~L();
    } else {
      rightValue.~R();
    }
  }

  constexpr auto left() const -> Maybe<L> {
    if(!isLeft)
      return maybe();
    return maybe(leftValue);
  }

  constexpr auto right() const -> Maybe<R> {
    if(isLeft)
      return maybe();
    return maybe(rightValue);
  }


  static constexpr auto leftOf( L const& l ) {
    return Either<L, R>{ neither::left(l) };
  }

  static constexpr auto rightOf( R const& r ) {
    return Either<L, R>{ neither::right(r) };
  }


  template<
    class L2 = L,
    class R2 = R>
    constexpr auto join() const
    -> std::common_type_t<L2, R2> {
    return isLeft?  leftValue : rightValue;
  }

  template<class LeftF, class RightF>
  constexpr auto join(LeftF const& leftCase, RightF const&  rightCase) const
    -> decltype( isLeft? leftCase( leftValue ) : rightCase( rightValue ) ) {
    return isLeft? leftCase( leftValue ) : rightCase( rightValue );
  }

  template<class F>
  constexpr auto leftMap(F const& leftCase) const -> Either<decltype(leftCase( leftValue )), R> {
    using NextEither = Either<decltype(leftCase(leftValue)), R>;
    return isLeft ?
      NextEither::leftOf( leftCase( leftValue ) ) :
      NextEither::rightOf( rightValue );
  }

  template<class F>
  constexpr auto rightMap(F const& rightCase) const -> Either<L, decltype(rightCase(rightValue))> {
    using NextEither = Either<L, decltype(rightCase(rightValue))>;
    return isLeft ?
      NextEither::leftOf( leftValue ) :
      NextEither::rightOf( rightCase( rightValue ) );
  }

  template<class LeftCase, class RightCase>
  constexpr auto rightMap(LeftCase const& leftCase, RightCase const& rightCase) const
    -> decltype((*this)
      .leftMap(leftCase)
      .rightMap(rightCase)) {
    return (*this)
      .leftMap(leftCase)
      .rightMap(rightCase);
  }

  template<class LeftCase>
  constexpr auto leftFlatMap(LeftCase const& leftCase) const
    -> decltype( ensureEitherRight( leftCase( leftValue ), rightValue)  ) {
    using NextEither = decltype(ensureEitherRight( leftCase( leftValue ), rightValue));

    if (!*this) {
      return leftCase( leftValue );
    }

    return NextEither::rightOf(rightValue);
  }

  template<class RightCase>
  constexpr auto rightFlatMap(RightCase const& rightCase) const
    -> decltype( ensureEitherLeft(rightCase(rightValue), leftValue) ) {
    using NextEither = ensureEitherRight( rightCase( rightValue ), rightValue);

    if (*this) {
      return rightCase( rightValue );
    }

    return NextEither::leftOf(leftValue);
  }

  constexpr operator bool()const { return !isLeft; }

};

}

#endif
