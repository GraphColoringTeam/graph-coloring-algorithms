#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <set>   // Dùng set để lưu trữ đỉnh kề
#include <iostream>

class Graph {
private:
    int numVertices;
    std::vector<std::set<int>> adj; // Danh sách kề dùng set
    int numEdges;                    // Lưu số cạnh

public:
    // Constructor
    Graph(int V = 0);

    // Thêm/xóa cạnh
    void addEdge(int u, int v);
    void removeEdge(int u, int v);

    // Kiểm tra kề
    bool isAdjacent(int u, int v) const;

    // Thông tin đỉnh
    int getDegree(int u) const;
    const std::set<int>& getNeighbors(int u) const; // Trả về tham chiếu const
    int getNumVertices() const;
    int getNumEdges() const;

    // Lấy danh sách tất cả đỉnh
    std::vector<int> getVertices() const {
        std::vector<int> verts(numVertices);
        for (int i = 0; i < numVertices; i++)
            verts[i] = i;
        return verts;
    }

    // In đồ thị
    void printGraph() const;
};

#endif // GRAPH_H
