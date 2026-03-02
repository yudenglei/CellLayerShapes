cmake_minimum_required(VERSION 3.15)

# FluxCAE Project
project(FluxCAE VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-Wall -Wextra -g -O0)
else()
    add_compile_options(-Wall -Wextra -O3)
endif()

# Find Qt5 (only Core and Widgets)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# Optional features - disabled by default
option(BUILD_GUI "Build GUI application" OFF)
option(BUILD_CLI "Build CLI application (Headless)" ON)
option(BUILD_TESTS "Build tests" ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Subdirectories
add_subdirectory(src/core)

if(BUILD_CLI)
    add_subdirectory(src/cli)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
install(DIRECTORY include/fluxcae DESTINATION include)
install(DIRECTORY docs/ DESTINATION share/doc/fluxcae)
