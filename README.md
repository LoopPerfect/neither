# neither

A Functional implementation of Either in C++14.

## Introductory Example

```c++

auto evil = []{
  if (true) {
    throw std::runtime_error("error");
  }
  return 1;
}

auto e = Try<std::exception>(evil);

e.left()
  .map([](auto const& e){ 
    return std::cerr << e.what() << std::endl; 
  }); // print error if available

int result = e
  .leftMap([](auto){ return 42;}) // do nothing with exception and map to 42
  .rightMap([](auto x){ return x*2;}) // do further computation if value available
  .join() // join both sides of either
  
ASSERT_TRUE(result == 42);

```

## Why Eithers? - Learned Lessons of Error handling

benchmarks: https://youtu.be/XVofgKH-uu4?t=1h
https://youtu.be/eR34r7HOU14?t=38m

why out-parameters are bad from the perspective of optimizers:
https://youtu.be/eR34r7HOU14?t=38m

### Summary and Conclusions

- error_codes break composition
  - requires out-parameters; making functions impure and hard to reason
  - using out-parameters makes inlining harder
  - => don't use output parameters 
- exceptions are 2-3 orders of magnitude slower if exception are thrown
  - => avoid throwing exceptions if possible
- Overhead of exceptions grows linear with the callstack
  - => catch exceptions early
- Exceptions are not part of the typesystem
  - annotating function signatures with throw and noexcept is not helpfull; 
    contract breaches are not detected in compiletime but call std::terminate in runtime
  - handling exceptions is errorprone and requires documentation
  - => encode errors in the types enforces propper handling by the API consumer

## Instalation

This library requires a C++14 compiler.

buck target is :neither 

Install with buckaroo: `buckaroo install loopperfect/neither`

alternatively with copy&paste: `cp neither/include/*.hpp $InstallPath/include/neither`
