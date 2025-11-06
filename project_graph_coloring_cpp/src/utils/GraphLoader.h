// src/utils/GraphLoader.h 

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include "graph_representation/Graph.h"
std::unique_ptr<Graph> loadGraphFromFile(const std::string& filename);
