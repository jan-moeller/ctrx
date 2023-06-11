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

// Helper macro to concatenate tokens
#define CTRX_DETAIL_CONCAT(A, B) A##B
#define CTRX_DETAIL_CONCAT2(A, B) CTRX_DETAIL_CONCAT(A, B)

// Assigning numbers to modes so we can preprocessor-compare them
#define CTRX_DETAIL_MODE_NUM_OFF 0
#define CTRX_DETAIL_MODE_NUM_ASSERT 1
#define CTRX_DETAIL_MODE_NUM_ASSUME 2
#define CTRX_DETAIL_MODE_NUM_THROW 3
#define CTRX_DETAIL_MODE_NUM_TERMINATE 4
#define CTRX_DETAIL_MODE_NUM_HANDLER 5

// Definition of what the different modes do
#define CTRX_DETAIL_MODE_OFF(TYPE, MSG, ...)
#define CTRX_DETAIL_MODE_ASSERT(TYPE, MSG, ...) assert((__VA_ARGS__))
#define CTRX_DETAIL_MODE_ASSUME(TYPE, MSG, ...) [[assume(__VA_ARGS__)]]
#define CTRX_DETAIL_MODE_THROW(TYPE, MSG, ...)                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(__VA_ARGS__))                                                                                            \
            throw CTRX_DETAIL_MODE_THROW_EXCEPTION(TYPE){#TYPE " failure: " #__VA_ARGS__ MSG,                          \
                                                         std::source_location::current()};                             \
    } while (false)
#define CTRX_DETAIL_MODE_TERMINATE(TYPE, MSG, ...)                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(__VA_ARGS__))                                                                                            \
            std::terminate();                                                                                          \
    } while (false)
#define CTRX_DETAIL_MODE_HANDLER(TYPE, MSG, ...)                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        if (std::is_constant_evaluated())                                                                              \
            assert((__VA_ARGS__));                                                                                     \
        else if (!(__VA_ARGS__))                                                                                       \
            ::ctrx::handle_contract_violation(CTRX_DETAIL_TYPE_ENUM(TYPE),                                             \
                                              #__VA_ARGS__ MSG,                                                        \
                                              std::source_location::current());                                        \
                                                                                                                       \
    } while (false)

// Helpers used above
#define CTRX_DETAIL_MODE_THROW_EXCEPTION_PRECONDITION ::ctrx::precondition_violation
#define CTRX_DETAIL_MODE_THROW_EXCEPTION_POSTCONDITION ::ctrx::postcondition_violation
#define CTRX_DETAIL_MODE_THROW_EXCEPTION_ASSERTION ::ctrx::assertion_violation
#define CTRX_DETAIL_MODE_THROW_EXCEPTION(TYPE) CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_THROW_EXCEPTION_, TYPE)

// If no global mode is set use ASSERT
#if !defined(CTRX_CONFIG_MODE)
#define CTRX_CONFIG_MODE ASSERT
#endif

// Fall back to global mode, if no specific mode is set for ASSERTION/PRECONDITION/POSTCONDITION
#if !defined(CTRX_CONFIG_MODE_PRECONDITION)
#if defined(CTRX_CONFIG_MODE)
#define CTRX_CONFIG_MODE_PRECONDITION CTRX_CONFIG_MODE
#endif
#endif

#if !defined(CTRX_CONFIG_MODE_POSTCONDITION)
#if defined(CTRX_CONFIG_MODE)
#define CTRX_CONFIG_MODE_POSTCONDITION CTRX_CONFIG_MODE
#endif
#endif

#if !defined(CTRX_CONFIG_MODE_ASSERTION)
#if defined(CTRX_CONFIG_MODE)
#define CTRX_CONFIG_MODE_ASSERTION CTRX_CONFIG_MODE
#endif
#endif

// Figure out what #includes are required according to the configured mode
#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_ASSERT
#if !defined(CTRX_DETAIL_NEED_INCLUDE_ASSERT)
#define CTRX_DETAIL_NEED_INCLUDE_ASSERT
#endif
#elif CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_THROW
#if !defined(CTRX_DETAIL_NEED_INCLUDE_ASSERTION_EXCEPTIONS)
#define CTRX_DETAIL_NEED_INCLUDE_ASSERTION_EXCEPTIONS
#endif
#elif CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_HANDLER
#if !defined(CTRX_DETAIL_NEED_INCLUDE_SOURCE_LOCATION)
#define CTRX_DETAIL_NEED_INCLUDE_SOURCE_LOCATION
#endif
#elif CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_TERMINATE
#if !defined(CTRX_DETAIL_NEED_INCLUDE_EXCEPTION)
#define CTRX_DETAIL_NEED_INCLUDE_EXCEPTION
#endif
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == CTRX_DETAIL_MODE_NUM_ASSERT
#if !defined(CTRX_DETAIL_NEED_INCLUDE_ASSERT)
#define CTRX_DETAIL_NEED_INCLUDE_ASSERT
#endif
#elif CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == CTRX_DETAIL_MODE_NUM_THROW
#if !defined(CTRX_DETAIL_NEED_INCLUDE_ASSERTION_EXCEPTIONS)
#define CTRX_DETAIL_NEED_INCLUDE_ASSERTION_EXCEPTIONS
#endif
#elif CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == CTRX_DETAIL_MODE_NUM_HANDLER
#if !defined(CTRX_DETAIL_NEED_INCLUDE_SOURCE_LOCATION)
#define CTRX_DETAIL_NEED_INCLUDE_SOURCE_LOCATION
#endif
#elif CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_TERMINATE
#if !defined(CTRX_DETAIL_NEED_INCLUDE_EXCEPTION)
#define CTRX_DETAIL_NEED_INCLUDE_EXCEPTION
#endif
#endif

#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == CTRX_DETAIL_MODE_NUM_ASSERT
#if !defined(CTRX_DETAIL_NEED_INCLUDE_ASSERT)
#define CTRX_DETAIL_NEED_INCLUDE_ASSERT
#endif
#elif CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == CTRX_DETAIL_MODE_NUM_THROW
#if !defined(CTRX_DETAIL_NEED_INCLUDE_ASSERTION_EXCEPTIONS)
#define CTRX_DETAIL_NEED_INCLUDE_ASSERTION_EXCEPTIONS
#endif
#elif CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == CTRX_DETAIL_MODE_NUM_HANDLER
#if !defined(CTRX_DETAIL_NEED_INCLUDE_SOURCE_LOCATION)
#define CTRX_DETAIL_NEED_INCLUDE_SOURCE_LOCATION
#endif
#elif CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_TERMINATE
#if !defined(CTRX_DETAIL_NEED_INCLUDE_EXCEPTION)
#define CTRX_DETAIL_NEED_INCLUDE_EXCEPTION
#endif
#endif

// #include the required headers
#if defined(CTRX_DETAIL_NEED_INCLUDE_ASSERT)
#include <cassert>
#endif
#if defined(CTRX_DETAIL_NEED_INCLUDE_ASSERTION_EXCEPTIONS)
#include "ctrx/exceptions/assertion_violation.hpp"
#include "ctrx/exceptions/contract_violation.hpp"
#include "ctrx/exceptions/postcondition_violation.hpp"
#include "ctrx/exceptions/precondition_violation.hpp"
#endif
#if defined(CTRX_DETAIL_NEED_INCLUDE_EXCEPTION)
#include <exception>
#endif
#if defined(CTRX_DETAIL_NEED_INCLUDE_SOURCE_LOCATION)
#include <source_location>
#endif

// Some helpers used below
#define CTRX_DETAIL_GET_CONFIG_MODE(TYPE) CTRX_DETAIL_CONCAT2(CTRX_CONFIG_MODE_, TYPE)
#define CTRX_DETAIL_CHECK_FN(TYPE) CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_, CTRX_DETAIL_GET_CONFIG_MODE(TYPE))
#define CTRX_DETAIL_CHECK(TYPE, MSG, ...) CTRX_DETAIL_CHECK_FN(TYPE)(TYPE, CTRX_DETAIL_FORMAT_MSG(MSG), __VA_ARGS__)
#define CTRX_DETAIL_FORMAT_MSG(...) "" __VA_OPT__(" (" __VA_ARGS__ ")")

// The main macros
#define CTRX_PRECONDITION_M(MSG, ...) CTRX_DETAIL_CHECK(PRECONDITION, MSG, __VA_ARGS__)
#define CTRX_POSTCONDITION_M(MSG, ...) CTRX_DETAIL_CHECK(POSTCONDITION, MSG, __VA_ARGS__)
#define CTRX_ASSERT_M(MSG, ...) CTRX_DETAIL_CHECK(ASSERTION, MSG, __VA_ARGS__)

#define CTRX_PRECONDITION(...) CTRX_PRECONDITION_M(, __VA_ARGS__)
#define CTRX_POSTCONDITION(...) CTRX_POSTCONDITION_M(, __VA_ARGS__)
#define CTRX_ASSERT(...) CTRX_ASSERT_M(, __VA_ARGS__)

// Define handler entry point if required
#if CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_PRECONDITION) == CTRX_DETAIL_MODE_NUM_HANDLER          \
    || CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_POSTCONDITION) == CTRX_DETAIL_MODE_NUM_HANDLER      \
    || CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_NUM_, CTRX_CONFIG_MODE_ASSERTION) == CTRX_DETAIL_MODE_NUM_HANDLER
#define CTRX_DETAIL_NEED_CONTRACT_VIOLATION_HANDLER_ENTRY_POINT
#endif
#if defined(CTRX_DETAIL_NEED_CONTRACT_VIOLATION_HANDLER_ENTRY_POINT)
#define CTRX_DETAIL_TYPE_ENUM_PRECONDITION ::ctrx::contract_type::precondition
#define CTRX_DETAIL_TYPE_ENUM_POSTCONDITION ::ctrx::contract_type::postcondition
#define CTRX_DETAIL_TYPE_ENUM_ASSERTION ::ctrx::contract_type::assertion
#define CTRX_DETAIL_TYPE_ENUM(TYPE) CTRX_DETAIL_CONCAT2(CTRX_DETAIL_TYPE_ENUM_, TYPE)
namespace ctrx
{
extern void handle_contract_violation(contract_type, char const*, std::source_location const&);
} // namespace ctrx
#endif

#endif // CTRX_CONTRACTS_HPP
