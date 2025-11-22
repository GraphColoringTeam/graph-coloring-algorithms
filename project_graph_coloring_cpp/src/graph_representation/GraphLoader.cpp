#include "GraphLoader.h"
#include <fstream>
#include <iostream>

using namespace std;

namespace GraphLoader {

Graph loadGraphFromFile(const string& filename) {
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Lỗi: Không thể mở file '" << filename << "'." << endl;
        return Graph(0);
    }

    int numVertices, numEdges;
    inputFile >> numVertices >> numEdges;

    if (inputFile.fail()) {
        cerr << "Lỗi: Không thể đọc số đỉnh và số cạnh." << endl;
        inputFile.close();
        return Graph(0);
    }

    Graph g(numVertices);
    int u, v;
    for (int i = 0; i < numEdges; ++i) {
        if (!(inputFile >> u >> v)) {
            cerr << "Lỗi: Không đủ cạnh. Đọc được " << i << "/" << numEdges << endl;
            break;
        }
        g.addEdge(u, v);
    }

    inputFile.close();
    return g;
}

void saveGraphToFile(const Graph& graph, const string& filename) {
    ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        cerr << "Lỗi: Không thể ghi file '" << filename << "'." << endl;
        return;
    }

    int numVertices = graph.getNumVertices();
    int numEdges = graph.getNumEdges();

    outputFile << numVertices << " " << numEdges << "\n";

    for (int u = 0; u < numVertices; ++u) {
        for (int v : graph.getNeighbors(u)) {
            if (u < v) {
                outputFile << u << " " << v << "\n";
            }
        }
    }

    outputFile.close();
    cout << "Đã lưu đồ thị vào: " << filename 
         << " (V=" << numVertices << ", E=" << numEdges << ")" << endl;
}

} // namespace GraphLoader