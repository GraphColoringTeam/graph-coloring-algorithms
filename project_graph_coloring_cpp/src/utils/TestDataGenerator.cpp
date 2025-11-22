// src/utils/TestDataGenerator.cpp
#include "TestDataGenerator.h"
#include "GraphGenerator.h"
#include "GraphLoader.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <tuple>
#include <sys/stat.h> 
#include <cstdlib> 

namespace fs = std::filesystem;

void TestDataGenerator::generateAllTestData() {
   fs::create_directories("data/small/correctness");
   fs::create_directories("data/medium/performance");
   fs::create_directories("data/large/scalability");
  
   generateSmallGraphs();
   generateMediumGraphs();
   generateLargeGraphs();
}


void TestDataGenerator::generateSmallGraphs() {
   std::cout << "Generating small correctness test graphs...\n";
  
   // Đồ thị nhỏ để test tính đúng
   std::vector<std::tuple<int, double, std::string>> graphs = {
       {6, 0.4, "simple_6"},      // Đơn giản
       {8, 0.3, "sparse_8"},      // Thưa
       {10, 0.6, "dense_10"},     // Dày
       {5, 1.0, "complete_5"},    // Đồ thị đầy đủ K5
   };
  
   for (const auto& [vertices, prob, name] : graphs) {
       Graph g = GraphGenerator::generateRandomGraph(vertices, prob);
       std::string filename = "data/small/correctness/" + name + ".txt";
       GraphLoader::saveGraph(g, filename);
       std::cout << "Generated: " << filename << "\n";
   }
}

void TestDataGenerator::generateMediumGraphs() {
   std::cout << "Generating medium performance test graphs...\n";
  
   // Đồ thị trung bình cho benchmark
   std::vector<std::tuple<int, double, std::string>> graphs = {
       {100, 0.1, "sparse_100_010"},
       {100, 0.3, "medium_100_030"},
       {100, 0.6, "dense_100_060"},
       {200, 0.08, "sparse_200_008"},
       {200, 0.25, "medium_200_025"},
       {200, 0.4, "dense_200_040"},
       {300, 0.05, "sparse_300_005"},
       {300, 0.2, "medium_300_020"}
   };

   for (const auto& [vertices, prob, name] : graphs) {
       Graph g = GraphGenerator::generateRandomGraph(vertices, prob);
       std::string filename = "data/medium/performance/" + name + ".txt";
       GraphLoader::saveGraph(g, filename);
       std::cout << "Generated: " << filename
                 << " (V=" << vertices << ", P=" << prob
                 << ", E=" << g.getNumEdges() << ")\n";
   }
}

void TestDataGenerator::generateLargeGraphs() {
   std::cout << "Generating large scalability test graphs...\n";
  
   std::vector<std::tuple<int, double, std::string>> graphs = {
       {500, 0.05, "sparse_500_005"},
       {500, 0.15, "medium_500_015"},
       {1000, 0.02, "sparse_1000_002"},
       {1000, 0.1, "medium_1000_010"}
   };
  
   for (const auto& [vertices, prob, name] : graphs) {
       Graph g = GraphGenerator::generateRandomGraph(vertices, prob);
       std::string filename = "data/large/scalability/" + name + ".txt";
       GraphLoader::saveGraph(g, filename);
       std::cout << "Generated: " << filename << "\n";
   }
}
