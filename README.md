# FluxCAE - CAE Framework Data Object System

A modern CAE (Computer-Aided Engineering) framework for PCB layout and IC packaging design, inspired by KLayout.

## Features

- **Layer-Based Data Storage**: Efficient layer-indexed storage like KLayout
- **Cross-Layer Objects**: Via, Drill support for multi-layer designs
- **Relational Data**: Net-Trace-Component-Pin topology management
- **Multi-Board Stacking**: Support for complex 2.5D/3D packaging
- **High Performance**: Memory pool, object arena for million-level objects
- **Undo/Redo**: Transaction system with command pattern
- **Plugin Architecture**: Extensible plugin system
- **Python Support**: pybind11 bindings
- **gRPC Support**: Remote procedure calls
- **Dual Mode**: Headless (CLI) and GUI modes

## Architecture

```
FluxCAE/
├── include/fluxcae/          # Header files
│   ├── foundation/           # Foundation layer (math, memory, utils)
│   ├── domain/               # Domain layer (layout, netlist, stackup)
│   └── service/              # Service layer (transaction, plugin, serializer)
├── src/core/                 # Core implementation
├── src/gui/                  # GUI application
├── src/cli/                  # Headless CLI application
├── src/api/                  # Python/gRPC API
├── src/plugins/              # Plugin modules
└── tests/                    # Unit tests
```

## Dependencies

- Qt 5.13+
- C++17
- CMake 3.15+
- Python 3.8+ (optional)
- pybind11 (optional)
- gRPC (optional)

## Building

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Testing

```bash
cd build
ctest
```

## License

MIT License
