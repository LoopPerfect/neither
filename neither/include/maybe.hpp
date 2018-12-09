#ifndef NEITHER_MAYBE_HPP
#define NEITHER_MAYBE_HPP

#include <memory>
#include <cassert>
#include <cstddef>
#include <type_traits>
#include <neither/traits.hpp>

namespace neither {

template <class T> struct Maybe;

template <> struct Maybe<void> {};

template <class T> struct Maybe {

  using size_type = std::size_t;

  union {
    T value;
  };

  bool const hasValue;

  constexpr Maybe() : hasValue{false} {}

  constexpr Maybe(T const& value) :  value{value}, hasValue{true} {}
  constexpr Maybe(T&& value) :  value{std::move(value)}, hasValue{true} {}

  constexpr Maybe(Maybe<void>) : hasValue{false} {}

  constexpr Maybe(Maybe<T> const &o) : hasValue{o.hasValue} {
    if (o.hasValue) {
      new (&value)T(o.value);
    }
  }

  ~Maybe() {
    if (hasValue) {
      value.~T();
    }
  }

  constexpr T get(T defaultValue) {
    return hasValue ? value : defaultValue;
  }

  constexpr T unsafeGet() {
    assert(hasValue && "unsafeGet must not be called on an empty Maybe");
    return value;
  }

  constexpr size_type size() const noexcept { return hasValue ? 1: 0; }
  
  constexpr bool empty() const noexcept { return !hasValue; }
  
  template<class F>
    constexpr auto map(F const &f) const&
    -> Maybe<decltype(f(isCopyable(value)))> {
    using ReturnType = decltype(f(value));
    if (!hasValue) {
      return Maybe<ReturnType>();
    }
    return Maybe<ReturnType>(f(value));
  }


  template<class F>
    auto map(F const& f)&&
    -> Maybe<decltype(f(std::move(value)))> {
    using ReturnType = decltype(f(std::move(value)));
    if (!hasValue) {
      return Maybe<ReturnType>();
    }
    return Maybe<ReturnType>(f(std::move(value)));
  }

  template <class F>
  constexpr auto flatMap(F const& f) const&
    -> decltype(ensureMaybe(f(value))) {
    using ReturnType = decltype(f(value));
    if (!hasValue) {
      return ReturnType();
    }

    return f(value);
  }

  template <class F>
  constexpr auto flatMap(F const& f)&&
    -> decltype(ensureMaybe(f(std::move(value)))) {
    using ReturnType = decltype(f(std::move(value)));
    if (!hasValue) {
      return ReturnType();
    }

    return f(std::move(value));
  }

  constexpr operator bool() const { return hasValue; }
};

template <typename T>
auto maybe(T value) -> Maybe<T> { return {value}; }

template <typename T = void>
auto maybe() -> Maybe<T> { return {}; }

namespace {

  inline
  bool equal(Maybe<void> const&, Maybe<void> const&) {
    return true;
  }

  template <typename T>
  bool equal(Maybe<T> const &a, Maybe<T> const &b) {
    if (a.hasValue) {
      return b.hasValue && a.value == b.value;
    }
    return !b.hasValue;
  }
}

template <typename T>
bool operator == (Maybe<T> const& a, Maybe<T> const& b) {
  return equal(a, b);
}

template <typename T>
bool operator != (Maybe<T> const& a, Maybe<T> const& b) {
  return !(a == b);
}

static const auto none = maybe();

}

#endif
