cmake_minimum_required(VERSION 3.14)
project(ctrx
        VERSION 1.2.0
        LANGUAGES CXX
)

#############################################################################################################
# Prevent in-source builds
#############################################################################################################
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

#############################################################################################################
# Dependencies
#############################################################################################################
include(cmake/CPM.cmake)

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.11.0")

#############################################################################################################
# Build configuration
#############################################################################################################
set(CTRX_CONFIG_MODE CACHE STRING "Set one of the ctrx modes: OFF, ASSERT, ASSUME, THROW, TERMINATE, HANDLER")
set(CTRX_CONFIG_MODE_PRECONDITION CACHE STRING "Set one of the ctrx modes: OFF, ASSERT, ASSUME, THROW, TERMINATE, HANDLER (or leave empty to use global mode)")
set(CTRX_CONFIG_MODE_POSTCONDITION CACHE STRING "Set one of the ctrx modes: OFF, ASSERT, ASSUME, THROW, TERMINATE, HANDLER (or leave empty to use global mode)")
set(CTRX_CONFIG_MODE_ASSERTION CACHE STRING "Set one of the ctrx modes: OFF, ASSERT, ASSUME, THROW, TERMINATE, HANDLER (or leave empty to use global mode)")

message(STATUS "------------------------------------------------------------------------------")
message(STATUS "    ${PROJECT_NAME} (${PROJECT_VERSION})")
message(STATUS "------------------------------------------------------------------------------")
message(STATUS "Build type:                ${CMAKE_BUILD_TYPE}")
message(STATUS "Global mode:               ${CTRX_CONFIG_MODE}")
message(STATUS "  - Precondition mode:     ${CTRX_CONFIG_MODE_PRECONDITION}")
message(STATUS "  - Postcondition mode:    ${CTRX_CONFIG_MODE_POSTCONDITION}")
message(STATUS "  - Assertion mode:        ${CTRX_CONFIG_MODE_ASSERTION}")


#############################################################################################################
# Main library target
#############################################################################################################
add_library(${PROJECT_NAME} INTERFACE
        include/ctrx/contract_type.hpp
        include/ctrx/contracts.hpp
        include/ctrx/exceptions/assertion_violation.hpp
        include/ctrx/exceptions/contract_violation.hpp
        include/ctrx/exceptions/postcondition_violation.hpp
        include/ctrx/exceptions/precondition_violation.hpp
)
target_include_directories(
        ${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
if (NOT CTRX_CONFIG_MODE STREQUAL "")
    target_compile_definitions(${PROJECT_NAME} INTERFACE CTRX_CONFIG_MODE=${CTRX_CONFIG_MODE})
endif ()
if (NOT CTRX_CONFIG_MODE_PRECONDITION STREQUAL "")
    target_compile_definitions(${PROJECT_NAME} INTERFACE CTRX_CONFIG_MODE_PRECONDITION=${CTRX_CONFIG_MODE_PRECONDITION})
endif ()
if (NOT CTRX_CONFIG_MODE_POSTCONDITION STREQUAL "")
    target_compile_definitions(${PROJECT_NAME} INTERFACE CTRX_CONFIG_MODE_POSTCONDITION=${CTRX_CONFIG_MODE_POSTCONDITION})
endif ()
if (NOT CTRX_CONFIG_MODE_ASSERTION STREQUAL "")
    target_compile_definitions(${PROJECT_NAME} INTERFACE CTRX_CONFIG_MODE_ASSERTION=${CTRX_CONFIG_MODE_ASSERTION})
endif ()

string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)
packageProject(
        NAME ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        NAMESPACE ${PROJECT_NAME}
        BINARY_DIR ${PROJECT_BINARY_DIR}
        INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
        INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
        VERSION_HEADER "${VERSION_HEADER_LOCATION}"
        COMPATIBILITY SameMajorVersion
        DEPENDENCIES ""
)

