cmake_minimum_required(VERSION 3.15)

# CLI Application
project(FluxCAE_CLI VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(CLI_SOURCES
    main.cpp
)

# Create executable
add_executable(fluxcae_cli ${CLI_SOURCES})

# Include directories
target_include_directories(fluxcae_cli PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(fluxcae_cli
    fluxcae_core
    Qt5::Core
)

# Installation
install(TARGETS fluxcae_cli
    RUNTIME DESTINATION bin
)
