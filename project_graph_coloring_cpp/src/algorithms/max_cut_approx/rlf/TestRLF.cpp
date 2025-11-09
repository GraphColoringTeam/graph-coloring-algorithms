#include "RLFColoring.h"
#include <iostream>
#include <chrono>

int main() {
    std::vector<std::pair<int, int>> edges = {
        {0,1},{0,2},{1,3},{2,3},{3,4},{4,5}
    };

    RLFColoring rlf(6, edges);

    std::vector<int> colors;
    auto start = std::chrono::high_resolution_clock::now();
    int used_colors = rlf.colorGraph(colors);
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "RLF Coloring Result:\n";
    for (int i = 0; i < colors.size(); ++i)
        std::cout << "Vertex " << i << " -> Color " << colors[i] << std::endl;
    std::cout << "Total colors used: " << used_colors << std::endl;
    std::cout << "Execution time: " << duration << " ms\n";
}
