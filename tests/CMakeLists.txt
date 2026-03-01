# CMakeLists.txt for tests
cmake_minimum_required(VERSION 3.15)

# 测试
add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic
    fluxcae_core
    Qt5::Core
)
add_test(NAME BasicTests COMMAND test_basic)
