/**
 * @file fluxcae.h
 * @brief FluxCAE 核心头文件
 * @author FluxCAE
 * @date 2024
 */
#pragma once

// ===== Foundation 层 =====

// Foundation - Math
#include "fluxcae/foundation/math/Point.h"
#include "fluxcae/foundation/math/Box.h"
#include "fluxcae/foundation/math/Polygon.h"
#include "fluxcae/foundation/math/Transform.h"

// Foundation - Memory
#include "fluxcae/foundation/memory/MemoryPool.h"

// Foundation - Utils
#include "fluxcae/foundation/utils/Logger.h"
#include "fluxcae/foundation/utils/Config.h"
#include "fluxcae/foundation/utils/IDGenerator.h"

// ===== Domain 层 =====

// Domain - Layout
#include "fluxcae/domain/layout/Layout.h"
#include "fluxcae/domain/layout/Layer.h"
#include "fluxcae/domain/layout/Cell.h"
#include "fluxcae/domain/layout/Shape.h"
#include "fluxcae/domain/layout/Instance.h"
#include "fluxcae/domain/layout/Via.h"
#include "fluxcae/domain/layout/Drill.h"
#include "fluxcae/domain/layout/SpatialIndex.h"
#include "fluxcae/domain/layout/Netlist.h"
#include "fluxcae/domain/layout/Net.h"
#include "fluxcae/domain/layout/Component.h"
#include "fluxcae/domain/layout/Pin.h"
#include "fluxcae/domain/layout/LayerStack.h"
#include "fluxcae/domain/layout/Material.h"
#include "fluxcae/domain/layout/StackupLayer.h"

// ===== Service 层 =====

// Service - Transaction
#include "fluxcae/service/transaction/Command.h"

// Service - Plugin
#include "fluxcae/service/plugin/IPlugin.h"
#include "fluxcae/service/PluginManager.h"

// Service - Serializer
#include "fluxcae/service/ISerializer.h"
#include "fluxcae/service/GDSReader.h"
#include "fluxcae/service/GDSWriter.h"
#include "fluxcae/service/OASISReader.h"
#include "fluxcae/service/OASISWriter.h"

// ===== 版本信息 =====

#define FLUXCAE_VERSION_MAJOR 0
#define FLUXCAE_VERSION_MINOR 1
#define FLUXCAE_VERSION_PATCH 0

#define FLUXCAE_CONCAT_(a, b) a##b
#define FLUXCAE_CONCAT(a, b) FLUXCAE_CONCAT_(a, b)
#define FLUXCAE_VERSION_STRING FLUXCAE_CONCAT(FLUXCAE_CONCAT(FLUXCAE_CONCAT(FLUXCAE_VERSION_MAJOR, .), FLUXCAE_CONCAT(FLUXCAE_VERSION_MINOR, .)), FLUXCAE_VERSION_PATCH)

// 命名空间快捷方式
namespace fluxcae {
    using namespace fluxcae::foundation;
    using namespace fluxcae::domain::layout;
    using namespace fluxcae::service;
}
