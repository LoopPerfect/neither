#ifndef NEITHER_MAYBE_HPP
#define NEITHER_MAYBE_HPP

namespace neither {

template <class T> struct Maybe;

template <> struct Maybe<void> {};

template <class T> struct Maybe {

  union {
    T value;
  };
  bool const hasValue = 0;

  constexpr Maybe() : hasValue{0} {}

  constexpr Maybe(T const &value) :  value{value}, hasValue{1} {}

  constexpr Maybe(Maybe<void>) : hasValue{0} {}

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
    if (hasValue)
      return value;
    return defaultValue;
  }

  template <class F> constexpr auto map(F const &f) const {
    using ReturnType = decltype(f(value));
    if (!hasValue) {
      return Maybe<ReturnType>();
    }
    return Maybe<ReturnType>(f(value));
  }

  template <class F>
  constexpr auto flatMap(F const &f) const -> decltype(f(value)) {
    using ReturnType = decltype(f(value));
    if (!hasValue) {
      return ReturnType();
    }

    return f(value);
  }

  constexpr operator bool()const { return hasValue; }
};

template <class T> auto maybe(T value) -> Maybe<T> { return {value}; }

template <class T = void> auto maybe() -> Maybe<T> { return {}; }
}

#endif
