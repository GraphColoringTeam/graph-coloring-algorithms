#include "Graph.h"
#include <stdexcept>
#include <algorithm> // Có thể cần cho các thao tác vector

// Constructor
Graph::Graph(int num_vertices) : V(num_vertices) {
    if (V < 0) {
        throw std::invalid_argument("So luong dinh phai khong am.");
    }
    adj.resize(V); // Khởi tạo vector kề cho V đỉnh
}

// Thêm cạnh
void Graph::addEdge(int u, int v) {
    if (u >= 0 && u < V && v >= 0 && v < V) {
        // Kiểm tra và thêm cạnh (cho đồ thị vô hướng)
        
        // Tránh cạnh lặp (tùy thuộc vào yêu cầu của bạn)
        bool u_exists = std::find(adj[u].begin(), adj[u].end(), v) != adj[u].end();
        bool v_exists = std::find(adj[v].begin(), adj[v].end(), u) != adj[v].end();

        if (!u_exists) {
            adj[u].push_back(v);
        }
        if (!v_exists) {
            adj[v].push_back(u);
        }

    } else {
        throw std::out_of_range("Dinh khong hop le trong addEdge.");
    }
}

// In đồ thị (Triển khai mẫu)
void Graph::printGraph() const {
    std::cout << "Do thi co " << V << " dinh:\n";
    for (int i = 0; i < V; ++i) {
        std::cout << "Dinh " << i << ": ";
        for (int neighbor : adj[i]) {
            std::cout << neighbor << " ";
        }
        std::cout << "\n";
    }
}

// Kiểm tra cạnh
bool Graph::isAdjacent(int u, int v) const {
    if (u >= 0 && u < V) {
        // Tìm v trong danh sách kề của u
        return std::find(adj[u].begin(), adj[u].end(), v) != adj[u].end();
    }
    return false;
}

// Lấy bậc
int Graph::getDegree(int u) const {
    if (u >= 0 && u < V) {
        return adj[u].size();
    }
    return 0;
}