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

#undef CTRX_CONFIG_MODE
#define CTRX_CONFIG_MODE HANDLER
#include "ctrx/contracts.hpp"

std::string pre_msg    = "";
std::string post_msg   = "";
std::string assert_msg = "";

namespace ctrx
{
void handle_contract_violation(contract_type t, std::string_view s, std::source_location const&)
{
    switch (t)
    {
    case contract_type::precondition:
        pre_msg = s;
        break;
    case contract_type::postcondition:
        post_msg = s;
        break;
    case contract_type::assertion:
        assert_msg = s;
        break;
    }
}
} // namespace ctrx

auto throws() -> bool
{
    throw std::runtime_error{"what message"};
    return true;
}

TEST_CASE("mode: handler", "[ctrx]", runtime)
{
    SECTION("pre")
    {
        pre_msg    = "";
        post_msg   = "";
        assert_msg = "";
        CTRX_PRECONDITION(true);
        CHECK(pre_msg == "");
        CHECK(post_msg == "");
        CHECK(assert_msg == "");
        CTRX_PRECONDITION(false);
        CHECK(pre_msg == "false");
        CHECK(post_msg == "");
        CHECK(assert_msg == "");
        CTRX_PRECONDITION(throws());
        CHECK(pre_msg == "throws(): what message");
        CHECK(post_msg == "");
        CHECK(assert_msg == "");
    }
    SECTION("post")
    {
        pre_msg    = "";
        post_msg   = "";
        assert_msg = "";
        CTRX_POSTCONDITION(true);
        CHECK(pre_msg == "");
        CHECK(post_msg == "");
        CHECK(assert_msg == "");
        CTRX_POSTCONDITION(false);
        CHECK(pre_msg == "");
        CHECK(post_msg == "false");
        CHECK(assert_msg == "");
        CTRX_POSTCONDITION(throws());
        CHECK(pre_msg == "");
        CHECK(post_msg == "throws(): what message");
        CHECK(assert_msg == "");
    }
    SECTION("assert")
    {
        pre_msg    = "";
        post_msg   = "";
        assert_msg = "";
        CTRX_ASSERT(true);
        CHECK(pre_msg == "");
        CHECK(post_msg == "");
        CHECK(assert_msg == "");
        CTRX_ASSERT(false);
        CHECK(pre_msg == "");
        CHECK(post_msg == "");
        CHECK(assert_msg == "false");
        CTRX_ASSERT(throws());
        CHECK(pre_msg == "");
        CHECK(post_msg == "");
        CHECK(assert_msg == "throws(): what message");
    }
}

TEST_CASE("mode: handler (constexpr)", "[ctrx]", compiletime)
{
    // Can't test negative case since that would be a compile error
    CTRX_PRECONDITION(true);
    CTRX_POSTCONDITION(true);
    CTRX_ASSERT(true);
}
EVAL_TEST_CASE("mode: handler (constexpr)");
