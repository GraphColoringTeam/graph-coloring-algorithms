#ifndef DSATUR_COLORING_H
#define DSATUR_COLORING_H

#include <vector>
#include <set>
#include <utility>
#include <algorithm>

class DSATURColoring {
private:
    struct VertexInfo {
        int id;
        int degree;
        int saturation_degree;

        // To use in std::set (ordered by DSAT first, then degree, then id)
        bool operator<(const VertexInfo& other) const {
            if (saturation_degree != other.saturation_degree)
                return saturation_degree < other.saturation_degree;
            if (degree != other.degree)
                return degree < other.degree;
            return id < other.id;
        }
    };

    int V;  // number of vertices
    std::vector<std::vector<int>> adj;

    int calculateSaturation(int u, const std::vector<int>& colors);

public:
    DSATURColoring(int num_vertices, const std::vector<std::pair<int, int>>& edges);
    int colorGraph(std::vector<int>& result_colors);
};

#endif
