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

#ifndef CTRX_TESTS_CONTRACT_VIOLATION_HPP
#define CTRX_TESTS_CONTRACT_VIOLATION_HPP

#include "ctrx/contract_type.hpp"

#include <source_location>
#include <utility>

namespace ctrx
{
class contract_violation
{
  public:
    constexpr explicit contract_violation(contract_type type, char const* what, std::source_location sloc)
        : m_type(type)
        , m_what(what)
        , m_sloc(std::move(sloc))
    {
    }

    [[nodiscard]] constexpr auto type() const noexcept -> contract_type { return m_type; }
    [[nodiscard]] constexpr auto what() const noexcept -> char const* { return m_what; }
    [[nodiscard]] constexpr auto source_location() const noexcept -> std::source_location const& { return m_sloc; }

  private:
    contract_type        m_type;
    char const*          m_what;
    std::source_location m_sloc;
};
} // namespace ctrx

#endif // CTRX_TESTS_CONTRACT_VIOLATION_HPP
