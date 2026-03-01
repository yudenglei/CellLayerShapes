# FluxCAE 项目结构

```
FluxCAE/
├── CMakeLists.txt                    # 顶层CMake配置
├── README.md                         # 项目说明
├── docs/
│   ├── ARCHITECTURE.md              # 架构设计文档
│   └── PROGRESS.md                  # 开发进度
│
├── include/fluxcae/                # 头文件
│   ├── fluxcae.h                   # 主头文件（包含所有模块）
│   │
│   ├── foundation/                  # 基础层
│   │   ├── math/
│   │   │   ├── Point.h             # 2D/3D点坐标
│   │   │   ├── Box.h               # 包围盒
│   │   │   ├── Polygon.h           # 多边形
│   │   │   └── Transform.h          # 变换矩阵
│   │   ├── memory/
│   │   │   ├── MemoryPool.h        # 内存池
│   │   │   └── ObjectArena.h       # 对象 Arena
│   │   └── utils/
│   │       ├── Logger.h            # 日志系统
│   │       ├── Config.h             # 配置管理
│   │       └── IDGenerator.h        # ID生成器
│   │
│   ├── domain/                     # 领域层
│   │   ├── layout/
│   │   │   ├── Layout.h            # 主布局类
│   │   │   ├── Layer.h              # 物理层
│   │   │   ├── Cell.h               # 单元
│   │   │   ├── Shape.h             # 形状基类
│   │   │   ├── Instance.h           # 实例
│   │   │   ├── Via.h               # 过孔
│   │   │   ├── Drill.h             # 钻孔
│   │   │   └── SpatialIndex.h       # 空间索引
│   │   ├── netlist/
│   │   │   ├── Netlist.h           # 网表
│   │   │   ├── Net.h               # 网络
│   │   │   ├── Component.h         # 器件
│   │   │   ├── Pin.h               # 引脚
│   │   │   └── Topology.h          # 拓扑结构
│   │   └── stackup/
│   │       ├── LayerStack.h         # 层叠结构
│   │       ├── Material.h           # 材料
│   │       └── StackupLayer.h       # 叠层单层
│   │
│   └── service/                    # 服务层
│       ├── transaction/
│       │   ├── Command.h            # 命令接口
│       │   └── CommandStack.h       # 命令栈
│       ├── plugin/
│       │   ├── IPlugin.h           # 插件接口
│       │   └── PluginManager.h     # 插件管理器
│       └── serializer/
│           ├── ISerializer.h        # 序列化接口
│           └── BinarySerializer.h   # 二进制序列化
│
├── src/
│   ├── core/                       # 核心库（无GUI依赖）
│   │   ├── CMakeLists.txt
│   │   ├── foundation/
│   │   ├── domain/
│   │   └── service/
│   │
│   ├── gui/                        # GUI应用程序
│   │   ├── CMakeLists.txt
│   │   ├── main.cpp
│   │   ├── mainwindow/
│   │   │   └── MainWindow.h
│   │   ├── viewport/
│   │   │   └── ViewportWidget.h
│   │   └── tools/
│   │       └── ITool.h
│   │
│   ├── cli/                        # CLI应用程序（Headless）
│   │   ├── CMakeLists.txt
│   │   ├── main.cpp
│   │   └── CLIOptions.h
│   │
│   ├── api/                        # API层
│   │   ├── python/
│   │   │   ├── CMakeLists.txt
│   │   │   └── pybind_main.cpp
│   │   └── grpc/
│   │       ├── CMakeLists.txt
│   │       ├── fluxcae.proto
│   │       └── FluxCAEService.cpp
│   │
│   └── plugins/                    # 插件目录
│       ├── CMakeLists.txt
│       ├── reader_gds/
│       ├── reader_oasis/
│       └── drc_basic/
│
├── tests/                          # 单元测试
│   ├── CMakeLists.txt
│   ├── test_core/
│   │   ├── test_geometry.cpp
│   │   ├── test_layout.cpp
│   │   └── test_netlist.cpp
│   └── test_transaction/
│       └── test_undo_redo.cpp
│
├── examples/                       # 示例代码
│   ├── python/
│   │   ├── hello_world.py
│   │   └── batch_process.py
│   └── cpp/
│       └── create_layout.cpp
│
└── scripts/                        # 构建脚本
    ├── build.sh
    ├── package.sh
    └── test.sh
```

## 模块说明

### Core模块（无GUI依赖）
- **foundation**: 基础工具库（数学、内存、工具）
- **domain**: 核心业务逻辑（布局、网表、层叠）
- **service**: 服务层（事务、插件、序列化）

### GUI模块
- Qt5.13 主窗口和视口渲染
- 交互工具系统

### CLI模块（Headless）
- Python脚本执行器
- gRPC服务器
- 批处理支持

### API模块
- pybind11 Python绑定
- gRPC服务定义

## 构建指南

```bash
# 创建构建目录
mkdir build && cd build

# 配置
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译
cmake --build . -j$(nproc)

# 运行GUI
./bin/fluxcae_gui

# 运行CLI
./bin/fluxcae_cli --help
```

## 关键技术特性

1. **轻量化设计**: 参考KLayout，对象使用ID而非指针，降低内存开销
2. **内存池**: 自定义MemoryPool实现高性能对象分配
3. **空间索引**: R-Tree支持快速空间查询
4. **事务系统**: Command模式实现Undo/Redo
5. **插件化**: 接口抽象+动态加载
6. **双模式**: Headless和GUI完全共享Core代码
