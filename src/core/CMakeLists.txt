cmake_minimum_required(VERSION 3.15)

# Core module - No GUI dependency
project(FluxCAE_Core VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
set(CORE_SOURCES
    # Foundation - Utils
    Logger.cpp
    
    # Foundation - Memory
    MemoryPool.cpp
    
    # Domain - Layout
    Layout.cpp
    Cell.cpp
    Layer.cpp
    Shape.cpp
    Via.cpp
    Drill.cpp
    SpatialIndex.cpp
    
    # Domain - Netlist
    Netlist.cpp
    Net.cpp
    Component.cpp
    Pin.cpp
    
    # Domain - Stackup
    LayerStack.cpp
    Material.cpp
    StackupLayer.cpp
    
    # Service - Transaction
    Command.cpp
    
    # Service - Plugin
    PluginManager.cpp
)

# Header files directory
set(CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

# Create library
add_library(fluxcae_core STATIC ${CORE_SOURCES})

# Include directories
target_include_directories(fluxcae_core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# Link Qt5 Core
target_link_libraries(fluxcae_core PUBLIC Qt5::Core)

# Output directory
set_target_properties(fluxcae_core PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

# Installation
install(TARGETS fluxcae_core
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
