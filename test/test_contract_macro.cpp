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

void type_condition_level_message()
{
    CTRX_CONTRACT(assertion, false, default, "a message");
}

void type_condition_level()
{
    CTRX_CONTRACT(assertion, false, default);
}

void type_condition()
{
    CTRX_CONTRACT(assertion, false);
}

void type_condition_level_message_caps()
{
    CTRX_CONTRACT(ASSERTION, false, DEFAULT, "a message");
}

void type_condition_level_caps()
{
    CTRX_CONTRACT(ASSERTION, false, DEFAULT);
}

void type_condition_caps()
{
    CTRX_CONTRACT(ASSERTION, false);
}

TEST_CASE("contract macro", "[ctrx]", runtime)
{
    CHECK_THROWS_AS(ctrx::assertion_violation, type_condition_level_message());
    CHECK_THROWS_AS(ctrx::assertion_violation, type_condition_level());
    CHECK_THROWS_AS(ctrx::assertion_violation, type_condition());

    CHECK_THROWS_AS(ctrx::assertion_violation, type_condition_level_message_caps());
    CHECK_THROWS_AS(ctrx::assertion_violation, type_condition_level_caps());
    CHECK_THROWS_AS(ctrx::assertion_violation, type_condition_caps());
}
