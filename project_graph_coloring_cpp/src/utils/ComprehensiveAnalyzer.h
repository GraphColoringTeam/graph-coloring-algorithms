// src/utils/ComprehensiveAnalyzer.h
#ifndef COMPREHENSIVE_ANALYZER_H
#define COMPREHENSIVE_ANALYZER_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Benchmark.h"

class ComprehensiveAnalyzer {
public:
    static void runComprehensiveAnalysis();
    
private:
    static void generateEnhancedVisualizationScripts(
        const std::vector<BenchmarkResult>& results, 
        const std::unordered_map<std::string, int>& chromatic_numbers);
};

#endif