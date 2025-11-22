#ifndef RLF_SOLVER_H
#define RLF_SOLVER_H

#include "../../graph_representation/Graph.h"
#include <vector>
#include <set>

using namespace std;

namespace Algorithms {
namespace RLF {

class RLFSolver {
private:
    int findHighestDegreeVertex(const Graph& graph, const set<int>& U) const;
    int findHighestDegreeToK(const Graph& graph, const set<int>& U, const set<int>& K) const;

public:
    RLFSolver();
    int solve(const Graph& graph, vector<int>& colors);
    void printColoring(const vector<int>& colors) const;
};

} // namespace RLF
} // namespace Algorithms

#endif // RLF_SOLVER_H