#ifndef NEITHER_MAYBE_HPP
#define NEITHER_MAYBE_HPP

namespace neither {

template <class T> struct Maybe;

template <> struct Maybe<void> {};

template <class T> struct Maybe {

  bool const hasValue = 0;
  union {
    T data;
  };

  Maybe() : hasValue{0} {}

  Maybe(T const &value) : hasValue{1}, data{value} {}

  Maybe(Maybe<void>) : hasValue{0} {}

  Maybe(Maybe<T> const &o) : hasValue{o.hasValue} {
    if (o.hasValue) {
      new (&data) T(o.data);
    }
  }

  ~Maybe() {
    if (hasValue) {
      data.~T();
    }
  }

  constexpr T get(T defaultValue) {
    if (hasValue)
      return data;
    return defaultValue;
  }

  template <class F> constexpr auto map(F const &f) const {
    using ReturnType = decltype(f(data));
    if (!hasValue) {
      return Maybe<ReturnType>();
    }
    return Maybe<ReturnType>(f(data));
  }

  template <class F>
  constexpr auto flatMap(F const &f) const -> decltype(f(data)) {
    using ReturnType = decltype(f(data));
    if (!hasValue) {
      return ReturnType();
    }

    return f(data);
  }
};

template <class T> auto maybe(T value) -> Maybe<T> { return {value}; }

template <class T = void> auto maybe() -> Maybe<T> { return {}; }
}

#endif
