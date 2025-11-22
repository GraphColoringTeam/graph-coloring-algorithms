#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <string>
#include <vector>

// Kết quả test của một thuật toán trên một đồ thị
struct TestResult {
    std::string algorithmName;
    std::string graphFile;
    int numColors;
    double timeMs;
    bool success;
};

// Chạy một thuật toán trên một đồ thị
TestResult runAlgorithmOnGraph(const std::string &graphPath, const std::string &algorithmName);

// Chạy tất cả thuật toán trên toàn bộ dataset
void runAllAlgorithmsOnDataset(const std::vector<std::string> &algorithms, const std::string &datasetFolder);

#endif // TEST_RUNNER_H
