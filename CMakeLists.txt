cmake_minimum_required(VERSION 3.14)
project(CONSOLE_PARSER_EXAMPLE_APP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/modules)
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "MSVC_TOOLSET_VERSION = ${MSVC_TOOLSET_VERSION}")

    add_compile_options(/W3)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    # https://blogs.msdn.microsoft.com/vcblog/2018/04/09/msvc-now-correctly-reports-__cplusplus/
    add_compile_options(/Zc:__cplusplus)
    # Disable Microsoft extension: Do not allow a non-const lvalue reference to bind to a temporary
    add_compile_options(/Zc:referenceBinding)

    # Suppresses the display of the copyright banner
    add_compile_options(/nologo)
    # Suppresses the display of linker copyright banner
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /nologo")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /nologo")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /nologo")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /nologo")
    # Treat compiler warnings as errors
    #add_compile_options(/WX)
    # Treat linker warnings as errors
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /WX")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /WX")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /WX")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /WX")
    # Allow linking of big objects
    add_compile_options(/bigobj)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wempty-body -Wno-deprecated -Wdeprecated-declarations")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type-c-linkage -Wno-mismatched-tags -Wno-c99-extensions -Wno-inconsistent-missing-override -Wno-overloaded-virtual")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces -Wno-unused-private-field")
    endif()
endif()

if(WIN32)
    if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
        if(MSVC)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
        endif()

        if(MINGW)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
        endif()
    endif()
elseif(UNIX)
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic -lrt -Wl,-rpath,'$ORIGIN'" )
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -rdynamic -lrt -Wl,-rpath,'$ORIGIN'" )
endif()

# This is the only thing that you need to do 
# when including the console parser
# to your application
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/console_parser)

file(GLOB SOURCE_FILES
    main.cpp
    )

add_executable(main_app ${SOURCE_FILES})

# Do not forget to link the lib to your executable
target_link_libraries(main_app PUBLIC console_parser)
target_include_directories(main_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
