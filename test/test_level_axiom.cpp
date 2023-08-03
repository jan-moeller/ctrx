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
#define CTRX_CONFIG_LEVEL AUDIT
#include "ctrx/contracts.hpp"

#include <bugspray/bugspray.hpp>

void precondition_failure()
{
    CTRX_PRECONDITION(DEFAULT, false);
}
void postcondition_failure()
{
    CTRX_POSTCONDITION(AUDIT, false);
}
void assertion_failure()
{
    CTRX_ASSERT(AXIOM, false);
}

TEST_CASE("level: axiom", "[ctrx]", runtime)
{
    CHECK_THROWS_AS(ctrx::precondition_violation, precondition_failure());
    CHECK_THROWS_AS(ctrx::postcondition_violation, postcondition_failure());
    CHECK_NOTHROW(assertion_failure());
}
