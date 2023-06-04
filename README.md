# ctrx

[![GCC](https://github.com/jan-moeller/ctrx/actions/workflows/gcc.yml/badge.svg)](https://github.com/jan-moeller/ctrx/actions/workflows/gcc.yml)

This is a macro-based contracts stand-in while we're waiting for official
language support. It provides precondition, postcondition, and assertion
checking, and has several modes that can be selected at compile time.

## Example

Let's say we want to implement the fibonacci sequence efficiently, so we come up
with the following iterative solution:

```c++
#include "ctrx/contracts.hpp"

auto fib(int n) -> int
{
    CTRX_PRECONDITION(n > 0);

    int second_last = 1;
    int last        = 1;
    for (int i = 2; i < n; ++i)
    {
        second_last = std::exchange(last, second_last + last);
        CTRX_ASSERT(second_last < last);
    }

    int const r = last;
    CTRX_POSTCONDITION(r >= 1);

    return r;
}
```

Now, this function has a precondition, because the fibonacci numbers start with
1 (at least in the classic definition). Since the implementation has some
complexity, we would like to add some assertions half-way, to make sure we stay
sane, and we also want to verify the postcondition that the result is greater or
equal to 1.

By default, all these checks are identical to C's good old `assert()`. However,
CTRX provides several other modes.

## Build Modes

### OFF

Turns off checking completely. You might want to choose this in release builds.

### ASSERT

Uses C's `assert()` macro under the hood, which implies no checking in release
builds, and implementation defined checking in debug builds.

### ASSUME

Uses C++23's attribute `[[assume()]]`. Note that this doesn't check at all, but
uses the contract for optimization, potentially introducing *more* UB rather
than reducing it. Use this for release builds if performance matters, and you're
really, *really* sure you never call anything out of contract.

### THROW

Throws exceptions on contract violations, namely `ctrx::precondition_violation`,
`ctrx::postcondition_violation` and `ctrx::assertion_violation`. Use this for
unit-testing out-of-contract calls.

### TERMINATE

This mode just calls `std::terminate` on contract violations. Maybe this is
useful if you can afford to crash, but you cannot afford to continue running
out-of-contract.

### HANDLER

This is the most general mode; it allows you to register any function as a
contract violation handler. This can be used for logging, and you can decide
whether you want to terminate, throw an exception, or even continue.

## Usage

CTRX uses pre-processor macros for its configuration. You could `#define` them
just prior to `#include`ing `ctrx/contracts.hpp`, but it's easier to define them
globally, and if you're using cmake, then this project comes equipped so you can
easily do that.

* CTRX_CONFIG_MODE \
  Can be used to set the mode globally, i.e. for preconditions, postconditions,
  and assertions all at once. Must be defined to be one of the build modes
  above, or can be left undefined, in which case it defaults to ASSERT.
* CTRX_CONFIG_MODE_PRECONDITION \
  Set the mode for preconditions only. Defaults to CTRX_CONFIG_MODE.
* CTRX_CONFIG_MODE_POSTCONDITION \
  Set the mode for postconditions only. Defaults to CTRX_CONFIG_MODE.
* CTRX_CONFIG_MODE_ASSERTION \
  Set the mode for assertions only. Defaults to CTRX_CONFIG_MODE.
* CTRX_CONFIG_HANDLER \
  Only relevant to the HANDLER mode. Sets the name of the global handler. The
  handler must have the following signature:
  ```c++
  /* constexpr */ void handler(char const* assertion, std::source_location const& sloc);
  ```
  When called, the first argument will be a string representation of the failed
  check, and the second argument will be the source location of that check.
  If the handler isn't marked `constexpr`, then the macros can't be used in
  `constexpr` code.
* CTRX_CONFIG_HANDLER_PRECONDITION \
  Only relevant to the HANDLER mode. Sets the name of the precondition handler.
  Defaults to CTRX_CONFIG_HANDLER.
* CTRX_CONFIG_HANDLER_POSTCONDITION \
  Only relevant to the HANDLER mode. Sets the name of the postcondition handler.
  Defaults to CTRX_CONFIG_HANDLER.
* CTRX_CONFIG_HANDLER_ASSERTION \
  Only relevant to the HANDLER mode. Sets the name of the assertion handler.
  Defaults to CTRX_CONFIG_HANDLER.
* CTRX_CONFIG_HANDLER_INCLUDE \
  Only relevant to the HANDLER mode. Can be set to automatically `#include` the
  correct header for the global handler.
* CTRX_CONFIG_HANDLER_INCLUDE_PRECONDITION \
  Only relevant to the HANDLER mode. Can be set to automatically `#include` the
  correct header for the precondition handler. Defaults to
  CTRX_CONFIG_HANDLER_INCLUDE.
* CTRX_CONFIG_HANDLER_INCLUDE_POSTCONDITION \
  Only relevant to the HANDLER mode. Can be set to automatically `#include` the
  correct header for the postcondition handler. Defaults to
  CTRX_CONFIG_HANDLER_INCLUDE.
* CTRX_CONFIG_HANDLER_INCLUDE_ASSERTION \
  Only relevant to the HANDLER mode. Can be set to automatically `#include` the
  correct header for the assertion handler. Defaults to
  CTRX_CONFIG_HANDLER_INCLUDE.

## CMake Integration

It's easiest using cmake with [CPM](https://github.com/cpm-cmake/CPM.cmake).

If you're writing an executable:

```cmake
CPMAddPackage(
        NAME ctrx
        VERSION 1.0.0
        GITHUB_REPOSITORY jan-moeller/ctrx
        OPTIONS "CTRX_CONFIG_MODE THROW"
) # Or set whatever options you like
target_link_libraries(YOUR_TARGET PUBLIC ctrx::ctrx)
```

If you're writing a library:

```cmake
CPMAddPackage("gh:jan-moeller/ctrx@1.0.0")
target_link_libraries(YOUR_TARGET PUBLIC ctrx::ctrx)
```

Don't set any options in this case, since you don't know what your user wants.

