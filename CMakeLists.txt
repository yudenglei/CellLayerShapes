cmake_minimum_required(VERSION 3.15)
project(FluxCAE VERSION 0.1.0 LANGUAGES CXX)

# C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-Wall -Wextra -g -O0)
else()
    add_compile_options(-Wall -Wextra -O3)
endif()

# 寻找Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# 寻找Python3
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# 寻找pybind11
find_package(pybind11 REQUIRED)

# 寻找线程
find_package(Threads REQUIRED)

# 可选依赖
find_package(Protobuf QUIET)
find_package(gRPC QUIET)
find_package(Boost QUIET COMPONENTS geometry)

# 选项
option(BUILD_GUI "Build GUI application" ON)
option(BUILD_CLI "Build CLI application (Headless)" ON)
option(BUILD_PYTHON "Build Python bindings" ON)
option(BUILD_GRPC "Build gRPC service" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_PLUGINS "Build plugins" ON)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# 子目录
add_subdirectory(src/core)
add_subdirectory(src/gui)
add_subdirectory(src/cli)
add_subdirectory(src/api)
add_subdirectory(src/plugins)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# 安装目标
install(DIRECTORY include/fluxcae DESTINATION include)
install(DIRECTORY docs/ DESTINATION share/doc/fluxcae)
