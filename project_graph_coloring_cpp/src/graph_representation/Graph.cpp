#include "Graph.h"
#include <algorithm> // Cho std::sort (nếu cần, nhưng với set thì không cần)

Graph::Graph(int V) : numVertices(V), adj(V), numEdges(0) {}

void Graph::addEdge(int u, int v) {
    if (u >= 0 && u < numVertices && v >= 0 && v < numVertices && u != v) {
        if (adj[u].find(v) == adj[u].end()) { // Chỉ thêm nếu cạnh chưa tồn tại
            adj[u].insert(v);
            adj[v].insert(u); // Đồ thị vô hướng
            numEdges++;
        }
    } else {
        std::cerr << "Lỗi: Cạnh không hợp lệ (" << u << ", " << v << ") hoặc đỉnh nằm ngoài giới hạn." << std::endl;
    }
}

void Graph::removeEdge(int u, int v) {
    if (u >= 0 && u < numVertices && v >= 0 && v < numVertices && u != v) {
        if (adj[u].erase(v)) { // erase trả về 1 nếu phần tử được xóa, 0 nếu không tìm thấy
            adj[v].erase(u);
            numEdges--;
        } else {
            // std::cerr << "Cảnh báo: Cạnh (" << u << ", " << v << ") không tồn tại để xóa." << std::endl;
        }
    } else {
        std::cerr << "Lỗi: Cạnh không hợp lệ (" << u << ", " << v << ") hoặc đỉnh nằm ngoài giới hạn để xóa." << std::endl;
    }
}


bool Graph::isAdjacent(int u, int v) const {
    if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
        return adj[u].count(v); // count(v) trả về 1 nếu v có trong set, 0 nếu không
    }
    return false;
}

int Graph::getDegree(int u) const {
    if (u >= 0 && u < numVertices) {
        return adj[u].size();
    }
    return -1; // Hoặc ném ngoại lệ
}

const std::set<int>& Graph::getNeighbors(int u) const {
    if (u >= 0 && u < numVertices) {
        return adj[u];
    }
    // Trả về một set rỗng hoặc ném ngoại lệ nếu u không hợp lệ
    static const std::set<int> emptySet;
    return emptySet;
}

int Graph::getNumVertices() const {
    return numVertices;
}

int Graph::getNumEdges() const {
    return numEdges;
}

void Graph::printGraph() const {
    std::cout << "Đồ thị " << numVertices << " đỉnh:" << std::endl;
    for (int i = 0; i < numVertices; ++i) {
        std::cout << i << ":";
        for (int neighbor : adj[i]) {
            std::cout << " " << neighbor;
        }
        std::cout << std::endl;
    }
}