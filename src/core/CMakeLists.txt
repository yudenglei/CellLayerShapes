cmake_minimum_required(VERSION 3.15)

# Core模块 - 无GUI依赖
project(FluxCAE_Core VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 源文件列表
set(CORE_SOURCES
    # Foundation - Math
    foundation/math/Point.cpp
    foundation/math/Box.cpp
    foundation/math/Polygon.cpp
    foundation/math/Transform.cpp
    
    # Foundation - Memory
    foundation/memory/MemoryPool.cpp
    
    # Foundation - Utils
    foundation/utils/Logger.cpp
    foundation/utils/Config.cpp
    foundation/utils/IDGenerator.cpp
    
    # Domain - Layout
    domain/layout/Layout.cpp
    domain/layout/Layer.cpp
    domain/layout/Cell.cpp
    domain/layout/Shape.cpp
    domain/layout/Instance.cpp
    domain/layout/Via.cpp
    domain/layout/Drill.cpp
    domain/layout/SpatialIndex.cpp
    
    # Domain - Netlist
    domain/netlist/Netlist.cpp
    domain/netlist/Net.cpp
    domain/netlist/Component.cpp
    domain/netlist/Pin.cpp
    domain/netlist/Topology.cpp
    
    # Domain - Stackup
    domain/stackup/LayerStack.cpp
    domain/stackup/Material.cpp
    domain/stackup/StackupLayer.cpp
    
    # Service - Transaction
    service/transaction/Command.cpp
    service/transaction/CommandStack.cpp
    
    # Service - Plugin
    service/plugin/PluginManager.cpp
    
    # Service - Serializer
    service/serializer/BinarySerializer.cpp
    service/serializer/GDSReader.cpp
    service/serializer/GDSWriter.cpp
    service/serializer/OASISReader.cpp
    service/serializer/OASISWriter.cpp
)

# 头文件列表
set(CORE_HEADERS
    fluxcae.h
    
    # Foundation
    foundation/math/Point.h
    foundation/math/Box.h
    foundation/math/Polygon.h
    foundation/math/Transform.h
    foundation/memory/MemoryPool.h
    foundation/memory/ObjectArena.h
    foundation/utils/Logger.h
    foundation/utils/Config.h
    foundation/utils/IDGenerator.h
    
    # Domain
    domain/layout/Layout.h
    domain/layout/Layer.h
    domain/layout/Cell.h
    domain/layout/Shape.h
    domain/layout/Instance.h
    domain/layout/Via.h
    domain/layout/Drill.h
    domain/layout/SpatialIndex.h
    domain/netlist/Netlist.h
    domain/netlist/Net.h
    domain/netlist/Component.h
    domain/netlist/Pin.h
    domain/netlist/Topology.h
    domain/stackup/LayerStack.h
    domain/stackup/Material.h
    domain/stackup/StackupLayer.h
    
    # Service
    service/transaction/Command.h
    service/transaction/CommandStack.h
    service/plugin/IPlugin.h
    service/plugin/PluginManager.h
    service/serializer/ISerializer.h
    service/serializer/BinarySerializer.h
)

# 创建库
add_library(fluxcae_core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# 链接Qt5 Core
target_link_libraries(fluxcae_core
    PUBLIC Qt5::Core
)

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

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION include/fluxcae
    FILES_MATCHING PATTERN "*.h"
)
