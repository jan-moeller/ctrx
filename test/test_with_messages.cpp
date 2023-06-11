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

void precondition_failure()
{
    CTRX_PRECONDITION_M("foo", false);
}
void postcondition_failure()
{
    CTRX_POSTCONDITION_M("bar", false);
}
void assertion_failure()
{
    CTRX_ASSERT_M("baz", false);
}

TEST_CASE("mode: throw", "[ctrx]", runtime)
{
    try
    {
        precondition_failure();
    }
    catch (ctrx::precondition_violation const& e)
    {
        CHECK(e.what() == "PRECONDITION failure: false (foo)");
    }
    try
    {
        postcondition_failure();
    }
    catch (ctrx::postcondition_violation const& e)
    {
        CHECK(e.what() == "POSTCONDITION failure: false (bar)");
    }
    try
    {
        assertion_failure();
    }
    catch (ctrx::assertion_violation const& e)
    {
        CHECK(e.what() == "ASSERTION failure: false (baz)");
    }
}
