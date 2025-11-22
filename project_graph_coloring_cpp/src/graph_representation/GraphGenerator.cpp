#include "GraphGenerator.h"
#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>

using namespace std;

namespace GraphGenerator {

static mt19937_64 rng(chrono::system_clock::now().time_since_epoch().count());

Graph generateRandomGraph(int numVertices, double probability) {
    if (numVertices <= 0) {
        cerr << "Lỗi: Số đỉnh phải lớn hơn 0." << endl;
        return Graph(0);
    }
    if (probability < 0.0 || probability > 1.0) {
        cerr << "Lỗi: Xác suất phải nằm trong khoảng [0.0, 1.0]." << endl;
        return Graph(numVertices);
    }

    Graph g(numVertices);
    bernoulli_distribution dist(probability);

    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            if (dist(rng)) {
                g.addEdge(i, j);
            }
        }
    }
    return g;
}

Graph generateRandomGraphByEdges(int numVertices, int numEdges) {
    if (numVertices <= 0) {
        cerr << "Lỗi: Số đỉnh phải lớn hơn 0." << endl;
        return Graph(0);
    }

    long long maxPossibleEdges = static_cast<long long>(numVertices) * (numVertices - 1) / 2;
    if (numEdges < 0 || numEdges > maxPossibleEdges) {
        cerr << "Lỗi: Số cạnh không hợp lệ. Phải trong khoảng [0, " << maxPossibleEdges << "]." << endl;
        return Graph(numVertices);
    }

    Graph g(numVertices);
    vector<pair<int, int>> allPossibleEdges;
    allPossibleEdges.reserve(maxPossibleEdges); // Optimize performance

    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            allPossibleEdges.emplace_back(i, j);
        }
    }

    shuffle(allPossibleEdges.begin(), allPossibleEdges.end(), rng);

    for (int k = 0; k < numEdges; ++k) {
        auto& edge = allPossibleEdges[k];
        g.addEdge(edge.first, edge.second);
    }

    return g;
}

} // namespace GraphGenerator