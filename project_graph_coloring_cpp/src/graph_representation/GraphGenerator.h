#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "Graph.h"
#include <string>

namespace GraphGenerator {
    Graph generateRandomGraph(int numVertices, double probability);
    Graph generateRandomGraphByEdges(int numVertices, int numEdges);
}

#endif // GRAPH_GENERATOR_H