#include <memory>
#include <vector>

#include "graph_representation/Graph.h"


// Khai báo hàm generator
std::unique_ptr<Graph> generateRandomGraph(int V, int E);