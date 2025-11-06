#include <iostream>
#include <memory>
#include "Graph.h" // Chỉ tên file, vì bạn đã thêm thư mục cha vào includePath
#include "utils/GraphLoader.h" 
#include "utils/GraphGenerator.h" 


void test_basic_graph() {
    std::cout << "--- BAT DAU TEST CO BAN LOP GRAPH ---\n";
    Graph g(5); 

    g.addEdge(0, 1);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(4, 3);
    g.addEdge(1, 4);

    g.printGraph(); 
    std::cout << "0 co ke 1 khong? " << (g.isAdjacent(0, 1) ? "Co" : "Khong") << std::endl; 
    std::cout << "Bac cua dinh 1: " << g.getDegree(1) << std::endl; 
    std::cout << "--- TEST CO BAN HOAN TAT ---\n";}

void test_full_modules() {
    std::cout << "\n--- TEST LOADER & GENERATOR ---\n";

    // ** TEST GENERATOR **
    std::cout << "Tao ngau nhien do thi 8 dinh, 10 canh...\n";
    auto g_rand = generateRandomGraph(8, 10);
    if (g_rand) {
        g_rand->printGraph();
    } else {
        std::cerr << "Tao ngau nhien that bai.\n";
    }

    // ** TEST LOADER **
    std::cout << "Tai do thi tu file 'input.txt'\n";
    auto g_loaded = loadGraphFromFile("input.txt"); 
    if (g_loaded) {
        g_loaded->printGraph();
    } else {
        std::cerr << "Tai file that bai.\n";
    }
    std::cout << "--- TEST TOAN DIEN HOAN TAT ---\n";
}


int main() {
    test_basic_graph();
    test_full_modules(); // Gọi hàm test mới
    return 0;
}
