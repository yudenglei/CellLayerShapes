cmake_minimum_required(VERSION 3.15)

# Tests
project(FluxCAE_Tests VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Test source
set(TEST_SOURCES
    test_basic.cpp
)

# Create test executable
add_executable(test_basic ${TEST_SOURCES})

# Include directories
target_include_directories(test_basic PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(test_basic
    fluxcae_core
    Qt5::Core
)

# Add test
add_test(NAME BasicTests COMMAND test_basic)
