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
#include <bugspray/bugspray.hpp>

#define CTRX_CONFIG_MODE HANDLER
#define CTRX_CONFIG_HANDLER_PRECONDITION precondition_handler
#define CTRX_CONFIG_HANDLER_POSTCONDITION postcondition_handler
#define CTRX_CONFIG_HANDLER_ASSERTION assertion_handler
#include "ctrx/contracts.hpp"

char const* pre_msg    = nullptr;
char const* post_msg   = nullptr;
char const* assert_msg = nullptr;

constexpr void precondition_handler(char const* s, std::source_location)
{
    if (!std::is_constant_evaluated())
        pre_msg = s;
}
constexpr void postcondition_handler(char const* s, std::source_location)
{
    if (!std::is_constant_evaluated())
        post_msg = s;
}
constexpr void assertion_handler(char const* s, std::source_location)
{
    if (!std::is_constant_evaluated())
        assert_msg = s;
}

TEST_CASE("mode: handler", "[ctrx]")
{
    SECTION("pre")
    {
        if (!std::is_constant_evaluated())
        {
            pre_msg    = nullptr;
            post_msg   = nullptr;
            assert_msg = nullptr;
        }
        CTRX_PRECONDITION(true);
        if (!std::is_constant_evaluated())
        {
            CHECK(pre_msg == nullptr);
            CHECK(post_msg == nullptr);
            CHECK(assert_msg == nullptr);
        }
        CTRX_PRECONDITION(false);
        if (!std::is_constant_evaluated())
        {
            CHECK(pre_msg == "false");
            CHECK(post_msg == nullptr);
            CHECK(assert_msg == nullptr);
        }
    }
    SECTION("post")
    {
        if (!std::is_constant_evaluated())
        {
            pre_msg    = nullptr;
            post_msg   = nullptr;
            assert_msg = nullptr;
        }
        CTRX_POSTCONDITION(true);
        if (!std::is_constant_evaluated())
        {
            CHECK(pre_msg == nullptr);
            CHECK(post_msg == nullptr);
            CHECK(assert_msg == nullptr);
        }
        CTRX_POSTCONDITION(false);
        if (!std::is_constant_evaluated())
        {
            CHECK(pre_msg == nullptr);
            CHECK(post_msg == "false");
            CHECK(assert_msg == nullptr);
        }
    }
    SECTION("assert")
    {
        if (!std::is_constant_evaluated())
        {
            pre_msg    = nullptr;
            post_msg   = nullptr;
            assert_msg = nullptr;
        }
        CTRX_ASSERT(true);
        if (!std::is_constant_evaluated())
        {
            CHECK(pre_msg == nullptr);
            CHECK(post_msg == nullptr);
            CHECK(assert_msg == nullptr);
        }
        CTRX_ASSERT(false);
        if (!std::is_constant_evaluated())
        {
            CHECK(pre_msg == nullptr);
            CHECK(post_msg == nullptr);
            CHECK(assert_msg == "false");
        }
    }
}
EVAL_TEST_CASE("mode: handler");