# ctrx

[![GCC](https://github.com/jan-moeller/ctrx/actions/workflows/gcc.yml/badge.svg)](https://github.com/jan-moeller/ctrx/actions/workflows/gcc.yml)

This is a macro-based contracts checking library while we're waiting for official
language support. It provides precondition, postcondition, and assertion checking,
has several contract levels that can be selected at compile time, and allows the
application to implement a custom violation handler.

## Example

Let's say we want to implement the fibonacci sequence efficiently, so we come up
with the following iterative solution:

```c++
#include "ctrx/contracts.hpp"

auto fib(int n) -> int
{
    CTRX_PRECONDITION(n > 0, default, "Fibonacci numbers start with 1!");

    int second_last = 1;
    int last        = 1;
    for (int i = 2; i < n; ++i)
    {
        second_last = std::exchange(last, second_last + last);
        CTRX_ASSERT(second_last < last, audit);
    }

    int const r = last;
    CTRX_POSTCONDITION(r >= 1);

    return r;
}
```

This function has a precondition, because the fibonacci numbers start with 1 (at
least in the classic definition). Since the implementation has some complexity,
we added an assertion in the middle as a sanity-check, and we also want to verify
that the result is in the right ballpark (greater or equal to 1, in this case).

The application implementer can independently decide how these checks should be
handled based on contract type and level.

## API

The library provides 4 central macros:

```c++
CTRX_CONTRACT(type, condition, [level], [message])
CTRX_PRECONDITION(condition, [level], [message])
CTRX_POSTCONDITION(condition, [level], [message])
CTRX_ASSERT(condition, [level], [message])
```

### Arguments

| Name        | Mandatory | Description                                                                                                         | Notes                                                                                                                      |
|-------------|-----------|---------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------|
| `type`      | ✔         | One of: <br> - `PRECONDITION`, `precondition`<br> - `POSTCONDITION`, `postcondition` <br> - `ASSERTION` `assertion` | It is usually better to use one of the macros not taking this parameter.                                                   |
| `condition` | ✔         | Must be a valid expression.                                                                                         | If the expression contains commas, it needs to be wrapped in an additional set of parentheses to satisfy the preprocessor. |  
| `level`     | ✘         | One of: <br> - `DEFAULT`, `default` <br> - `AUDIT`, `audit` <br> - `AXIOM`, `axiom`                                 | Defaults to `default`. See below for the meaning of these contract levels.                                                 |
| `message`   | ✘         | Optional explanatory string literal. Some build modes use it to augment the error report.                           |                                                                                                                            |

*Important*: This library makes no guarantees as to how often the condition is
evaluated - this may also depend on build mode. You should therefore never use
expressions with side effects in a contract check!

## Build Time Configuration

CTRX has the following build-time configuration macros:

| Macro                             | Default             | Description                                                                                             | Notes                                             |
|-----------------------------------|---------------------|---------------------------------------------------------------------------------------------------------|---------------------------------------------------|
| `CTRX_CONFIG_LEVEL`               | `DEFAULT`           | One of: <br> - `OFF` <br> - `DEFAULT` <br> - `AUDIT`                                                    |                                                   |
| `CTRX_CONFIG_LEVEL_PRECONDITION`  | `CTRX_CONFIG_LEVEL` | One of: <br> - `OFF` <br> - `DEFAULT` <br> - `AUDIT`                                                    | Overrides `CTRX_CONFIG_LEVEL` for preconditions.  |
| `CTRX_CONFIG_LEVEL_POSTCONDITION` | `CTRX_CONFIG_LEVEL` | One of: <br> - `OFF` <br> - `DEFAULT` <br> - `AUDIT`                                                    | Overrides `CTRX_CONFIG_LEVEL` for postconditions. |
| `CTRX_CONFIG_LEVEL_ASSERTION`     | `CTRX_CONFIG_LEVEL` | One of: <br> - `OFF` <br> - `DEFAULT` <br> - `AUDIT`                                                    | Overrides `CTRX_CONFIG_LEVEL` for assertions.     |
| `CTRX_CONFIG_MODE`                | `ASSERT`            | One of: <br> - `OFF` <br> - `ASSERT` <br> - `ASSUME` <br> - `THROW` <br> - `TERMINATE` <br> - `HANDLER` |                                                   |
| `CTRX_CONFIG_MODE_PRECONDITION`   | `CTRX_CONFIG_MODE`  | One of: <br> - `OFF` <br> - `ASSERT` <br> - `ASSUME` <br> - `THROW` <br> - `TERMINATE` <br> - `HANDLER` | Overrides `CTRX_CONFIG_MODE` for preconditions.   |
| `CTRX_CONFIG_MODE_POSTCONDITION`  | `CTRX_CONFIG_MODE`  | One of: <br> - `OFF` <br> - `ASSERT` <br> - `ASSUME` <br> - `THROW` <br> - `TERMINATE` <br> - `HANDLER` | Overrides `CTRX_CONFIG_MODE` for postconditions.  |
| `CTRX_CONFIG_MODE_ASSERTION`      | `CTRX_CONFIG_MODE`  | One of: <br> - `OFF` <br> - `ASSERT` <br> - `ASSUME` <br> - `THROW` <br> - `TERMINATE` <br> - `HANDLER` | Overrides `CTRX_CONFIG_MODE` for assertions.      |

### Build Levels

Contracts can be annotated with a level. At build time, this allows you to turn
off expensive checks. The following table lists whether checks are turned on or
off according to their contract level and the global build level:

| Build Level ↴ \ Contract Level → | `DEFAULT` | `AUDIT` | `AXIOM` |
|----------------------------------|-----------|---------|---------|
| `DEFAULT`                        | ✔         | ✘       | ✘       |
| `AUDIT`                          | ✔         | ✔       | ✘       |

In other words, the `DEFAULT` build level enables checking of all `DEFAULT`
contracts, and the `AUDIT` build level enables checking of all `DEFAULT` and
`AUDIT` contracts. Contracts with `AXIOM` level are never checked and serve as
formal comments.

### Build Modes

#### OFF

Turns off checking completely. You might want to choose this in release builds.

#### ASSERT

Uses C's `assert()` macro under the hood, which implies no checking in release
builds, and implementation defined checking in debug builds.

#### ASSUME

Uses C++23's attribute `[[assume()]]`. Note that this doesn't check at all, but
uses the contract for optimization, potentially introducing *more* UB rather
than reducing it. Use this for release builds if performance matters most,
you're really, *really* sure you never call anything out of contract, and
you're feeling particularly adventurous today.

#### THROW

Throws exceptions on contract violations, namely `ctrx::precondition_violation`,
`ctrx::postcondition_violation` and `ctrx::assertion_violation`. All of these
exceptions inherit from `ctrx::contract_violation`, which can be used as a
catch-all. `ctrx::contract_violation` inherits from `std::exception`. Use this
mode for unit-testing out-of-contract calls.

Note that, if you need to throw a different exception type, you can do so using
the `HANDLER` mode.

See below for a full description of these exception types.

#### TERMINATE

This mode just calls `std::terminate` on contract violations. Maybe this is
useful if you can afford to crash, but you cannot afford to continue running
out-of-contract.

#### HANDLER

This is the most general mode; it allows you to implement your own contract
violation handler. This can be used for logging, and you can decide whether you
want to terminate, throw an exception, or even continue.
You have to implement a function with the following signature as handler:

```c++
namespace ctrx
{
void handle_contract_violation(contract_type type, char const* msg, std::source_location const& sloc);
} // namespace ctrx
```

- The `type` argument determines the type of the failed contract check.
- The `msg` is a descriptive string of the failed contract check.
- The `sloc` is the source location where the contract violation occurred.

## Constant Evaluation

Generally, contract checks can be used in `constexpr` and `consteval` functions.
However, in most modes, a failed contract check during constant evaluation results
in an immediate compilation error. The only guaranteed exception is if the
build mode is `OFF` - in this case, no diagnostic is issued and constant
evaluation continues as if the contract wasn't violated.

## Conditionally Defined Types

The following types are made available only if required by the currently set build
mode:

```c++
namespace ctrx
{
enum class contract_type
{
    precondition,
    postcondition,
    assertion,
};
    
struct contract_violation : public std::exception
{
    [[nodiscard]] constexpr auto type() const noexcept -> contract_type;
    [[nodiscard]] constexpr auto source_location() const noexcept -> std::source_location const&;
    [[nodiscard]] inline auto what() const noexcept -> char const* override;
};
struct precondition_violation : contract_violation;
struct postcondition_violation : contract_violation;
struct assertion_violation : contract_violation;
} // namespace ctrx
```

## How To Use

CTRX is a header-only library, so in theory you could just drop the `include/ctrx`
folder in your project and start using it. `#include "ctrx/contracts.hpp"` to get
the main macros.

### CMake Integration

CTRX makes itself available as `ctrx::ctrx` to cmake targets. All you need to do
is

```cmake
target_link_libraries(YOUR_TARGET PRIVATE ctrx::ctrx)
```

#### Cache Variables

The following cache variables are available to conveniently change the build mode
and level:

- CTRX_CONFIG_LEVEL
- CTRX_CONFIG_LEVEL_PRECONDITION
- CTRX_CONFIG_LEVEL_POSTCONDITION
- CTRX_CONFIG_LEVEL_ASSERTION
- CTRX_CONFIG_MODE
- CTRX_CONFIG_MODE_PRECONDITION
- CTRX_CONFIG_MODE_POSTCONDITION
- CTRX_CONFIG_MODE_ASSERTION

These intentionally have the same names as the preprocessor macros they set.

#### CPM

If you're using [CPM](https://github.com/cpm-cmake/CPM.cmake), you can use CTRX like this:

```cmake
CPMAddPackage("gh:jan-moeller/ctrx@1.4.0")
target_link_libraries(YOUR_TARGET PRIVATE ctrx::ctrx)
```

Note that you can also configure CTRX this way:

```cmake
CPMAddPackage(
        NAME ctrx
        VERSION 1.4.0
        GITHUB_REPOSITORY jan-moeller/ctrx
        OPTIONS "CTRX_CONFIG_MODE THROW" "CTRX_CONFIG_LEVEL AUDIT"
) # Or set whatever options you like
target_link_libraries(YOUR_EXECUTABLE_TARGET PUBLIC ctrx::ctrx)
```

However, this should only be done if you are implementing an application (as
opposed to a library). See the recommendations section below for further
discussion.

## Recommended Use

1. If you are writing a library, do not set any configuration - this choice has
   to be made by the application using your library. The reason is that, as a
   library author, you don't know in what context your library is going to be
   used.
   Note that your unit test application is an application, and probably should
   set a configuration convenient for unit testing (see below).
2. If a check is costly compared to what the function does (e.g. the check is
   more expensive than the function itself), tag it as `audit`.
3. If a check is in a frequent path, e.g. a tight loop, tag it as `audit` as
   well.
4. It is advisable to compile your unit tests with all checks turned on and in
   `THROW` mode - virtually all unit test frameworks treat an escaped exception
   as a test failure. Therefore, your tests will fail if any contract violation
   occured, and give you a nice message of what went wrong.
5. Be very careful about the `ASSUME` mode. Few software is free of bugs, and
   this mode has the ability to make your application behave much worse than it
   did without it.
6. You will find that you can rarely exactly check your postconditions. That's
   okay, your unit tests will cover that case. But it sometimes pays off to at
   least check the bounds of the return value.
7. Never have side effects in your contract checks.
