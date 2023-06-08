#
# MIT License
#
# Copyright (c) 2023 Jan Möller
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
add_library(libinterface INTERFACE libinterface.hpp)
target_link_libraries(libinterface INTERFACE ctrx::ctrx)
set_target_properties(libinterface PROPERTIES CXX_STANDARD 20)
target_include_directories(libinterface INTERFACE .)

add_library(libstatic STATIC libstatic.cpp)
target_link_libraries(libstatic PUBLIC ctrx::ctrx libinterface)
set_target_properties(libstatic PROPERTIES CXX_STANDARD 20)

add_library(libshared SHARED libshared.cpp)
target_link_libraries(libshared PUBLIC ctrx::ctrx libinterface)
set_target_properties(libshared PROPERTIES CXX_STANDARD 20)

add_executable(ctrx-test-with-deps complex_test.cpp)
target_link_libraries(ctrx-test-with-deps PUBLIC ctrx::ctrx libinterface bugspray-with-main libstatic libshared)
set_target_properties(ctrx-test-with-deps PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
add_test(NAME ctrx-test-with-deps COMMAND ctrx-test-with-deps)