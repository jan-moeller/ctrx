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
#define CTRX_DETAIL_MODE_OFF(TYPE, ...)
#define CTRX_DETAIL_MODE_ASSERT(TYPE, ...) assert((__VA_ARGS__))
#define CTRX_DETAIL_MODE_ASSUME(TYPE, ...) [[assume(__VA_ARGS__)]]
#define CTRX_DETAIL_MODE_THROW(TYPE, ...)                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(__VA_ARGS__))                                                                                            \
            throw CTRX_DETAIL_MODE_THROW_EXCEPTION(TYPE){#TYPE " failure: " #__VA_ARGS__};                             \
    } while (false)
#define CTRX_DETAIL_MODE_TERMINATE(TYPE, ...)                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(__VA_ARGS__))                                                                                            \
            std::terminate();                                                                                          \
    } while (false)
#define CTRX_DETAIL_MODE_HANDLER(TYPE, ...)                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(__VA_ARGS__))                                                                                            \
            CTRX_DETAIL_HANDLER(TYPE)(#__VA_ARGS__, std::source_location::current());                                  \
    } while (false)

// Helpers used above
#define CTRX_DETAIL_MODE_THROW_EXCEPTION_PRECONDITION ::ctrx::precondition_violation
#define CTRX_DETAIL_MODE_THROW_EXCEPTION_POSTCONDITION ::ctrx::postcondition_violation
#define CTRX_DETAIL_MODE_THROW_EXCEPTION_ASSERTION ::ctrx::assertion_violation
#define CTRX_DETAIL_MODE_THROW_EXCEPTION(TYPE) CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_THROW_EXCEPTION_, TYPE)

#define CTRX_DETAIL_HANDLER(TYPE) CTRX_DETAIL_CONCAT2(CTRX_CONFIG_HANDLER_, TYPE)

// Fall back to the global handler, if no specific one is set for ASSERTION/PRECONDITION/POSTCONDITION
#if !defined(CTRX_CONFIG_HANDLER_PRECONDITION)
#if defined(CTRX_CONFIG_HANDLER)
#define CTRX_CONFIG_HANDLER_PRECONDITION CTRX_CONFIG_HANDLER
#endif
#endif

#if !defined(CTRX_CONFIG_HANDLER_POSTCONDITION)
#if defined(CTRX_CONFIG_HANDLER)
#define CTRX_CONFIG_HANDLER_POSTCONDITION CTRX_CONFIG_HANDLER
#endif
#endif

#if !defined(CTRX_CONFIG_HANDLER_ASSERTION)
#if defined(CTRX_CONFIG_HANDLER)
#define CTRX_CONFIG_HANDLER_ASSERTION CTRX_CONFIG_HANDLER
#endif
#endif

// Same thing for handler configured handler includes
#if !defined(CTRX_CONFIG_HANDLER_INCLUDE_PRECONDITION)
#if defined(CTRX_CONFIG_HANDLER_INCLUDE)
#define CTRX_CONFIG_HANDLER_INCLUDE_PRECONDITION CTRX_CONFIG_HANDLER_INCLUDE
#endif
#endif

#if !defined(CTRX_CONFIG_HANDLER_INCLUDE_POSTCONDITION)
#if defined(CTRX_CONFIG_HANDLER_INCLUDE)
#define CTRX_CONFIG_HANDLER_INCLUDE_POSTCONDITION CTRX_CONFIG_HANDLER_INCLUDE
#endif
#endif

#if !defined(CTRX_CONFIG_HANDLER_INCLUDE_ASSERTION)
#if defined(CTRX_CONFIG_HANDLER_INCLUDE)
#define CTRX_CONFIG_HANDLER_INCLUDE_ASSERTION CTRX_CONFIG_HANDLER_INCLUDE
#endif
#endif

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
#if defined(CTRX_CONFIG_HANDLER_INCLUDE_PRECONDITION) && !defined(CTRX_DETAIL_NEED_INCLUDE_PRECONDITION_HANDLER)
#define CTRX_DETAIL_NEED_INCLUDE_PRECONDITION_HANDLER
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
#if defined(CTRX_CONFIG_HANDLER_INCLUDE_POSTCONDITION) && !defined(CTRX_DETAIL_NEED_INCLUDE_POSTCONDITION_HANDLER)
#define CTRX_DETAIL_NEED_INCLUDE_POSTCONDITION_HANDLER
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
#if defined(CTRX_CONFIG_ASSERT_HANDLER_INCLUDE) && !defined(CTRX_DETAIL_NEED_INCLUDE_ASSERT_HANDLER)
#define CTRX_DETAIL_NEED_INCLUDE_ASSERT_HANDLER
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
#include "ctrx/exceptions/postcondition_violation.hpp"
#include "ctrx/exceptions/precondition_violation.hpp"
#endif
#if defined(CTRX_DETAIL_NEED_INCLUDE_EXCEPTION)
#include <exception>
#endif
#if defined(CTRX_DETAIL_NEED_INCLUDE_SOURCE_LOCATION)
#include <source_location>
#endif
#if defined(CTRX_DETAIL_NEED_INCLUDE_PRECONDITION_HANDLER)
#include CTRX_CONFIG_HANDLER_INCLUDE_PRECONDITION
#endif
#if defined(CTRX_DETAIL_NEED_INCLUDE_POSTCONDITION_HANDLER)
#include CTRX_CONFIG_HANDLER_INCLUDE_POSTCONDITION
#endif
#if defined(CTRX_DETAIL_NEED_INCLUDE_ASSERT_HANDLER)
#include CTRX_CONFIG_ASSERT_HANDLER_INCLUDE
#endif

// Some helpers used below
#define CTRX_DETAIL_GET_CONFIG_MODE(TYPE) CTRX_DETAIL_CONCAT2(CTRX_CONFIG_MODE_, TYPE)
#define CTRX_DETAIL_CHECK_FN(TYPE) CTRX_DETAIL_CONCAT2(CTRX_DETAIL_MODE_, CTRX_DETAIL_GET_CONFIG_MODE(TYPE))
#define CTRX_DETAIL_CHECK(TYPE, ...) CTRX_DETAIL_CHECK_FN(TYPE)(TYPE, __VA_ARGS__)

// The main macros
#define CTRX_PRECONDITION(...) CTRX_DETAIL_CHECK(PRECONDITION, __VA_ARGS__)
#define CTRX_POSTCONDITION(...) CTRX_DETAIL_CHECK(POSTCONDITION, __VA_ARGS__)
#define CTRX_ASSERT(...) CTRX_DETAIL_CHECK(ASSERTION, __VA_ARGS__)

#endif // CTRX_CONTRACTS_HPP
