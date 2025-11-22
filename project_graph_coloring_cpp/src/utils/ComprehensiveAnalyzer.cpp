// src/utils/ComprehensiveAnalyzer.cpp
#include "ComprehensiveAnalyzer.h"
#include "Benchmark.h"
#include "../graph_representation/GraphLoader.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <unordered_map>
#include <climits>

using namespace std;
namespace fs = filesystem;

// Helper function in vector
template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) os << ", ";
    }
    os << "]";
    return os;
}

// Forward declarations
int calculateActualChromaticNumber(const vector<BenchmarkResult>& results, const string& graph_file);
double calculateOptimalityGap(int actual_colors, int chromatic_number);

// Helper function to check if algorithm should run on this graph
bool shouldRunAlgorithm(const string& algorithm, int vertices) {
    // CHUẨN HÓA: Small graphs = ≤15 vertices
    if (algorithm == "Backtracking" && vertices > 15) return false;
    if (algorithm == "ConstraintProgramming" && vertices > 15) return false;
    return true;
}

// Hàm tính chromatic number thực tế từ kết quả các thuật toán
int calculateActualChromaticNumber(const vector<BenchmarkResult>& results, const string& graph_file) {
    int min_colors = INT_MAX;
    for (const auto& result : results) {
        if (result.graph_name == graph_file && result.is_valid) {
            if (result.colors_used < min_colors) min_colors = result.colors_used;
        }
    }
    return (min_colors == INT_MAX) ? -1 : min_colors;
}

// Hàm tính optimality gap
double calculateOptimalityGap(int actual_colors, int chromatic_number) {
    if (chromatic_number <= 0) return -1.0;
    return (double)(actual_colors - chromatic_number) / chromatic_number * 100.0;
}

// HÀM MỚI: Tự động scan tất cả file trong thư mục test data
vector<string> scanAllTestFiles() {
    vector<string> all_files;
    vector<string> directories = {
        "data/small/correctness",
        "data/medium/performance",
        "data/large/scalability"
    };

    for (const auto& dir : directories) {
        try {
            if (fs::exists(dir) && fs::is_directory(dir)) {
                cout << "📁 Scanning directory: " << dir << endl;
                for (const auto& entry : fs::directory_iterator(dir)) {
                    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                        all_files.push_back(entry.path().string());
                        cout << "   ✅ Found: " << entry.path().filename() << endl;
                    }
                }
            } else {
                cout << "   ⚠️ Directory not found: " << dir << endl;
            }
        } catch (const exception& e) {
            cout << "   ❌ Error scanning " << dir << ": " << e.what() << endl;
        }
    }

    return all_files;
}

// ======================= Run comprehensive analysis =======================
void ComprehensiveAnalyzer::runComprehensiveAnalysis() {
    vector<string> algorithms = {
        "Backtracking", "DSATUR", "SimulatedAnnealing",
        "ConstraintProgramming", "RLF"
    };

    vector<BenchmarkResult> all_results;

    cout << "🔍 Scanning for test data files..." << endl;
    vector<string> test_files = scanAllTestFiles();

    if (test_files.empty()) {
        cout << "❌ No test files found! Please generate test data first (option 2)." << endl;
        return;
    }

    cout << "\n🎯 Found " << test_files.size() << " test files. Starting benchmark..." << endl;

    // PHASE 1: Chạy tất cả thuật toán trên small graphs để tính chromatic number
    vector<string> small_graphs;
    for (const auto& file : test_files) {
        Graph graph = GraphLoader::loadGraphFromFile(file);
        if (graph.getNumVertices() <= 15) small_graphs.push_back(file);
    }

    cout << "\n📊 PHASE 1: Calculating chromatic numbers on " << small_graphs.size() << " small graphs..." << endl;

    unordered_map<string, int> chromatic_numbers;

    for (const auto& file : small_graphs) {
        Graph graph = GraphLoader::loadGraphFromFile(file);
        int vertices = graph.getNumVertices();
        cout << "   🎯 Graph: " << fs::path(file).filename().string() 
             << " (V=" << vertices << ")" << endl;

        vector<int> color_counts;
        for (const auto& algo : algorithms) {
            if (!shouldRunAlgorithm(algo, vertices)) {
                cout << "      ⏭️ Skipping " << algo << " (graph too large)" << endl;
                continue;
            }

            auto result = Benchmark::runTest(algo, graph, file);
            all_results.push_back(result);

            if (result.is_valid) {
                color_counts.push_back(result.colors_used);
                cout << "      " << algo << ": " << result.colors_used 
                     << " colors, " << result.runtime_ms << " ms, ✅" << endl;
            } else {
                cout << "      " << algo << ": " << result.colors_used 
                     << " colors, " << result.runtime_ms << " ms, ❌ INVALID" << endl;
            }
        }

        if (!color_counts.empty()) {
            int chromatic = *min_element(color_counts.begin(), color_counts.end());
            chromatic_numbers[file] = chromatic;
            cout << "   🎯 Chromatic number: " << chromatic << endl;
        } else {
            cout << "   ⚠️ No valid coloring found for chromatic number calculation" << endl;
        }
        cout << endl;
    }

    // PHASE 2: Chạy các thuật toán scalable trên tất cả graphs
    cout << "\n📊 PHASE 2: Running scalable algorithms on all graphs..." << endl;
    vector<string> scalable_algorithms = {"DSATUR", "RLF", "SimulatedAnnealing"};

    for (const auto& file : test_files) {
        Graph graph = GraphLoader::loadGraphFromFile(file);
        int vertices = graph.getNumVertices();
        if (vertices <= 15) continue;

        cout << "   📈 Graph: " << fs::path(file).filename().string() 
             << " (V=" << vertices << ")" << endl;

        for (const auto& algo : scalable_algorithms) {
            auto result = Benchmark::runTest(algo, graph, file);
            all_results.push_back(result);

            double optimality_gap = -1.0;
            if (chromatic_numbers.find(file) != chromatic_numbers.end())
                optimality_gap = calculateOptimalityGap(result.colors_used, chromatic_numbers[file]);

            cout << "      " << algo << ": " << result.colors_used 
                 << " colors, " << result.runtime_ms << " ms"
                 << (optimality_gap >= 0 ? ", Gap: " + to_string(optimality_gap) + "%" : "")
                 << ", " << (result.is_valid ? "✅" : "❌") << endl;
        }
        cout << endl;
    }

    if (all_results.empty()) {
        cout << "⚠️  No results to export!" << endl;
        return;
    }

    cout << "\n💾 Exporting " << all_results.size() << " results to CSV..." << endl;
    Benchmark::exportToCSV(all_results, "comprehensive_results.csv");

    generateEnhancedVisualizationScripts(all_results, chromatic_numbers);

    cout << "✅ Giai đoạn 1 completed!" << endl;
    cout << "📈 Results saved to: comprehensive_results.csv" << endl;
    cout << "🐍 Run: python visualize_results.py to see enhanced charts" << endl;
}

// ======================= Generate enhanced visualization script =======================
void ComprehensiveAnalyzer::generateEnhancedVisualizationScripts(
    const vector<BenchmarkResult>& results, 
    const unordered_map<string, int>& chromatic_numbers) {

    ofstream py_script("visualize_results.py");

    py_script << R"(
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import numpy as np

print('=== ENHANCED GRAPH COLORING BENCHMARK ANALYSIS ===')
print('=== GIAI ĐOẠN 1: ĐẦY ĐỦ METRICS ===')

df = pd.read_csv('comprehensive_results.csv')
print(f'Total records: {len(df)}')
print(f'Algorithms: {", ".join(df["Algorithm"].unique())}')
print(f'Graph files: {len(df["Graph"].unique())}')

df['Graph_Name'] = df['Graph'].apply(lambda x: os.path.basename(x))

def classify_graph_size(vertices):
    if vertices <= 15: return 'Small (≤15)'
    elif vertices <= 100: return 'Medium (16-100)' 
    else: return 'Large (>100)'

df['Graph_Size'] = df['Vertices'].apply(classify_graph_size)

print('\nGraph size distribution (CHUẨN HÓA):')
print(df['Graph_Size'].value_counts())

# ==================== METRICS TỔNG QUAN ====================
success_rate = df.groupby('Algorithm')['Valid'].apply(lambda x: (x == 'Yes').mean() * 100)
print('\n1. SUCCESS RATE (%):')
for algo, rate in success_rate.items():
    print(f'   {algo:<20}: {rate:.1f}%')

# 2. Memory Usage Analysis
if 'MemoryUsageKB' in df.columns:
    memory_usage = df.groupby('Algorithm')['MemoryUsageKB'].agg(['mean', 'max'])
    print('\n2. MEMORY USAGE (KB):')
    for algo in memory_usage.index:
        avg_mem = memory_usage.loc[algo, 'mean']
        max_mem = memory_usage.loc[algo, 'max']
        print(f'   {algo:<20}: Avg={avg_mem:.0f}KB, Max={max_mem:.0f}KB')

# 3. Time Statistics
time_stats = df.groupby('Algorithm')['Time(ms)'].agg(['mean', 'std', 'min', 'max'])
print('\n3. RUNTIME STATISTICS (ms):')
for algo in time_stats.index:
    avg_time = time_stats.loc[algo, 'mean']
    std_time = time_stats.loc[algo, 'std'] 
    max_time = time_stats.loc[algo, 'max']
    print(f'   {algo:<20}: Avg={avg_time:.1f}±{std_time:.1f}, Max={max_time:.1f}')

# 4. Colors Used
color_stats = df.groupby('Algorithm')['Colors'].agg(['mean', 'std', 'min', 'max'])
print('\n4. COLORS USED:')
for algo in color_stats.index:
    avg_colors = color_stats.loc[algo, 'mean']
    std_colors = color_stats.loc[algo, 'std']
    min_colors = color_stats.loc[algo, 'min']
    print(f'   {algo:<20}: Avg={avg_colors:.1f}±{std_colors:.1f}, Min={min_colors}')

# ==================== VISUALIZATION MỚI ====================

# 1. Memory vs Time Scatter Plot
if 'MemoryUsageKB' in df.columns:
    plt.figure(figsize=(12, 8))
    for algo in df['Algorithm'].unique():
        algo_data = df[df['Algorithm'] == algo]
        plt.scatter(algo_data['Time(ms)'], algo_data['MemoryUsageKB'], 
                   label=algo, alpha=0.7, s=60)
    plt.xlabel('Time (ms)')
    plt.ylabel('Memory Usage (KB)')
    plt.title('Time vs Memory Usage by Algorithm\n(Đánh giá trade-off thời gian và bộ nhớ)')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('time_memory_tradeoff.png', dpi=300, bbox_inches='tight')
    print('\nSaved: time_memory_tradeoff.png')

# 2. Success Rate Bar Chart
plt.figure(figsize=(10, 6))
success_data = df.groupby('Algorithm')['Valid'].apply(lambda x: (x == 'Yes').mean() * 100)
success_data.plot(kind='bar', color='lightgreen')
plt.title('Success Rate by Algorithm\n(Tỷ lệ tìm được coloring hợp lệ)')
plt.ylabel('Success Rate (%)')
plt.xticks(rotation=45)
plt.ylim(0, 105)
plt.tight_layout()
plt.savefig('success_rate.png', dpi=300, bbox_inches='tight')
print('Saved: success_rate.png')

# 3. Memory Usage Comparison
if 'MemoryUsageKB' in df.columns:
    plt.figure(figsize=(12, 6))
    sns.boxplot(data=df, x='Algorithm', y='MemoryUsageKB')
    plt.title('Memory Usage Distribution by Algorithm\n(Phân phối sử dụng bộ nhớ)')
    plt.xticks(rotation=45)
    plt.ylabel('Memory Usage (KB)')
    plt.tight_layout()
    plt.savefig('memory_usage.png', dpi=300, bbox_inches='tight')
    print('Saved: memory_usage.png')

# 4. Performance Profile (Cumulative Distribution)
plt.figure(figsize=(12, 8))
for algo in df['Algorithm'].unique():
    algo_data = df[df['Algorithm'] == algo]
    if len(algo_data) > 0:
        sorted_times = np.sort(algo_data['Time(ms)'])
        y_vals = np.arange(1, len(sorted_times) + 1) / len(sorted_times)
        plt.plot(sorted_times, y_vals, label=algo, linewidth=2)

plt.xlabel('Time (ms)')
plt.ylabel('Cumulative Probability')
plt.title('Performance Profile: Time Distribution\n(Phân phối thời gian thực thi)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('performance_profile.png', dpi=300, bbox_inches='tight')
print('Saved: performance_profile.png')

print('\n✅ Visualization completed! Check the generated PNG files for detailed analysis.')
)";

    py_script.close();
    cout << "✅ Generated enhanced visualization script with memory tracking" << endl;
}