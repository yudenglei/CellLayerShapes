cmake_minimum_required(VERSION 3.15)

# Core模块 - 无GUI依赖
project(FluxCAE_Core VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 源文件列表 - 只包含实际存在的文件
set(CORE_SOURCES
    # Foundation - Utils
    Logger.cpp
    
    # Foundation - Memory
    MemoryPool.cpp
    
    # Domain - Layout
    Layout.cpp
    Cell.cpp
    
    # Service - Transaction
    Command.cpp
)

# 头文件目录
set(CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

# 创建库
add_library(fluxcae_core STATIC ${CORE_SOURCES})

# 设置包含目录
target_include_directories(fluxcae_core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# 链接Qt5 Core
target_link_libraries(fluxcae_core PUBLIC Qt5::Core)

# 设置输出目录
set_target_properties(fluxcae_core PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

# 安装
install(TARGETS fluxcae_core
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
