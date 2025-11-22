#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <string>
#include <vector>
#include <chrono>
#include <cstddef>

class Graph;

struct BenchmarkResult {
    std::string algorithm_name;
    std::string graph_name;
    int vertices;
    int edges;
    int colors_used;
    double runtime_ms;
    bool is_valid;
    int iterations;
    std::string solution_quality;
    double stability_std_dev;
    size_t memory_usage_kb;
    
    BenchmarkResult();
};

class Benchmark {
public:
    static BenchmarkResult runTest(const std::string& algorithm_name, 
                                   Graph& graph, 
                                   const std::string& graph_file);
    
    static bool validateColoring(const Graph& graph, const std::vector<int>& coloring);
    
    static void exportToCSV(const std::vector<BenchmarkResult>& results, 
                           const std::string& filename);
    
    static double calculateStandardDeviation(const std::vector<double>& values);
};

#endif