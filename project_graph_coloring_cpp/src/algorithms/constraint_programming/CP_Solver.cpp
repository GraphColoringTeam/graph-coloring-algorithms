#include "CP_Solver.h"
#include <iostream>

// Nếu bạn đã cài OR-Tools, bỏ comment phần dưới và include thư viện:
#include "ortools/sat/cp_model.h"

using namespace operations_research;
using namespace sat;

std::vector<int> CPSolver::solve(const Graph& graph, int maxColors) {
    CpModelBuilder model;

    std::vector<IntVar> colorVars;
    for (int i = 0; i < graph.numVertices(); ++i) {
        colorVars.push_back(model.NewIntVar(Domain(1, maxColors)).WithName("color_" + std::to_string(i)));
    }

    // Ràng buộc: các đỉnh kề nhau không được cùng màu
    for (int u = 0; u < graph.numVertices(); ++u) {
        for (int v : graph.getAdj(u)) {
            if (u < v) model.AddNotEqual(colorVars[u], colorVars[v]);
        }
    }

    Model cpModel;
    const CpSolverResponse response = SolveCpModel(model.Build(), &cpModel);

    std::vector<int> colors(graph.numVertices(), 0);
    if (response.status() == CpSolverStatus::OPTIMAL || response.status() == CpSolverStatus::FEASIBLE) {
        for (int i = 0; i < graph.numVertices(); ++i)
            colors[i] = SolutionIntegerValue(response, colorVars[i]);
    } else {
        std::cerr << "Không tìm được nghiệm khả thi.\n";
    }

    return colors;
}
