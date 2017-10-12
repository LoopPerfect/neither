<a target='_blank' rel='nofollow' href='https://app.codesponsor.io/link/hq7GXSxnYW3qEM98fqMVob9v/LoopPerfect/neither'>  <img alt='Sponsor' width='888' height='68' src='https://app.codesponsor.io/embed/hq7GXSxnYW3qEM98fqMVob9v/LoopPerfect/neither.svg' /></a>

# neither

A functional implementation of Either in C++14.

[![Travis](https://img.shields.io/travis/LoopPerfect/neither.svg)](https://travis-ci.org/LoopPerfect/neither) [![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/LoopPerfect/neither/master/license)

```
buckaroo install loopperfect/neither
```

## Examples


### Handling Unsafe Code
```c++

auto unsafe = [] { // a function that throws, sometimes we can't avoid it...
  if (true) {
    throw std::runtime_error("error");
  }
  return 1;
}

Either<std::exception, int> e = Try<std::exception>(unsafe); // let's lift the exception into the typesystem

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

### Another Example

```c++
Either<std::string, int> compute(int x) {
  if(x<0) return left("don't pass x<0");
  return right(x*x);
}

std::string resultString = compute(5)
  .rightMap([](auto x){ return x/2.0;}) // success case
  .join(
    [](auto errorStr) { return "compute said: " + errorStr; }, // error-case
    [](auto x) { return "compute said: " + std::to_string(x); } // success-case
   );
   
std::cout << resultString << std::endl;

```

### Composition of Eithers

```c++

neither::Either<my_error_t, int> f1();
neither::Either<my_error_t, float> f2();

void compose() {
    auto value = f1()
        .rightFlatMap([](const struct_a& v){
            return f2();
        })
        .rightMap([](const struct_b& v){
            return 5;
        })
        .leftMap([](const auto& my_error){
            return 6;
        }).join();
    // value should be either 5 or 6
}

```

### Maybe Example

```c++

Maybe<float> compute(float x) {
  if(x<0) return {};
  return {sqrtf(x)};
}

Maybe<float> x = compute(-4)
 .map([](auto x){ return x*x;})
 .map([](auto x){ return x+1 });
 
if(!x.hasValue) {
  std::cerr << "error occured" << std::endl;
}
 
```

### Monadic Lifting

```c++

int sum(int x, int y){ return x+y; }

//...

auto monadicSum = lift(sum); // transforms sum to: Maybe<int> MonadicSum(Maybe<int>, Maybe<int>)

ASSERT_TRUE( monadicSum( maybe(5) , maybe(7) ).get(0) == 12 );
ASSERT_TRUE( monadicSum( maybe(), maybe(1) ).hasValue == false);
 
```

## Why Eithers? - Learned Lessons About Error handling

Some useful references: 

 - Benchmarks: [Mongrel Monads, Dirty, Dirty, Dirty - Niall Douglas [ACCU 2017]](https://youtu.be/XVofgKH-uu4?t=1h)

 - Why out-parameters are bad from the perspective of optimizers:
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

<a target='_blank' rel='nofollow' href='https://app.codesponsor.io/link/hq7GXSxnYW3qEM98fqMVob9v/LoopPerfect/neither'>  <img alt='Sponsor' width='888' height='68' src='https://app.codesponsor.io/embed/hq7GXSxnYW3qEM98fqMVob9v/LoopPerfect/neither.svg' /></a>
