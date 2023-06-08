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
#include "ctrx/exceptions/precondition_violation.hpp"
#include "libinterface.hpp"

#include <bugspray/bugspray.hpp>
#include <ctrx/contracts.hpp>

#include <filesystem>
#include <string_view>

extern auto bar(int) -> int;
extern auto baz(int) -> int;

auto bam(int i) -> int
{
    CTRX_PRECONDITION(i != 5);
    return foo(i) && bar(i - 1) && baz(i - 2);
}

TEST_CASE("check", "", runtime)
{
    using namespace std::string_view_literals;

    try
    {
        foo(0);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "libinterface.hpp"sv);
    }

    try
    {
        bar(0);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "libinterface.hpp"sv);
    }

    try
    {
        bar(1);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "libstatic.cpp"sv);
    }

    try
    {
        baz(0);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "libinterface.hpp"sv);
    }

    try
    {
        baz(2);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "libshared.cpp"sv);
    }

    try
    {
        bam(0);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "libinterface.hpp"sv);
    }
    try
    {
        bam(1);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "libinterface.hpp"sv);
    }

    try
    {
        bam(2);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "libstatic.cpp"sv);
    }

    try
    {
        bam(3);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "libstatic.cpp"sv);
    }
    try
    {
        bam(4);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "libshared.cpp"sv);
    }
    try
    {
        bam(5);
    }
    catch (ctrx::precondition_violation const& e)
    {
        std::filesystem::path p{e.source_location().file_name()};
        REQUIRE(p.filename().c_str() == "complex_test.cpp"sv);
    }
}