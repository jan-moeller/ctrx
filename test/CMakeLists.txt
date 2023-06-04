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
CPMAddPackage("gh:jan-moeller/bugspray@0.2.0")

function(create_test name)
    set(TEST_EXEC_NAME ${PROJECT_NAME}-tests-${name})
    add_executable(${TEST_EXEC_NAME}
            test_${name}.cpp
    )
    target_link_libraries(${TEST_EXEC_NAME} PUBLIC bugspray-with-main ${PROJECT_NAME})
    set_target_properties(${TEST_EXEC_NAME} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )
    target_compile_options(
            ${TEST_EXEC_NAME}
            PUBLIC
            "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->" # Turn off permissive mode on MSVC
            "$<$<COMPILE_LANG_AND_ID:CXX,GCC>:-Wall -Wextra -pedantic -Werror>"
    )

    add_test(NAME ${TEST_EXEC_NAME} COMMAND ${TEST_EXEC_NAME})
endfunction()

create_test(mode_off)
create_test(mode_throw)
create_test(mode_handler)

create_test(fibonacci)