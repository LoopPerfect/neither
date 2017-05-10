#ifndef NEITHER_OPTIONAL_HPP
#define NEITHER_OPTIONAL_HPP

namespace neither {

template<class T>
struct Optional;

template<>
struct Optional<void> {};

template<class T>
struct Optional {

  bool const hasValue = 0;
  union {
    T data;
  };

  Optional()
    : hasValue{0}
  {}

  Optional(T const& value)
    : hasValue{1}
    , data{value}
  {}

  Optional(Optional<void>)
    : hasValue{0} {}

  Optional(Optional<T> const& o)
    : hasValue{o.hasValue} {
    if(o.hasValue) {
      new (&data) T(o.data);
    }
  }

  ~Optional() {
    if(hasValue) {
       data.~T();
    }
  }

  constexpr T get(T defaultValue) {
    if(hasValue) return data;
    return defaultValue;
  }

  template<class F>
  constexpr auto map(F const& f)const {
    using ReturnType = decltype(f(data));
    if(!hasValue) {
      return Optional<ReturnType>();
    }

    return Optional<ReturnType>(f(data));
  }

  template<class F>
  constexpr auto flatMap(F const& f)const -> decltype(f(data)) {
    using ReturnType = decltype(f(data));
    if(!hasValue) {
      return ReturnType();
    }

    return f(data);
  }
};

template<class T>
auto optional(T value) -> Optional<T> {
  return {value};
}

template<class T=void>
auto optional() -> Optional<T> {
  return {};
}

}

#endif
