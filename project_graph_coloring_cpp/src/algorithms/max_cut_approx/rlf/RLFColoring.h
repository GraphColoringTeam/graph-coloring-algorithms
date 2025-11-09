// RLF.h
#include <vector>
#include <list>

class RLFColoring {
private:
    int num_vertices;
    std::vector<std::list<int>> adj_list; // Danh sách kề

public:
    RLFColoring(int V, const std::vector<std::pair<int, int>>& edges);
    int colorGraph(std::vector<int>& result_colors);
};
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <map>

class RLFColoring {
private:
    int V; 
    std::vector<std::list<int>> adj; 

    
    int findMaxDegreeVertex(const std::vector<bool>& U, const std::map<int, int>& current_degree);

    
    int selectNextVertexForIS(const std::vector<bool>& U, const std::vector<bool>& IS, 
                              const std::map<int, int>& is_neighbors_count);

public:
    RLFColoring(int num_vertices, const std::vector<std::pair<int, int>>& edges);
    int colorGraph(std::vector<int>& result_colors);
};