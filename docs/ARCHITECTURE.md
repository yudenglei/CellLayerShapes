# FluxCAE 架构设计文档

## 1. 系统架构概述

FluxCAE采用**分层架构** + **微内核插件**模式，严格遵循**MVC变体**，确保数据层不依赖任何GUI库，实现逻辑与视图的完全解耦。

### 1.1 层次结构

| 层次 | 模块名称 | 职责 |
|------|----------|------|
| **L4** | Application | GUI/MainWindow 或 CLI/Headless |
| **L3** | Interface | Python绑定(pybind11) + gRPC服务 |
| **L2** | Service | 插件管理器 + 事务系统 |
| **L1** | Domain | Layout引擎 + Netlist引擎 + Stackup管理器 |
| **L0** | Foundation | 几何库 + 内存池 + 工具库 |

---

## 2. 核心技术设计

### 2.1 内存与性能优化（参考KLayout）

#### 2.1.1 享元模式 (Flyweight Pattern)

对于百万级重复器件（如Pad, Via），采用享元模式：

- **ComponentDefinition**: 原型定义（共享数据）
- **ComponentInstance**: 实例引用（位置、旋转、参数覆盖）

```cpp
// 享元模式实现
class ComponentDefinition {
public:
    ComponentID id;
    std::string part_number;
    std::vector<PadstackDef> pads;
    Box bounding_box;
};

class ComponentInstance {
public:
    InstanceID id;
    ComponentID definition_id;
    Transform2D transform;
    std::string reference;
};
```

#### 2.1.2 自定义内存池 (Memory Arena)

放弃std::shared_ptr用于核心微小对象，使用分块内存池：

- O(1) 分配/释放
- 内存连续，对CPU缓存友好
- 无控制块开销

### 2.2 逻辑与UI解耦（Headless支持）

Core模块不链接QtGUI，仅链接QtCore：

```cmake
target_link_libraries(fluxcae_core
    PUBLIC Qt5::Core
)
```

渲染接口抽象：
- GUI模式：OpenGLRenderer
- Headless模式：NullRenderer / ImageExportRenderer

---

## 3. 核心数据模型

### 3.1 分层存储架构

```cpp
class Layout {
    // Layer映射表
    std::map<LayerID, Layer> layers_;
    
    // 空间索引（每层一个）
    std::map<LayerID, SpatialIndex> spatial_indexes_;
    
    // Cell树
    Cell* root_cell_;
    
    // 跨层对象注册表
    ViaRegistry via_registry_;
    DrillRegistry drill_registry_;
    ComponentRegistry component_registry_;
    
    // 关系图
    Netlist netlist_;
    
    // 层叠结构
    std::unique_ptr<LayerStack> layer_stack_;
};
```

### 3.2 双索引访问模式

- **按层遍历**：适合渲染、DRC
- **按Net遍历**：适合SI仿真
- **混合遍历**：适合3D渲染

---

## 4. 事务系统 (Undo/Redo)

### 4.1 设计原理

采用**逆向增量存储**而非全量快照：

- 每个操作封装为Command对象
- Command实现execute()和undo()方法
- CommandStack维护操作历史
- 支持事务组（TransactionGroup）打包批量操作

### 4.2 核心实现

```cpp
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string description() const = 0;
};

class CommandStack {
    std::vector<std::unique_ptr<ICommand>> undo_stack_;
    std::vector<std::unique_ptr<ICommand>> redo_stack_;
public:
    void execute(std::unique_ptr<ICommand> cmd);
    void undo();
    void redo();
};

class Transaction {
    std::vector<std::unique_ptr<ICommand>> commands_;
    CommandStack& stack_;
public:
    void addCommand(std::unique_ptr<ICommand> cmd);
    void commit();
    void rollback();
};
```

---

## 5. 插件系统

### 5.1 架构设计

采用**接口抽象 + 动态加载**模式：

```cpp
class IPlugin {
    virtual std::string name() const = 0;
    virtual std::string version() const = 0;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
};

class IToolPlugin : public IPlugin { ... };
class IImporterPlugin : public IPlugin { ... };
class IExporterPlugin : public IPlugin { ... };
class IDRCPlugin : public IPlugin { ... };

class PluginManager {
    std::vector<std::unique_ptr<IPlugin>> plugins_;
public:
    void loadPlugin(const std::string& path);
    void unloadPlugin(const std::string& name);
    void loadDynamicLibrary(const std::string& path);
};
```

### 5.2 插件生命周期

Init -> Register -> Run -> Shutdown

---

## 6. Python绑定 (pybind11)

```cpp
PYBIND11_MODULE(fluxcae, m) {
    py::class_<Layout>(m, "Layout")
        .def(py::init<>())
        .def("create_cell", &Layout::createCell)
        .def("add_shape", &Layout::addShape);
    
    py::class_<Layer>(m, "Layer")
        .def("add_shape", &Layer::addShape);
}
```

---

## 7. gRPC服务

```protobuf
service FluxCAEService {
    rpc CreateLayout(CreateLayoutRequest) returns (CreateLayoutResponse);
    rpc OpenLayout(OpenLayoutRequest) returns (OpenLayoutResponse);
    rpc AddShape(AddShapeRequest) returns (AddShapeResponse);
    rpc QueryShapes(QueryShapesRequest) returns (QueryShapesResponse);
}
```

---

## 8. Headless模式

```cpp
int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);  // 非QApplication
    
    auto layout = std::make_unique<Layout>();
    
    if (!script_file.empty()) {
        PythonRunner runner;
        runner.runScript(script_file, *layout);
    }
    
    if (!output_file.empty()) {
        layout->save(output_file);
    }
    
    return 0;
}
```

使用场景：
```bash
./fluxcae_cli -i input.gds -o output.oas
./fluxcae_cli -i input.db -s process.py -o output.db
./fluxcae_cli --grpc 50051
```

---

## 9. 关键技术选型

| 类别 | 选型 |
|------|------|
| 语言标准 | C++17/C++14 |
| GUI框架 | Qt 5.13 LTS |
| 构建系统 | CMake 3.15+ |
| 脚本绑定 | pybind11 |
| RPC框架 | gRPC + Protobuf |
| 日志 | spdlog |
| 测试 | Google Test |

---

## 10. 验收标准

### 性能指标

- 加载速度: 100MB文件 < 2秒
- 内存占用: 100万Line对象 < 200MB
- 渲染帧率: 10万对象 > 30 FPS
- Undo/Redo: 1000次操作无泄漏

### 功能验证

- [ ] Headless模式正常运行
- [ ] Python脚本执行成功
- [ ] 插件热加载正常
- [ ] Undo/Redo正确工作
- [ ] gRPC服务正常响应
- [ ] 跨平台编译通过
