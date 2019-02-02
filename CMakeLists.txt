cmake_minimum_required(VERSION 3.7)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/static)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include(GenerateExportHeader)

find_package(Catch2 REQUIRED)
enable_testing()

add_compile_options(-Wall
    -Werror
    -ansi
    -pedantic
    -fpic
    -fvisibility=hidden
    -fvisibility-inlines-hidden
    -std=c++14
)

include_directories(${CMAKE_SOURCE_DIR})

set(src
    property.cpp
    property.h
    basic_property.cpp
    basic_property.h
    numeric_property.cpp
    numeric_property.h
)

add_library(properties ${src})

generate_export_header(properties)
target_include_directories(properties PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(properties
    )

add_subdirectory(tests)
