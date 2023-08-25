//
// MIT License
//
// Copyright (c) 2023 Jan Möller
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef CTRX_CONTRACTS_HPP
#define CTRX_CONTRACTS_HPP

#include "ctrx/contract_type.hpp"

// ------------------------------------------------------
// Helpers
// ------------------------------------------------------

// Concatenate tokens
#define CTRX_DETAIL_CONCAT(A, B) A##B
#define CTRX_DETAIL_CONCAT2(A, B) CTRX_DETAIL_CONCAT(A, B)

// Stringify tokens
#define CTRX_DETAIL_STRINGIFY(A) #A
#define CTRX_DETAIL_STRINGIFY2(A) CTRX_DETAIL_STRINGIFY(A)

// Allow macro overloading based on argument count
#define CTRX_DETAIL_GET_OVERLOADED_MACRO_3(ARG1, ARG2, ARG3, NAME, ...) NAME

// Check code validity in an unevaluated context
#define CTRX_DETAIL_CHECK_CODE_VALIDITY(...) (void)sizeof(__VA_ARGS__)

// ------------------------------------------------------
// Modes
// ------------------------------------------------------

// Assigning numbers to modes so we can preprocessor-compare them
#define CTRX_DETAIL_MODE_NUM_OFF 1
#define CTRX_DETAIL_MODE_NUM_ASSERT 2
#define CTRX_DETAIL_MODE_NUM_ASSUME 3
#define CTRX_DETAIL_MODE_NUM_THROW 4
#define CTRX_DETAIL_MODE_NUM_TERMINATE 5
#define CTRX_DETAIL_MODE_NUM_HANDLER 6

#define CTRX_DETAIL_MODE_NUM_off CTRX_DETAIL_MODE_NUM_OFF
#define CTRX_DETAIL_MODE_NUM_assert CTRX_DETAIL_MODE_NUM_ASSERT
#define CTRX_DETAIL_MODE_NUM_assume CTRX_DETAIL_MODE_NUM_ASSUME
#define CTRX_DETAIL_MODE_NUM_throw CTRX_DETAIL_MODE_NUM_THROW
#define CTRX_DETAIL_MODE_NUM_terminate CTRX_DETAIL_MODE_NUM_TERMINATE
#define CTRX_DETAIL_MODE_NUM_handler CTRX_DETAIL_MODE_NUM_HANDLER

// ------------------------------------------------------
// Levels
// ------------------------------------------------------

// Assigning numbers to levels so we can preprocessor-compare them
#define CTRX_DETAIL_LEVEL_NUM_OFF 1
#define CTRX_DETAIL_LEVEL_NUM_DEFAULT 2
#define CTRX_DETAIL_LEVEL_NUM_AUDIT 3
#define CTRX_DETAIL_LEVEL_NUM_AXIOM 4

#define CTRX_DETAIL_LEVEL_NUM_off CTRX_DETAIL_LEVEL_NUM_OFF
#define CTRX_DETAIL_LEVEL_NUM_default CTRX_DETAIL_LEVEL_NUM_DEFAULT
#define CTRX_DETAIL_LEVEL_NUM_audit CTRX_DETAIL_LEVEL_NUM_AUDIT
#define CTRX_DETAIL_LEVEL_NUM_axiom CTRX_DETAIL_LEVEL_NUM_AXIOM

// ------------------------------------------------------
// Default settings
// ------------------------------------------------------

// If no level is set, use DEFAULT
#if !defined(CTRX_CONFIG_LEVEL)
#define CTRX_CONFIG_LEVEL DEFAULT
#endif

// If no global mode is set use ASSERT
#if !defined(CTRX_CONFIG_MODE)
#define CTRX_CONFIG_MODE ASSERT
#endif

// Fall back to global level, if no specific level is set for ASSERTION/PRECONDITION/POSTCONDITION
#if !defined(CTRX_CONFIG_LEVEL_PRECONDITION)
#define CTRX_CONFIG_LEVEL_PRECONDITION CTRX_CONFIG_LEVEL
#endif

#if !defined(CTRX_CONFIG_MODE_POSTCONDITION)
#define CTRX_CONFIG_LEVEL_POSTCONDITION CTRX_CONFIG_LEVEL
#endif

#if !defined(CTRX_CONFIG_LEVEL_ASSERTION)
#define CTRX_CONFIG_LEVEL_ASSERTION CTRX_CONFIG_LEVEL
#endif

// Fall back to global mode, if no specific mode is set for ASSERTION/PRECONDITION/POSTCONDITION
#if !defined(CTRX_CONFIG_MODE_PRECONDITION)
#define CTRX_CONFIG_MODE_PRECONDITION CTRX_CONFIG_MODE
#endif

#if !defined(CTRX_CONFIG_MODE_POSTCONDITION)
#define CTRX_CONFIG_MODE_POSTCONDITION CTRX_CONFIG_MODE
#endif

#if !defined(CTRX_CONFIG_MODE_ASSERTION)
#define CTRX_CONFIG_MODE_ASSERTION CTRX_CONFIG_MODE
#endif

// ------------------------------------------------------
// Config validation
// ------------------------------------------------------

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL) == 0
#error "Invalid CTRX_CONFIG_LEVEL"
#endif
#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_PRECONDITION) == 0
#error "Invalid CTRX_CONFIG_LEVEL_PRECONDITION"
#endif
#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_POSTCONDITION) == 0
#error "Invalid CTRX_CONFIG_LEVEL_POSTCONDITION"
#endif
#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_ASSERTION) == 0
#error "Invalid CTRX_CONFIG_LEVEL_ASSERTION"
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE) == 0
#error "Invalid CTRX_CONFIG_MODE"
#endif
#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == 0
#error "Invalid CTRX_CONFIG_MODE_PRECONDITION"
#endif
#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == 0
#error "Invalid CTRX_CONFIG_MODE_POSTCONDITION"
#endif
#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == 0
#error "Invalid CTRX_CONFIG_MODE_ASSERTION"
#endif

// ------------------------------------------------------
// Check which modes are in use
// ------------------------------------------------------

#if (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_OFF)            \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == CTRX_DETAIL_MODE_NUM_OFF)        \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == CTRX_DETAIL_MODE_NUM_OFF)
#define CTRX_DETAIL_USING_MODE_OFF
#endif
#if (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_ASSERT)         \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == CTRX_DETAIL_MODE_NUM_ASSERT)     \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == CTRX_DETAIL_MODE_NUM_ASSERT)
#define CTRX_DETAIL_USING_MODE_ASSERT
#endif
#if (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_ASSUME)         \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == CTRX_DETAIL_MODE_NUM_ASSUME)     \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == CTRX_DETAIL_MODE_NUM_ASSUME)
#define CTRX_DETAIL_USING_MODE_ASSUME
#endif
#if (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_THROW)          \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == CTRX_DETAIL_MODE_NUM_THROW)      \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == CTRX_DETAIL_MODE_NUM_THROW)
#define CTRX_DETAIL_USING_MODE_THROW
#endif
#if (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_TERMINATE)      \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == CTRX_DETAIL_MODE_NUM_TERMINATE)  \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == CTRX_DETAIL_MODE_NUM_TERMINATE)
#define CTRX_DETAIL_USING_MODE_TERMINATE
#endif
#if (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_HANDLER)        \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == CTRX_DETAIL_MODE_NUM_HANDLER)    \
    || (CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == CTRX_DETAIL_MODE_NUM_HANDLER)
#define CTRX_DETAIL_USING_MODE_HANDLER
#endif

// ------------------------------------------------------
// Include the required headers
// ------------------------------------------------------

#if defined(CTRX_DETAIL_USING_MODE_ASSERT)
#include <concepts>
#include <optional>

#include <cassert>
#endif
#if defined(CTRX_DETAIL_USING_MODE_THROW)
#include "ctrx/exceptions/assertion_violation.hpp"
#include "ctrx/exceptions/contract_violation.hpp"
#include "ctrx/exceptions/postcondition_violation.hpp"
#include "ctrx/exceptions/precondition_violation.hpp"

#include <concepts>
#include <optional>
#endif
#if defined(CTRX_DETAIL_USING_MODE_TERMINATE)
#include <concepts>
#include <exception>
#include <optional>
#endif
#if defined(CTRX_DETAIL_USING_MODE_HANDLER)
#include <concepts>
#include <optional>
#include <source_location>
#include <string_view>
#endif

// ------------------------------------------------------
// Define handler entry point if required
// ------------------------------------------------------

#if defined(CTRX_DETAIL_USING_MODE_HANDLER)
namespace ctrx
{
extern void handle_contract_violation(contract_type, std::string_view, std::source_location const&);
} // namespace ctrx
#endif

// ------------------------------------------------------
// Map of contract type to exception type
// ------------------------------------------------------

#define CTRX_DETAIL_EXCEPTION_TYPE_PRECONDITION ::ctrx::precondition_violation
#define CTRX_DETAIL_EXCEPTION_TYPE_POSTCONDITION ::ctrx::postcondition_violation
#define CTRX_DETAIL_EXCEPTION_TYPE_ASSERTION ::ctrx::assertion_violation
#define CTRX_DETAIL_EXCEPTION_TYPE(TYPE) CTRX_DETAIL_CONCAT2(CTRX_DETAIL_EXCEPTION_TYPE_, TYPE)

// ------------------------------------------------------
// Map of contract type to enum type
// ------------------------------------------------------

#define CTRX_DETAIL_ENUM_TYPE_PRECONDITION ::ctrx::contract_type::precondition
#define CTRX_DETAIL_ENUM_TYPE_POSTCONDITION ::ctrx::contract_type::postcondition
#define CTRX_DETAIL_ENUM_TYPE_ASSERTION ::ctrx::contract_type::assertion
#define CTRX_DETAIL_ENUM_TYPE(TYPE) CTRX_DETAIL_CONCAT2(CTRX_DETAIL_ENUM_TYPE_, TYPE)

// ------------------------------------------------------
// Implementation of contract checks in all modes
// ------------------------------------------------------

// Evaluates if a contract check passes (returns an engaged optional with detail message on failure, nullopt on success)
#define CTRX_DETAIL_EXPR_FAILED(...)                                                                                   \
    [&]() -> std::optional<std::string>                                                                                \
    {                                                                                                                  \
        static_assert(std::convertible_to<decltype(__VA_ARGS__), bool>,                                                \
                      "contract expression must be convertible to bool");                                              \
        try                                                                                                            \
        {                                                                                                              \
            if (__VA_ARGS__)                                                                                           \
                return std::nullopt;                                                                                   \
        }                                                                                                              \
        catch (std::exception const& e)                                                                                \
        {                                                                                                              \
            return std::string(": ") + e.what();                                                                       \
        }                                                                                                              \
        catch (...)                                                                                                    \
        {                                                                                                              \
            return ": An exception was caught during contract check evaluation";                                       \
        }                                                                                                              \
        return "";                                                                                                     \
    }()

#define CTRX_DETAIL_CHECK_MODE_OFF(TYPE, MSG, ...) CTRX_DETAIL_CHECK_CODE_VALIDITY(__VA_ARGS__)
#define CTRX_DETAIL_CHECK_MODE_ASSERT(TYPE, MSG, ...) assert(!CTRX_DETAIL_EXPR_FAILED(__VA_ARGS__))
#define CTRX_DETAIL_CHECK_MODE_ASSUME(TYPE, MSG, ...) [[assume(__VA_ARGS__)]]
#define CTRX_DETAIL_CHECK_MODE_THROW(TYPE, MSG, ...)                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        if (auto msg = CTRX_DETAIL_EXPR_FAILED(__VA_ARGS__); msg)                                                      \
            throw CTRX_DETAIL_EXCEPTION_TYPE(TYPE){CTRX_DETAIL_STRINGIFY2(TYPE) " failure: " #__VA_ARGS__ MSG + *msg,  \
                                                   std::source_location::current()};                                   \
    } while (false)
#define CTRX_DETAIL_CHECK_MODE_TERMINATE(TYPE, MSG, ...)                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        if (CTRX_DETAIL_EXPR_FAILED(__VA_ARGS__))                                                                      \
            std::terminate();                                                                                          \
    } while (false)
#define CTRX_DETAIL_CHECK_MODE_HANDLER(TYPE, MSG, ...)                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        auto msg = CTRX_DETAIL_EXPR_FAILED(__VA_ARGS__);                                                               \
        if (std::is_constant_evaluated() && msg)                                                                       \
            std::abort();                                                                                              \
        else if (msg)                                                                                                  \
            ::ctrx::handle_contract_violation(CTRX_DETAIL_ENUM_TYPE(TYPE),                                             \
                                              #__VA_ARGS__ MSG + *msg,                                                 \
                                              std::source_location::current());                                        \
                                                                                                                       \
    } while (false)

// ------------------------------------------------------
// Implementation of contract checks in all levels
// ------------------------------------------------------

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_PRECONDITION) == CTRX_DETAIL_LEVEL_NUM_OFF
#define CTRX_DETAIL_CHECK_LEVEL_DEFAULT_TYPE_PRECONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#define CTRX_DETAIL_CHECK_LEVEL_AUDIT_TYPE_PRECONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#define CTRX_DETAIL_CHECK_LEVEL_AXIOM_TYPE_PRECONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_POSTCONDITION) == CTRX_DETAIL_LEVEL_NUM_OFF
#define CTRX_DETAIL_CHECK_LEVEL_DEFAULT_TYPE_POSTCONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#define CTRX_DETAIL_CHECK_LEVEL_AUDIT_TYPE_POSTCONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#define CTRX_DETAIL_CHECK_LEVEL_AXIOM_TYPE_POSTCONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_ASSERTION) == CTRX_DETAIL_LEVEL_NUM_OFF
#define CTRX_DETAIL_CHECK_LEVEL_DEFAULT_TYPE_ASSERTION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#define CTRX_DETAIL_CHECK_LEVEL_AUDIT_TYPE_ASSERTION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#define CTRX_DETAIL_CHECK_LEVEL_AXIOM_TYPE_ASSERTION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_PRECONDITION) == CTRX_DETAIL_LEVEL_NUM_DEFAULT
#define CTRX_DETAIL_CHECK_LEVEL_DEFAULT_TYPE_PRECONDITION(CHECKER) CHECKER
#define CTRX_DETAIL_CHECK_LEVEL_AUDIT_TYPE_PRECONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#define CTRX_DETAIL_CHECK_LEVEL_AXIOM_TYPE_PRECONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_POSTCONDITION) == CTRX_DETAIL_LEVEL_NUM_DEFAULT
#define CTRX_DETAIL_CHECK_LEVEL_DEFAULT_TYPE_POSTCONDITION(CHECKER) CHECKER
#define CTRX_DETAIL_CHECK_LEVEL_AUDIT_TYPE_POSTCONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#define CTRX_DETAIL_CHECK_LEVEL_AXIOM_TYPE_POSTCONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_ASSERTION) == CTRX_DETAIL_LEVEL_NUM_DEFAULT
#define CTRX_DETAIL_CHECK_LEVEL_DEFAULT_TYPE_ASSERTION(CHECKER) CHECKER
#define CTRX_DETAIL_CHECK_LEVEL_AUDIT_TYPE_ASSERTION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#define CTRX_DETAIL_CHECK_LEVEL_AXIOM_TYPE_ASSERTION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_PRECONDITION) == CTRX_DETAIL_LEVEL_NUM_AUDIT
#define CTRX_DETAIL_CHECK_LEVEL_DEFAULT_TYPE_PRECONDITION(CHECKER) CHECKER
#define CTRX_DETAIL_CHECK_LEVEL_AUDIT_TYPE_PRECONDITION(CHECKER) CHECKER
#define CTRX_DETAIL_CHECK_LEVEL_AXIOM_TYPE_PRECONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_POSTCONDITION) == CTRX_DETAIL_LEVEL_NUM_AUDIT
#define CTRX_DETAIL_CHECK_LEVEL_DEFAULT_TYPE_POSTCONDITION(CHECKER) CHECKER
#define CTRX_DETAIL_CHECK_LEVEL_AUDIT_TYPE_POSTCONDITION(CHECKER) CHECKER
#define CTRX_DETAIL_CHECK_LEVEL_AXIOM_TYPE_POSTCONDITION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_NUM_, CTRX_CONFIG_LEVEL_ASSERTION) == CTRX_DETAIL_LEVEL_NUM_AUDIT
#define CTRX_DETAIL_CHECK_LEVEL_DEFAULT_TYPE_ASSERTION(CHECKER) CHECKER
#define CTRX_DETAIL_CHECK_LEVEL_AUDIT_TYPE_ASSERTION(CHECKER) CHECKER
#define CTRX_DETAIL_CHECK_LEVEL_AXIOM_TYPE_ASSERTION(CHECKER) CTRX_DETAIL_CHECK_MODE_OFF
#endif

#define CTRX_DETAIL_CHECK_LEVEL_DEFAULT(CHECKER, TYPE)                                                                 \
    CTRX_DETAIL_CONCAT2(CTRX_DETAIL_CHECK_LEVEL_DEFAULT_TYPE_, TYPE)(CHECKER)
#define CTRX_DETAIL_CHECK_LEVEL_AUDIT(CHECKER, TYPE)                                                                   \
    CTRX_DETAIL_CONCAT2(CTRX_DETAIL_CHECK_LEVEL_AUDIT_TYPE_, TYPE)(CHECKER)
#define CTRX_DETAIL_CHECK_LEVEL_AXIOM(CHECKER, TYPE)                                                                   \
    CTRX_DETAIL_CONCAT2(CTRX_DETAIL_CHECK_LEVEL_AXIOM_TYPE_, TYPE)(CHECKER)

#define CTRX_DETAIL_CHECK_LEVEL(CHECKER, TYPE, LEVEL)                                                                  \
    CTRX_DETAIL_CONCAT2(CTRX_DETAIL_CHECK_LEVEL_, LEVEL)(CHECKER, TYPE)

// ------------------------------------------------------
// Map of types to canonical types (i.e. lowercase to uppercase)
// ------------------------------------------------------

#define CTRX_DETAIL_TYPE_PRECONDITION PRECONDITION
#define CTRX_DETAIL_TYPE_POSTCONDITION POSTCONDITION
#define CTRX_DETAIL_TYPE_ASSERTION ASSERTION
#define CTRX_DETAIL_TYPE_precondition CTRX_DETAIL_TYPE_PRECONDITION
#define CTRX_DETAIL_TYPE_postcondition CTRX_DETAIL_TYPE_POSTCONDITION
#define CTRX_DETAIL_TYPE_assertion CTRX_DETAIL_TYPE_ASSERTION
#define CTRX_DETAIL_TYPE(TYPE) CTRX_DETAIL_CONCAT2(CTRX_DETAIL_TYPE_, TYPE)

// ------------------------------------------------------
// Map of levels to canonical levels (i.e. lowercase to uppercase)
// ------------------------------------------------------

#define CTRX_DETAIL_LEVEL_DEFAULT DEFAULT
#define CTRX_DETAIL_LEVEL_AUDIT AUDIT
#define CTRX_DETAIL_LEVEL_AXIOM AXIOM
#define CTRX_DETAIL_LEVEL_default CTRX_DETAIL_LEVEL_DEFAULT
#define CTRX_DETAIL_LEVEL_audit CTRX_DETAIL_LEVEL_AUDIT
#define CTRX_DETAIL_LEVEL_axiom CTRX_DETAIL_LEVEL_AXIOM
#define CTRX_DETAIL_LEVEL(LEVEL) CTRX_DETAIL_CONCAT2(CTRX_DETAIL_LEVEL_, LEVEL)

// ------------------------------------------------------
// Implementation of the main macros
// ------------------------------------------------------

#define CTRX_DETAIL_GET_MODE_FROM_TYPE(TYPE) CTRX_DETAIL_CONCAT2(CTRX_CONFIG_MODE_, TYPE)
#define CTRX_DETAIL_GET_CHECKER(MODE) CTRX_DETAIL_CONCAT2(CTRX_DETAIL_CHECK_MODE_, MODE)
#define CTRX_DETAIL_FORMAT_MSG(...) "" __VA_OPT__(" (" __VA_ARGS__ ")")

#define CTRX_DETAIL_CONTRACT_4(TYPE, CONDITION, LEVEL, MESSAGE)                                                        \
    CTRX_DETAIL_CHECK_LEVEL(CTRX_DETAIL_GET_CHECKER(CTRX_DETAIL_GET_MODE_FROM_TYPE(CTRX_DETAIL_TYPE(TYPE))),           \
                            CTRX_DETAIL_TYPE(TYPE),                                                                    \
                            CTRX_DETAIL_LEVEL(LEVEL))                                                                  \
    (CTRX_DETAIL_TYPE(TYPE), CTRX_DETAIL_FORMAT_MSG(MESSAGE), CONDITION)
#define CTRX_DETAIL_CONTRACT_3(TYPE, CONDITION, LEVEL) CTRX_DETAIL_CONTRACT_4(TYPE, CONDITION, LEVEL, )
#define CTRX_DETAIL_CONTRACT_2(TYPE, CONDITION) CTRX_DETAIL_CONTRACT_3(TYPE, CONDITION, DEFAULT)
#define CTRX_CONTRACT(TYPE, ...)                                                                                       \
    CTRX_DETAIL_GET_OVERLOADED_MACRO_3(__VA_ARGS__,                                                                    \
                                       CTRX_DETAIL_CONTRACT_4,                                                         \
                                       CTRX_DETAIL_CONTRACT_3,                                                         \
                                       CTRX_DETAIL_CONTRACT_2)                                                         \
    (TYPE, __VA_ARGS__)

#define CTRX_PRECONDITION(...) CTRX_CONTRACT(PRECONDITION, __VA_ARGS__)
#define CTRX_POSTCONDITION(...) CTRX_CONTRACT(POSTCONDITION, __VA_ARGS__)
#define CTRX_ASSERT(...) CTRX_CONTRACT(ASSERTION, __VA_ARGS__)

#endif // CTRX_CONTRACTS_HPP
