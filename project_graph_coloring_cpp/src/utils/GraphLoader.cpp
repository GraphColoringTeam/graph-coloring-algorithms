// src/utils/GraphLoader.cpp 

#include "GraphLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::unique_ptr<Graph> loadGraphFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Loi: Khong the mo file " << filename << ". Kiem tra duong dan file." << std::endl;
        return nullptr;
    }

    int V = 0, E = 0; 
    std::string line;

    if (std::getline(file, line)) {
        std::stringstream ss(line);
        if (!(ss >> V >> E)) V = 0; 
    }

    if (V <= 0) {
        std::cerr << "Loi: So luong dinh khong hop le hoac khong doc duoc." << std::endl;
        return nullptr;
    }

    auto g = std::make_unique<Graph>(V);

    int u, v;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        if (ss >> u >> v) {
            try {
                g->addEdge(u, v);
            } catch (const std::out_of_range& e) {
                std::cerr << "Canh (" << u << ", " << v << ") bi bo qua: " << e.what() << std::endl;
            }
        }
    }

    file.close();
    return g;
}