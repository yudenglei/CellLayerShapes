# FluxCAE 核心头文件

#ifndef FLUXCAE_H
#define FLUXCAE_H

// Foundation - Math
#include "foundation/math/Point.h"
#include "foundation/math/Box.h"
#include "foundation/math/Polygon.h"
#include "foundation/math/Transform.h"

// Foundation - Memory
#include "foundation/memory/MemoryPool.h"
#include "foundation/memory/ObjectArena.h"

// Foundation - Utils
#include "foundation/utils/Logger.h"
#include "foundation/utils/Config.h"
#include "foundation/utils/IDGenerator.h"

// Domain - Layout
#include "domain/layout/Layout.h"
#include "domain/layout/Layer.h"
#include "domain/layout/Cell.h"
#include "domain/layout/Shape.h"
#include "domain/layout/Instance.h"
#include "domain/layout/Via.h"
#include "domain/layout/Drill.h"
#include "domain/layout/SpatialIndex.h"

// Domain - Netlist
#include "domain/netlist/Netlist.h"
#include "domain/netlist/Net.h"
#include "domain/netlist/Component.h"
#include "domain/netlist/Pin.h"
#include "domain/netlist/Topology.h"

// Domain - Stackup
#include "domain/stackup/LayerStack.h"
#include "domain/stackup/Material.h"
#include "domain/stackup/StackupLayer.h"

// Service - Transaction
#include "service/transaction/Command.h"
#include "service/transaction/CommandStack.h"

// Service - Plugin
#include "service/plugin/IPlugin.h"
#include "service/plugin/PluginManager.h"

// Service - Serializer
#include "service/serializer/ISerializer.h"
#include "service/serializer/BinarySerializer.h"

// 版本信息
#define FLUXCAE_VERSION_MAJOR 0
#define FLUXCAE_VERSION_MINOR 1
#define FLUXCAE_VERSION_PATCH 0

#define FLUXCAE_VERSION_STRING \
    FLUXCAE_MAKE_VERSION_STRING(FLUXCAE_VERSION_MAJOR, FLUXCAE_VERSION_MINOR, FLUXCAE_VERSION_PATCH)

#define FLUXCAE_MAKE_VERSION_STRING(major, minor, patch) \
    #major "." #minor "." #patch

#endif // FLUXCAE_H
