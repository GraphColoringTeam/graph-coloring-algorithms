#ifndef GRAPH_LOADER_H
#define GRAPH_LOADER_H

#include "Graph.h"
#include <string>

using namespace std;

namespace GraphLoader {
    Graph loadGraphFromFile(const string& filename);
    void saveGraphToFile(const Graph& graph, const string& filename);
    
    // Thêm alias để tương thích với code cũ
    inline void saveGraph(const Graph& graph, const string& filename) {
        saveGraphToFile(graph, filename);
    }
}

#endif // GRAPH_LOADER_H