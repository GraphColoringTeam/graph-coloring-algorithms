// src/utils/GraphLoader.h 

#ifndef GRAPHLOADER_H
#define GRAPHLOADER_H

#include "../graph_representation/Graph.h" 
#include <string>
#include <memory>
#include <iostream>

std::unique_ptr<Graph> loadGraphFromFile(const std::string& filename);

#endif // GRAPHLOADER_H