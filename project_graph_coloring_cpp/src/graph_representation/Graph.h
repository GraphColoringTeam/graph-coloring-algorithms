#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>

class Graph {
private:
    int V; // Số lượng đỉnh
    std::vector<std::vector<int>> adj; // Danh sách kề

public:
    // Constructor: Khởi tạo đồ thị với V đỉnh
    Graph(int num_vertices);

    // Phương thức cơ bản: Thêm cạnh
    void addEdge(int u, int v);

    // Phương thức: In đồ thị
    void printGraph() const;
    
    // Phương thức: Kiểm tra cạnh (adjaceny)
    bool isAdjacent(int u, int v) const;

    // Phương thức: Lấy bậc của đỉnh
    int getDegree(int u) const;

    // Getter cho số đỉnh
    int getV() const { return V; }

    // Bất kỳ phương thức nào khác (như tô màu) sẽ được thêm vào sau
};

#endif // GRAPH_H