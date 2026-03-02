/**
 * @file main.cpp
 * @brief FluxCAE CLI Application (Headless Mode)
 * @author FluxCAE
 * @date 2024
 */
#include <iostream>
#include <string>
#include <memory>
#include <QCoreApplication>
#include <QCommandLineParser>

#include "fluxcae/fluxcae.h"

using namespace fluxcae;

void printBanner() {
    std::cout << "FluxCAE CLI v0.1.0" << std::endl;
    std::cout << "CAE Framework Data Object System" << std::endl;
    std::cout << std::endl;
}

void printHelp() {
    std::cout << "Usage: fluxcae_cli [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help           Show this help message" << std::endl;
    std::cout << "  -v, --version        Show version" << std::endl;
    std::cout << "  -i, --input <file>  Input file" << std::endl;
    std::cout << "  -o, --output <file> Output file" << std::endl;
    std::cout << "  --info               Show layout info" << std::endl;
}

void showLayoutInfo(domain::layout::Layout& layout) {
    std::cout << "Layout Information:" << std::endl;
    std::cout << "  Cells: " << layout.cellCount() << std::endl;
    std::cout << "  Layers: " << layout.layerCount() << std::endl;
    std::cout << "  Database Unit: " << layout.databaseUnit() << std::endl;
    std::cout << "  User Unit: " << layout.userUnit() << std::endl;
    
    if (layout.topCell()) {
        std::cout << "  Top Cell: " << layout.topCell()->name() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("fluxcae_cli");
    QCoreApplication::setApplicationVersion("0.1.0");
    
    printBanner();
    
    QCommandLineParser parser;
    parser.setApplicationDescription("FluxCAE CLI - Headless CAE Framework");
    
    QCommandLineOption helpOption = parser.addHelpOption();
    QCommandLineOption versionOption = parser.addVersionOption();
    QCommandLineOption inputOption("i", "input", "Input file", "file");
    QCommandLineOption outputOption("o", "output", "Output file", "file");
    QCommandLineOption infoOption("info", "Show layout information");
    
    parser.addOption(inputOption);
    parser.addOption(outputOption);
    parser.addOption(infoOption);
    
    if (!parser.parse(app.arguments())) {
        std::cerr << "Error parsing arguments" << std::endl;
        return 1;
    }
    
    if (parser.isSet(versionOption)) {
        return 0;
    }
    
    if (parser.isSet(helpOption) || argc == 1) {
        printHelp();
        return 0;
    }
    
    // Create a layout for testing
    auto layout = std::make_shared<domain::layout::Layout>();
    
    // Create a test cell
    auto cellId = layout->createCell("TestCell");
    auto cell = layout->findCell(cellId);
    
    // Create a test layer
    auto layerId = layout->createLayer(1, "TopSignal");
    auto layer = layout->findLayer(layerId);
    
    // Create a test shape (box)
    auto boxShape = std::make_shared<domain::layout::BoxShape>(
        math::Box(0, 0, 1000000, 1000000) // 1mm x 1mm
    );
    cell->addShape(boxShape);
    
    // Set top cell
    layout->setTopCell(cellId);
    
    if (parser.isSet(infoOption)) {
        showLayoutInfo(*layout);
    }
    
    std::cout << "FluxCAE CLI completed successfully." << std::endl;
    
    return 0;
}
