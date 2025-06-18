cmake_minimum_required(VERSION 3.14)
set (LIB_NAME "console_parser")

# Custom function used to declare a test case
# Takes the name of the test (also the name of the resulting executable),
# the files needed for that test and the main directory of the test (this is used
# to place the test in the correct bin/tests/ directory)
function(declare_test test_name test_files test_dir)
    add_executable(
        ${test_name}
        ${test_files}
    )
    target_link_libraries(
        ${test_name}
        GTest::gtest
        GTest::gmock_main
        GTest::gtest_main
    )
    message(STATUS "Cmake runtime output directory is: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    add_test(NAME ${test_name} COMMAND ${test_name} WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/${test_dir}/")
    set_target_properties(${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/${test_dir}/)
    gtest_discover_tests(${test_name} WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/${test_dir}/")
endfunction()

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()

option(BUILD_WITH_CONSOLE_PARSER_TESTS "Enable the tests of the library when building" OFF)

if(BUILD_WITH_CONSOLE_PARSER_TESTS)
    message(STATUS "Tests for the console parser lib are enabled!")
else()
    message(STATUS "Tests for the console parser lib are not enabled! To enable them cmake with -DBUILD_WITH_CONSOLE_PARSER_TESTS=On")
endif()

if(BUILD_WITH_CONSOLE_PARSER_TESTS)
    include(FetchContent)
    if(CMAKE_VERSION VERSION_GREATER "3.23.0")
        # When downloading content with the FetchContent_Declare, the new versions
        # use another policy regarding the timestamps in the archive being downloaded
        # For more information: https://cmake.org/cmake/help/latest/policy/CMP0135.html
        set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
        message(STATUS "CMAKE CMP0135 policy set to NEW")
    endif()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    message(STATUS "Fething the google test repository...")
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    )
    # For windows prevent overriding the parent project's compiler/linker settings
    if(WIN32)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    include(GoogleTest)
    include_directories(${googletest_SOURCE_DIR}/googletest/include ${googletest_SOURCE_DIR}/googletest)

    message(STATUS "Declaring the tests for the console parser")
    file(GLOB CONSOLE_PARSER_TEST_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/ConsoleParser.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/ConsoleParserTests.cpp"
        )
    declare_test(console_parser_tests "${CONSOLE_PARSER_TEST_FILES}" console_parser)
endif()

file(GLOB LIB_SOURCE_FILES
    ConsoleParser.cpp
    ConsoleParser.h
    )

add_library(${LIB_NAME} STATIC ${LIB_SOURCE_FILES})
set_target_properties(${LIB_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_compile_options(-Wall -Wpedantic -Wempty-body -Wno-deprecated -Wdeprecated-declarations)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-return-type-c-linkage -Wno-mismatched-tags -Wno-c99-extensions -Wno-inconsistent-missing-override -Wno-overloaded-virtual)
    add_compile_options(-Wno-missing-braces -Wno-unused-private-field)
endif()
