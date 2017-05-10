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
struct Either {
private:
  L const& getLeft()const {
    return *reinterpret_cast<L const*>(data);
  }

  R const& getRight()const {
    return *reinterpret_cast<R const*>(data);
  }


  L& getLeft() {
    return *reinterpret_cast<L*>(data);
  }

  R& getRight() {
    return *reinterpret_cast<R*>(data);
  }

  bool const isLeft = 0;
  char data[max(sizeof(L), sizeof(R))];

public:

  Either( Left<L> const& l )
    : isLeft(1) {
    new (data) L(l.value);
  }

  Either( Right<R> const& r )
    : isLeft(0) {
    new (data) R(r.value);
  }

  Either( Either<L, R> const& e )
    : isLeft(e.isLeft) {
    if(isLeft) {
      new (data) L(e.getLeft());
    } else {
      new (data) R(e.getRight());
    }
  }

  ~Either() {
    if(isLeft) {
      getLeft().~L();
    } else {
      getRight().~R();
    }
  }


  constexpr auto left()const -> Maybe<L> {
    if(!isLeft)
      return maybe();
    return maybe(getLeft());
  }

  constexpr auto right()const -> Maybe<R> {
    if(isLeft)
      return maybe();
    return maybe(getRight());
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
    return isLeft? getLeft() : getRight();
  }

  template<class LeftF, class RightF>
  constexpr auto join(LeftF const& leftCase, RightF const&  rightCase) const
    -> decltype( isLeft? leftCase( getLeft() ) : rightCase( getRight() ) ) {
    return isLeft? leftCase( getLeft() ) : rightCase( getRight() );
  }

  template<class F>
  constexpr auto leftMap(F const& leftCase) const -> Either<decltype(leftCase( getLeft() )), R> {
    using NextEither = Either<decltype(leftCase(getLeft())), R>;
    return isLeft ?
      NextEither::leftOf( leftCase( getLeft() ) ) :
      NextEither::rightOf( getRight() );
  }

  template<class F>
  constexpr auto rightMap(F const& rightCase) const -> Either<L, decltype(rightCase(getRight()))> {
    using NextEither = Either<L, decltype(rightCase(getRight()))>;
    return isLeft ?
      NextEither::leftOf( getLeft() ) :
      NextEither::rightOf( rightCase( getRight() ) );
  }

  template<class LeftCase, class RightCase>
  constexpr auto rightMap(LeftCase const& leftCase, RightCase const& rightCase) const {
    return (*this)
      .leftMap(leftCase)
      .rightMap(rightCase);
  }

};

}

#endif
