#include "DSATURColoring.h"
#include <iostream>

// Constructor
DSATURColoring::DSATURColoring(int num_vertices, const std::vector<std::pair<int, int>>& edges)
    : V(num_vertices) {
    adj.resize(V);
    for (const auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }
}

int DSATURColoring::calculateSaturation(int u, const std::vector<int>& colors) {
    std::set<int> used_colors;
    for (int v : adj[u]) {
        if (colors[v] != 0) 
            used_colors.insert(colors[v]);
    }
    return used_colors.size();
}

int DSATURColoring::colorGraph(std::vector<int>& result_colors) {
    result_colors.assign(V, 0); 


    std::vector<VertexInfo> info(V);
    for (int i = 0; i < V; ++i)
        info[i] = {i, (int)adj[i].size(), 0};

    std::set<VertexInfo> uncolored_vertices;
    for (int i = 0; i < V; ++i)
        uncolored_vertices.insert(info[i]);

    int max_color = 0;

    for (int count = 0; count < V; ++count) {
        if (uncolored_vertices.empty()) break;

        
        auto it = uncolored_vertices.rbegin(); 
        VertexInfo u_info = *it;
        uncolored_vertices.erase(std::next(it).base());

        int u = u_info.id;

        
        std::vector<bool> available(V + 1, true);
        for (int v : adj[u])
            if (result_colors[v] != 0)
                available[result_colors[v]] = false;

        int assigned_color = 0;
        for (int c = 1; c <= V; ++c) {
            if (available[c]) {
                assigned_color = c;
                break;
            }
        }

        result_colors[u] = assigned_color;
        max_color = std::max(max_color, assigned_color);

        
        for (int v : adj[u]) {
            if (result_colors[v] == 0) {
                VertexInfo old_v_info = info[v];
                uncolored_vertices.erase(old_v_info);
                info[v].saturation_degree = calculateSaturation(v, result_colors);
                uncolored_vertices.insert(info[v]);
            }
        }
    }

    return max_color;
}
