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
#include "ctrx/contracts.hpp"

#include <bugspray/bugspray.hpp>

#include <regex>

TEST_CASE("with messages", "[ctrx]", runtime)
{
    SECTION("with messages")
    {
        try
        {
            CTRX_PRECONDITION(false, default, "foo");
        }
        catch (ctrx::precondition_violation const& e)
        {
            CAPTURE(e.what());
            CHECK(std::regex_match(e.what(),
                                   std::regex(
                                       R"(^.*test_with_messages.cpp:36.*PRECONDITION failure: false \(foo\).*$)")));
        }
        try
        {
            CTRX_POSTCONDITION(false, default, "bar");
        }
        catch (ctrx::postcondition_violation const& e)
        {
            CAPTURE(e.what());
            CHECK(std::regex_match(e.what(),
                                   std::regex(
                                       R"(^.*test_with_messages.cpp:47.*POSTCONDITION failure: false \(bar\).*$)")));
        }
        try
        {
            CTRX_ASSERT(false, default, "baz");
        }
        catch (ctrx::assertion_violation const& e)
        {
            CAPTURE(e.what());
            CHECK(std::regex_match(e.what(),
                                   std::regex(R"(^.*test_with_messages.cpp:58.*ASSERTION failure: false \(baz\).*$)")));
        }
    }
    SECTION("without messages")
    {
        try
        {
            CTRX_PRECONDITION(false, default);
        }
        catch (ctrx::precondition_violation const& e)
        {
            CAPTURE(e.what());
            CHECK(std::regex_match(e.what(),
                                   std::regex(R"(^.*test_with_messages.cpp:71.*PRECONDITION failure: false.*$)")));
        }
        try
        {
            CTRX_POSTCONDITION(false, default);
        }
        catch (ctrx::postcondition_violation const& e)
        {
            CAPTURE(e.what());
            CHECK(std::regex_match(e.what(),
                                   std::regex(R"(^.*test_with_messages.cpp:81.*POSTCONDITION failure: false.*$)")));
        }
        try
        {
            CTRX_ASSERT(false, default);
        }
        catch (ctrx::assertion_violation const& e)
        {
            CAPTURE(e.what());
            CHECK(
                std::regex_match(e.what(), std::regex(R"(^.*test_with_messages.cpp:91.*ASSERTION failure: false.*$)")));
        }
    }
}
