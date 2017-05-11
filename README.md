# neither

A functional implementation of Either in C++14.

```
buckaroo install loopperfect/neither
```

## Introductory Example

```c++

auto evil = [] {
  if (true) {
    throw std::runtime_error("error");
  }
  return 1;
}

auto e = Try<std::exception>(evil);

e.left()
  .map([](auto const& e) { 
    return std::cerr << e.what() << std::endl; 
  }); // print error if available

int result = e
  .leftMap([](auto) { return 42; }) // do nothing with exception and map to 42
  .rightMap([](auto x) { return x * 2; }) // do further computation if value available
  .join() // join both sides of either
  
ASSERT_TRUE(result == 42);

```

## Why Eithers? - Learned Lessons About Error handling

Benchmarks: [Mongrel Monads, Dirty, Dirty, Dirty - Niall Douglas [ACCU 2017]](https://youtu.be/XVofgKH-uu4?t=1h)

Why out-parameters are bad from the perspective of optimizers:
[2013 Keynote: Chandler Carruth: Optimizing the Emergent Structures of C++](https://youtu.be/eR34r7HOU14?t=38m)

### Summary and Conclusions

- Error codes break composition
  - requires out-parameters; making functions impure and hard to reason about
  - using out-parameters makes inlining harder
  - => don't use output parameters 
- Exceptions are 2-3 orders of magnitude slower if exceptions are thrown
  - => avoid throwing exceptions - not always possible
- Overhead of exceptions grows linear with the callstack
  - => catch exceptions early
- Exceptions are not part of the type-system
  - annotating function signatures with `throw` and `noexcept` is not helpful; 
    contract breaches are not detected in compile-time but call `std::terminate` in run-time
  - handling exceptions is error prone and requires documentation
  - => encode errors in the types to enforce propper handling by the API consumer

## Installation

This library requires a C++ 14 compiler.

Install with [Buckaroo](https://buckaroo.pm): 
```
buckaroo install loopperfect/neither
```

The [Buck](https://www.buckbuild.com) target is `:neither` 

Alternatively you can copy & paste the headers to your include path: 
```
cp neither/include/*.hpp $InstallPath/include/neither
```
