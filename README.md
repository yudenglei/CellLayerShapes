# FluxCAE - 高速PCB与先进封装3D Layout CAE框架

## 项目概述

FluxCAE是一个现代化的高性能CAE框架，专为高速PCB和先进封装（2.5D/3D封装、多板堆叠）的3D Layout设计而开发。参考KLayout的轻量化设计理念，支持百万级器件对象，具备高效低内存占用的特性。

## 核心特性

- **高性能轻量化**: 参考KLayout的数据对象设计，支持百万级器件对象
- **跨平台**: Windows/Linux/Mac三大平台原生支持
- **插件化架构**: 现代化插件系统，支持运行时热加载
- **事务系统**: 完整的Undo/Redo支持
- **双模式**: Headless（无界面）和GUI（图形界面）双模式运行
- **Python支持**: 通过pybind11实现Python脚本自动化
- **gRPC支持**: 远程过程调用支持
- **现代化C++**: C++14/17标准，Qt 5.13

## 技术架构

```
L4: Application Layer
├── GUI Application (Qt5.13)
└── CLI Application (Headless)

L3: Interface Layer
├── Python Binding (pybind11)
└── gRPC Service

L2: Service Layer
├── Plugin Manager
└── Transaction Manager (Undo/Redo)

L1: Domain Layer
├── Layout Engine (分层存储)
├── Netlist Engine (关系图)
└── Stackup Manager (层叠管理)

L0: Foundation Layer
├── Geometry Library
├── Memory Pool
└── Utilities (Log, Config, Thread)
```

## 技术要求

- QT5.13, C++14以上
- CMake 3.15+
- pybind11
- gRPC + Protobuf
- 跨平台（Windows/Linux/Mac）

## 项目结构

```
FluxCAE/
├── src/
│   ├── core/              # 核心库（无GUI依赖）
│   │   ├── foundation/    # 基础库
│   │   ├── domain/        # 领域模型
│   │   └── service/       # 服务层
│   ├── gui/               # Qt界面
│   ├── cli/               # Headless命令行
│   ├── api/               # Python和gRPC API
│   └── plugins/           # 插件系统
├── tests/                 # 单元测试
├── examples/              # 示例代码
├── docs/                  # 文档
└── cmake/                 # CMake模块
```

## 构建步骤

```bash
# 1. 创建构建目录
mkdir build && cd build

# 2. 配置CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# 3. 编译
cmake --build . -j$(nproc)

# 4. 运行GUI版本
./bin/fluxcae_gui

# 5. 运行Headless版本
./bin/fluxcae_cli --help
```

## 许可证

MIT License
