/**
 * @file test_basic.cpp
 * @brief Basic unit tests for FluxCAE
 * @author FluxCAE
 * @date 2024
 */
#include <iostream>
#include <cassert>
#include <memory>
#include "fluxcae/fluxcae.h"

using namespace fluxcae;

// Test Point class
void testPoint() {
    std::cout << "Testing Point..." << std::endl;
    
    math::Point p1(10, 20);
    assert(p1.x == 10);
    assert(p1.y == 20);
    
    math::Point p2(30, 40);
    math::Point p3 = p1 + p2;
    assert(p3.x == 40);
    assert(p3.y == 60);
    
    auto dist = p1.distance(p2);
    assert(dist > 0);
    
    std::cout << "  Point tests passed!" << std::endl;
}

// Test Box class
void testBox() {
    std::cout << "Testing Box..." << std::endl;
    
    math::Box box(10, 20, 100, 200);
    assert(box.x1 == 10);
    assert(box.y1 == 20);
    assert(box.x2 == 100);
    assert(box.y2 == 200);
    
    math::Box box2(50, 60, 150, 250);
    math::Box merged = math::Box::merge(box, box2);
    assert(merged.x1 == 10);
    assert(merged.y1 == 20);
    assert(merged.x2 == 150);
    assert(merged.y2 == 250);
    
    assert(box.contains(math::Point(50, 100)));
    assert(!box.contains(math::Point(0, 0)));
    
    std::cout << "  Box tests passed!" << std::endl;
}

// Test Layout class
void testLayout() {
    std::cout << "Testing Layout..." << std::endl;
    
    auto layout = std::make_shared<domain::layout::Layout>();
    
    auto layerId1 = layout->createLayer(1, "TopSignal");
    auto layer1 = layout->findLayer(1);
    assert(layer1 != nullptr);
    assert(layer1->layerNumber() == 1);
    
    auto cellId = layout->createCell("Main");
    auto cell = layout->findCell("Main");
    assert(cell != nullptr);
    assert(cell->name() == "Main");
    
    layout->setTopCell(cellId);
    assert(layout->topCell() != nullptr);
    assert(layout->topCell()->name() == "Main");
    
    std::cout << "  Layout tests passed!" << std::endl;
}

// Test Cell class
void testCell() {
    std::cout << "Testing Cell..." << std::endl;
    
    auto cell = std::make_shared<domain::layout::Cell>("TestCell", 1);
    assert(cell->name() == "TestCell");
    assert(cell->id() == 1);
    assert(cell->isEmpty());
    
    auto boxShape = std::make_shared<domain::layout::BoxShape>(math::Box(0, 0, 100, 100));
    cell->addShape(boxShape);
    assert(cell->shapeCount() == 1);
    assert(!cell->isEmpty());
    
    std::cout << "  Cell tests passed!" << std::endl;
}

// Test Command/Transaction
void testTransaction() {
    std::cout << "Testing Transaction..." << std::endl;
    
    service::transaction::CommandStack stack;
    assert(!stack.canUndo());
    assert(!stack.canRedo());
    
    class TestCommand : public service::transaction::Command {
    public:
        bool executed = false;
        bool undone = false;
        
    protected:
        void doExecute() override { executed = true; }
        void doUndo() override { undone = true; }
    };
    
    TestCommand* cmd = new TestCommand();
    cmd->setLabel("Test");
    
    stack.execute(cmd);
    assert(stack.canUndo());
    assert(cmd->executed);
    
    stack.undo();
    assert(stack.canRedo());
    assert(cmd->undone);
    
    stack.redo();
    assert(cmd->executed);
    
    std::cout << "  Transaction tests passed!" << std::endl;
}

// Test MemoryPool
void testMemoryPool() {
    std::cout << "Testing MemoryPool..." << std::endl;
    
    foundation::MemoryPool pool(64, 1024);
    
    void* p1 = pool.allocate();
    assert(p1 != nullptr);
    
    void* p2 = pool.allocate();
    assert(p2 != nullptr);
    
    char* c1 = static_cast<char*>(p1);
    char* c2 = static_cast<char*>(p2);
    assert(c2 > c1);
    
    std::cout << "  MemoryPool tests passed!" << std::endl;
}

// Test LayerStack
void testLayerStack() {
    std::cout << "Testing LayerStack..." << std::endl;
    
    auto stack = domain::layout::LayerStack::create2LayerBoard();
    assert(stack != nullptr);
    assert(stack->layerCount() == 3);
    
    auto layer1 = stack->getLayer(0);
    assert(layer1 != nullptr);
    assert(layer1->isOuter());
    
    std::cout << "  LayerStack tests passed!" << std::endl;
}

// Main
int main() {
    std::cout << "=== FluxCAE Unit Tests ===" << std::endl;
    
    try {
        testPoint();
        testBox();
        testLayout();
        testCell();
        testTransaction();
        testMemoryPool();
        testLayerStack();
        
        std::cout << "\n=== All tests passed! ===" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
