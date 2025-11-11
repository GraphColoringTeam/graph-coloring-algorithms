#include "CP_Solver.h"
#include "ortools/sat/cp_model.h"
#include <iostream>
using namespace operations_research;
using namespace sat;

std::vector<int> CPSolver::solve(const Graph& g, int maxColors) {
    CpModelBuilder model;
    int n = g.numVertices();

    std::vector<IntVar> color;
    for (int i = 0; i < n; ++i)
        color.push_back(model.NewIntVar(Domain(1, maxColors)).WithName("c" + std::to_string(i)));

    for (int u = 0; u < n; ++u)
        for (int v : g.getAdj(u))
            if (u < v)
                model.AddNotEqual(color[u], color[v]);

    Model m;
    CpSolverResponse resp = SolveCpModel(model.Build(), &m);

    std::vector<int> result(n, 0);
    if (resp.status() == CpSolverStatus::OPTIMAL || resp.status() == CpSolverStatus::FEASIBLE) {
        for (int i = 0; i < n; ++i)
            result[i] = SolutionIntegerValue(resp, color[i]);
    }
    return result;
}
