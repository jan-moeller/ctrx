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
#include "ctrx/contracts.hpp"

char const* pre_msg    = nullptr;
char const* post_msg   = nullptr;
char const* assert_msg = nullptr;

namespace ctrx
{
void handle_contract_violation(contract_type t, char const* s, std::source_location const&)
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

TEST_CASE("mode: handler", "[ctrx]", runtime)
{
    SECTION("pre")
    {
        pre_msg    = nullptr;
        post_msg   = nullptr;
        assert_msg = nullptr;
        CTRX_PRECONDITION(true);
        CHECK(pre_msg == nullptr);
        CHECK(post_msg == nullptr);
        CHECK(assert_msg == nullptr);
        CTRX_PRECONDITION(false);
        CHECK(pre_msg == "false");
        CHECK(post_msg == nullptr);
        CHECK(assert_msg == nullptr);
    }
    SECTION("post")
    {
        pre_msg    = nullptr;
        post_msg   = nullptr;
        assert_msg = nullptr;
        CTRX_POSTCONDITION(true);
        CHECK(pre_msg == nullptr);
        CHECK(post_msg == nullptr);
        CHECK(assert_msg == nullptr);
        CTRX_POSTCONDITION(false);
        CHECK(pre_msg == nullptr);
        CHECK(post_msg == "false");
        CHECK(assert_msg == nullptr);
    }
    SECTION("assert")
    {
        pre_msg    = nullptr;
        post_msg   = nullptr;
        assert_msg = nullptr;
        CTRX_ASSERT(true);
        CHECK(pre_msg == nullptr);
        CHECK(post_msg == nullptr);
        CHECK(assert_msg == nullptr);
        CTRX_ASSERT(false);
        CHECK(pre_msg == nullptr);
        CHECK(post_msg == nullptr);
        CHECK(assert_msg == "false");
    }
}