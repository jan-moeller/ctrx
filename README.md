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
CTRX provides several other modes. In particular, checking of preconditions,
postconditions and assertions can be turned on and off individually.

## Build Levels

Contracts can be annotated with a level. At build time, this allows you to turn
off expensive checks.

| Build Level ↴ \ Contract Level → | DEFAULT | AUDIT | AXIOM |
|----------------------------------|---------|-------|-------|
| DEFAULT                          | ✔       | ✘     | ✘     |
| AUDIT                            | ✔       | ✔     | ✘     |

### DEFAULT

Enables checking of all `DEFAULT` contracts.

### AUDIT

Enables checking of all `DEFAULT` and `AUDIT` contracts.

## Build Modes

### OFF

Turns off checking completely. You might want to choose this in release builds.

### ASSERT

Uses C's `assert()` macro under the hood, which implies no checking in release
builds, and implementation defined checking in debug builds.

### ASSUME

Uses C++23's attribute `[[assume()]]`. Note that this doesn't check at all, but
uses the contract for optimization, potentially introducing *more* UB rather
than reducing it. Use this for release builds if performance matters most,
you're really, *really* sure you never call anything out of contract, and
you're feeling particularly adventurous today.

### THROW

Throws exceptions on contract violations, namely `ctrx::precondition_violation`,
`ctrx::postcondition_violation` and `ctrx::assertion_violation`. All of these
exceptions inherit from `ctrx::contract_violation`, which can be used as a
catch-all. Use this mode for unit-testing out-of-contract calls.

### TERMINATE

This mode just calls `std::terminate` on contract violations. Maybe this is
useful if you can afford to crash, but you cannot afford to continue running
out-of-contract.

### HANDLER

This is the most general mode; it allows you to implement your own contract
violation handler. This can be used for logging, and you can decide whether you
want to terminate, throw an exception, or even continue.
You have to implement a function with the following signature as handler:

```c++
namespace ctrx
{
void handle_contract_violation(contract_type, char const*, std::source_location const&);
} // namespace ctrx
```

Here, `contract_type` is defined by `contracts.hpp` like this:

```c++
namespace ctrx
{
enum class contract_type
{
    precondition,
    postcondition,
    assertion,
};
} // namespace ctrx
```

The second function argument is a stringified version of the failed contract,
and the third argument is the source location where the contract violation
occurred.

Note that the `HANDLER` mode falls back to `ASSERT` mode during constant
evaluation. This is because (for obvious reasons) it isn't possible to call
`extern` functions during constant evaluation.

## Usage

CTRX uses pre-processor macros for its configuration. You could `#define` them
just prior to `#include`ing `ctrx/contracts.hpp`, but it's easier to define them
globally, and if you're using cmake, then this project allows you to easily do
that.

* `CTRX_CONFIG_LEVEL` \
  Can be set to one of the build levels listed above. Defaults to `DEFAULT`.
* `CTRX_CONFIG_MODE` \
  Can be used to set the mode globally, i.e. for preconditions, postconditions,
  and assertions all at once. Must be defined to be one of the build modes
  above, or can be left undefined, in which case it defaults to `ASSERT`.
* `CTRX_CONFIG_MODE_PRECONDITION` \
  Set the mode for preconditions only. Defaults to `CTRX_CONFIG_MODE`.
* `CTRX_CONFIG_MODE_POSTCONDITION` \
  Set the mode for postconditions only. Defaults to `CTRX_CONFIG_MODE`.
* `CTRX_CONFIG_MODE_ASSERTION` \
  Set the mode for assertions only. Defaults to `CTRX_CONFIG_MODE`.

The library provides these macros:

* `CTRX_CONTRACT(type, level, message, condition)` \
  This is the central macro, however, it probably shouldn't be used directly.
  Instead, prefer one of the specialized macros below.
    + `type` \
      Must be one of `PRECONDITION`, `POSTCONDITION`, or `ASSERTION`
    + `level` \
      Optional. If set, must be one of `DEFAULT`, `AUDIT`, or `AXIOM`. Defaults to
      `DEFAULT`.
    + `message` \
      Optional. If set, must be a string literal. \
      Note that, if the macro is called with 3 arguments, the second one is assumed
      to be level. If you want to provide a message and keep the level at default,
      use the _M variant below.
    + `condition` \
      The condition to check. Must be convertible to `bool`. \
      Note that, if your condition contains commas, you have to wrap it in additional
      parentheses.
* `CTRX_CONTRACT_M(type, message, condition)` \
  Analogous to `CTRX_CONTRACT`, but allows providing a message while using the
  default `level`.
* `CTRX_PRECONDITION(level, message, condition)` \
  Identical to `CTRX_CONTRACT(PRECONDITION, level, message, condition)`, with
  `level` and `message` being optional.
* `CTRX_PRECONDITION_M(message, condition)` \
  Identical to `CTRX_CONTRACT_M(PRECONDITION, message, condition)`.
* `CTRX_POSTCONDITION(level, message, condition)` \
  Identical to `CTRX_CONTRACT(POSTCONDITION, level, message, condition)`, with
  `level` and `message` being optional.
* `CTRX_POSTCONDITION_M(message, condition)` \
  Identical to `CTRX_CONTRACT_M(POSTCONDITION, message, condition)`.
* `CTRX_ASSERT(level, message, condition)` \
  Identical to `CTRX_CONTRACT(ASSERTION, level, message, condition)`, with
  `level` and `message` being optional.
* `CTRX_ASSERT_M(message, condition)` \
  Identical to `CTRX_CONTRACT_M(ASSERTION, message, condition)`.

## CMake Integration

It's easiest using cmake with [CPM](https://github.com/cpm-cmake/CPM.cmake).

If you're writing an executable and you absolutely require one specific mode:

```cmake
CPMAddPackage(
        NAME ctrx
        VERSION 1.3.0
        GITHUB_REPOSITORY jan-moeller/ctrx
        OPTIONS "CTRX_CONFIG_MODE THROW" "CTRX_CONFIG_LEVEL AUDIT"
) # Or set whatever options you like
target_link_libraries(YOUR_TARGET PUBLIC ctrx::ctrx)
```

If you're writing a library:

```cmake
CPMAddPackage("gh:jan-moeller/ctrx@1.3.0")
target_link_libraries(YOUR_TARGET PUBLIC ctrx::ctrx)
```

Don't set any options in this case, since you don't know what your user wants.

In all other cases, you can set the mode via CMakeCache as appropriate, and
CMake will rebuild all source dependencies with that mode set.
