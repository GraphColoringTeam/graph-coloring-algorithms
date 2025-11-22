#include <iostream>
#include <string>
#include <sys/stat.h>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include "../../utils/utils.h"

#include "graph_representation/Graph.h"
#include "graph_representation/GraphGenerator.h"
#include "graph_representation/GraphLoader.h"
#include "algorithms/backtracking/BacktrackingSolver.h"
#include "algorithms/dsatur/DSATURSolver.h"
#include "algorithms/simulated_annealing/SimulatedAnnealingSolver.h"
#include "algorithms/rlf/RLFSolver.h"
#include "algorithms/constraint_programming/CPSolver.h"
#include "utils/TestDataGenerator.h"
#include "utils/TestRunner.h"
#include "utils/ComprehensiveAnalyzer.h"
#include "utils/Benchmark.h"

using namespace std;
namespace fs = filesystem;

// Hàm kiểm tra đồ thị tạo thủ công
void testManualGraph() {
    cout << "Kiểm tra lớp Graph...\n";

    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    g.printGraph();

    cout << "Số đỉnh: " << g.getNumVertices() << "\n"
         << "Bậc của đỉnh 1: " << g.getDegree(1) << "\n"
         << "Đỉnh 0 và 2 có kề nhau không? " << (g.isAdjacent(0, 2) ? "Có" : "Không") << "\n"
         << "Đỉnh 0 và 1 có kề nhau không? " << (g.isAdjacent(0, 1) ? "Có" : "Không") << "\n"
         << "Các đỉnh kề của đỉnh 3:";

    for (int neighbor : g.getNeighbors(3)) {
        cout << " " << neighbor;
    }
    cout << "\nKiểm tra hoàn tất lớp Graph.\n"
         << "---------------------------------\n";
}

// Hàm kiểm tra GraphGenerator
void testRandomGraphs() {
    cout << "Kiểm tra GraphGenerator...\n";

    // Đồ thị ngẫu nhiên với xác suất cạnh
    Graph randomG1 = GraphGenerator::generateRandomGraph(10, 0.3);
    cout << "Đồ thị ngẫu nhiên 1 (10 đỉnh, p=0.3):\n";
    randomG1.printGraph();
    cout << "Số đỉnh: " << randomG1.getNumVertices()
         << ", Bậc đỉnh 0: " << randomG1.getDegree(0) << "\n"
         << "---------------------------------\n";

    // Đồ thị ngẫu nhiên với số cạnh xác định
    Graph randomG2 = GraphGenerator::generateRandomGraphByEdges(8, 10);
    cout << "Đồ thị ngẫu nhiên 2 (8 đỉnh, 10 cạnh):\n";
    randomG2.printGraph();
    cout << "Số đỉnh: " << randomG2.getNumVertices()
         << ", Bậc đỉnh 0: " << randomG2.getDegree(0) << "\n"
         << "---------------------------------\n";
}

// Hàm kiểm tra các trường hợp đặc biệt của GraphGenerator
void testGeneratorEdgeCases() {
    cout << "Kiểm tra các trường hợp đặc biệt của GraphGenerator...\n";

    // Đồ thị rỗng
    Graph emptyGraph = GraphGenerator::generateRandomGraph(0, 0.5);
    cout << "Đồ thị rỗng - Số đỉnh: " << emptyGraph.getNumVertices() << "\n";

    // Đồ thị với xác suất 0 (không có cạnh)
    Graph noEdgesGraph = GraphGenerator::generateRandomGraph(5, 0.0);
    cout << "Đồ thị không cạnh - Bậc đỉnh 0: " << noEdgesGraph.getDegree(0) << "\n";

    // Đồ thị với xác suất 1 (đồ thị đầy đủ)
    Graph fullGraph = GraphGenerator::generateRandomGraph(4, 1.0);
    cout << "Đồ thị đầy đủ 4 đỉnh - Bậc đỉnh 0: " << fullGraph.getDegree(0) << "\n";

    cout << "---------------------------------\n";
}

// HÀM MỚI: Kiểm tra GraphLoader
void testGraphLoader() {
    cout << "Kiểm tra GraphLoader...\n";

    string testFilename = "test_graph.txt";

    Graph loadedG = GraphLoader::loadGraphFromFile(testFilename);
    if (loadedG.getNumVertices() > 0) {
        cout << "Đồ thị được tải từ file " << testFilename << ":\n";
        loadedG.printGraph();
        cout << "Số đỉnh: " << loadedG.getNumVertices() << ", Số cạnh: " << loadedG.getNumEdges() << "\n";
    }
    cout << "---------------------------------\n";

    // Kiểm tra trường hợp file không tồn tại
    cout << "Kiểm tra tải file không tồn tại...\n";
    Graph nonExistentG = GraphLoader::loadGraphFromFile("non_existent_file.txt");
    cout << "Số đỉnh của đồ thị không tồn tại: " << nonExistentG.getNumVertices() << "\n";
    cout << "---------------------------------\n";
}

// HÀM MỚI: Kiểm tra BacktrackingSolver
void testBacktrackingSolver() {
    cout << "Kiểm tra BacktrackingSolver...\n";

    // Đồ thị C5
    Graph g_c5(5);
    g_c5.addEdge(0, 1);
    g_c5.addEdge(1, 2);
    g_c5.addEdge(2, 3);
    g_c5.addEdge(3, 4);
    g_c5.addEdge(4, 0);

    cout << "Đồ thị C5 (chu trình 5 đỉnh):\n";
    g_c5.printGraph();

    Algorithms::Backtracking::BacktrackingSolver solver_c5;
    vector<int> colors_c5;
    int minColors_c5 = solver_c5.solve(g_c5, colors_c5);

    if (minColors_c5 != -1) {
        cout << "Số màu tối thiểu cho C5: " << minColors_c5 << "\n";
        cout << "Cách tô màu C5: ";
        for (int i = 0; i < g_c5.getNumVertices(); ++i) {
            cout << "V" << i << ":" << colors_c5[i] << " ";
        }
        cout << "\n";
    } else {
        cout << "Không thể tô màu đồ thị C5.\n";
    }
    cout << "---------------------------------\n";

    // Kiểm tra với đồ thị từ file
    cout << "Đồ thị từ test_graph.txt:\n";
    Graph loadedG = GraphLoader::loadGraphFromFile("test_graph.txt");
    if (loadedG.getNumVertices() > 0) {
        loadedG.printGraph();
        Algorithms::Backtracking::BacktrackingSolver solver_loaded;
        vector<int> colors_loaded;
        int minColors_loaded = solver_loaded.solve(loadedG, colors_loaded);

        if (minColors_loaded != -1) {
            cout << "Số màu tối thiểu cho đồ thị từ file: " << minColors_loaded << "\n";
            cout << "Cách tô màu: ";
            for (int i = 0; i < loadedG.getNumVertices(); ++i) {
                cout << "V" << i << ":" << colors_loaded[i] << " ";
            }
            cout << "\n";
        } else {
            cout << "Không thể tô màu đồ thị từ file.\n";
        }
    }
    cout << "---------------------------------\n";

    // Kiểm tra đồ thị rỗng
    cout << "Kiểm tra đồ thị rỗng:\n";
    Graph emptyG(0);
    Algorithms::Backtracking::BacktrackingSolver solver_empty;
    vector<int> colors_empty;
    int minColors_empty = solver_empty.solve(emptyG, colors_empty);
    cout << "Số màu cho đồ thị rỗng: " << minColors_empty << "\n";
    cout << "---------------------------------\n";
}

// HÀM MỚI: Kiểm tra DSATURSolver
void testDSATURSolver() {
    cout << "Kiểm tra DSATURSolver...\n";
    Algorithms::DSATUR::DSATURSolver dsaturSolver;
    vector<int> coloring;
    int numColors;

    // Đồ thị C5
    Graph c5(5);
    c5.addEdge(0, 1); c5.addEdge(1, 2); c5.addEdge(2, 3); c5.addEdge(3, 4); c5.addEdge(4, 0);
    cout << "Đồ thị C5 (chu trình 5 đỉnh):\n";
    c5.printGraph();
    numColors = dsaturSolver.solve(c5, coloring);
    cout << "Số màu DSATUR cho C5: " << numColors << "\n";
    cout << "Cách tô màu C5:";
    for (int i = 0; i < c5.getNumVertices(); ++i) {
        cout << " V" << i << ":" << coloring[i];
    }
    cout << "\n---------------------------------\n";

    // Đồ thị từ test_graph.txt
    Graph loadedG = GraphLoader::loadGraphFromFile("test_graph.txt");
    if (loadedG.getNumVertices() > 0) {
        cout << "Đồ thị từ test_graph.txt:\n";
        loadedG.printGraph();
        numColors = dsaturSolver.solve(loadedG, coloring);
        cout << "Số màu DSATUR cho đồ thị từ file: " << numColors << "\n";
        cout << "Cách tô màu:";
        for (int i = 0; i < loadedG.getNumVertices(); ++i) {
            cout << " V" << i << ":" << coloring[i];
        }
        cout << "\n---------------------------------\n";
    }

    // Đồ thị ngẫu nhiên
    Graph randomG = GraphGenerator::generateRandomGraph(20, 0.2);
    cout << "Đồ thị ngẫu nhiên (20 đỉnh, p=0.2):\n";
    numColors = dsaturSolver.solve(randomG, coloring);
    cout << "Số màu DSATUR cho đồ thị ngẫu nhiên: " << numColors << "\n";
    cout << "---------------------------------\n";

    // Đồ thị rỗng
    Graph emptyGraph(0);
    numColors = dsaturSolver.solve(emptyGraph, coloring);
    cout << "Số màu DSATUR cho đồ thị rỗng: " << numColors << "\n";
    cout << "---------------------------------\n";
}

// HÀM MỚI: Kiểm tra SimulatedAnnealingSolver
void testSimulatedAnnealingSolver() {
    cout << "Kiểm tra SimulatedAnnealingSolver...\n";
    Algorithms::SimulatedAnnealing::SimulatedAnnealingSolver saSolver;
    vector<int> coloring;
    int numColors;

    // Đồ thị C5
    Graph c5(5);
    c5.addEdge(0, 1); c5.addEdge(1, 2); c5.addEdge(2, 3); c5.addEdge(3, 4); c5.addEdge(4, 0);
    cout << "Đồ thị C5 (chu trình 5 đỉnh):\n";
    c5.printGraph();
    // Sử dụng solveAdaptive cho SA để tối ưu số màu
    cout << "  Sử dụng SA Adaptive để tối ưu số màu (tham số mặc định: 2000 iter, 1000.0 temp, 0.95 cooling).\n";
    numColors = saSolver.solveAdaptive(c5, coloring); 
    cout << "Số màu SA cho C5: " << numColors << "\n";
    cout << "Cách tô màu C5 (nếu hợp lệ):";
    if (saSolver.countConflicts(c5, coloring) == 0) {
        for (int i = 0; i < c5.getNumVertices(); ++i) {
            cout << " V" << i << ":" << coloring[i];
        }
    } else {
        cout << " (Không tìm thấy giải pháp hợp lệ)";
    }
    cout << "\n---------------------------------\n";

    // Đồ thị từ test_graph.txt
    Graph loadedG = GraphLoader::loadGraphFromFile("test_graph.txt");
    if (loadedG.getNumVertices() > 0) {
        cout << "Đồ thị từ test_graph.txt:\n";
        loadedG.printGraph();
        // Tăng số iterations cho đồ thị từ file để có kết quả tốt hơn
        cout << "  Sử dụng SA Adaptive (20000 iter, 1000.0 temp, 0.99 cooling).\n";
        numColors = saSolver.solveAdaptive(loadedG, coloring, 20000, 1000.0, 0.99);
        cout << "Số màu SA cho đồ thị từ file: " << numColors << "\n";
        cout << "Cách tô màu (nếu hợp lệ):";
        if (saSolver.countConflicts(loadedG, coloring) == 0) {
            for (int i = 0; i < loadedG.getNumVertices(); ++i) {
                cout << " V" << i << ":" << coloring[i];
            }
        } else {
            cout << " (Không tìm thấy giải pháp hợp lệ)";
        }
        cout << "\n---------------------------------\n";
    }

    // Đồ thị ngẫu nhiên
    Graph randomG = GraphGenerator::generateRandomGraph(20, 0.2);
    cout << "Đồ thị ngẫu nhiên (20 đỉnh, p=0.2):\n";
    // Tăng số iterations cho đồ thị ngẫu nhiên
    cout << "  Sử dụng SA Adaptive (50000 iter, 1000.0 temp, 0.99 cooling).\n";
    numColors = saSolver.solveAdaptive(randomG, coloring, 50000, 1000.0, 0.99);
    cout << "Số màu SA cho đồ thị ngẫu nhiên: " << numColors << "\n";
    cout << "Số xung đột còn lại: " << saSolver.countConflicts(randomG, coloring) << "\n";
    cout << "---------------------------------\n";

    // Đồ thị rỗng
    Graph emptyGraph(0);
    // Với đồ thị rỗng, hàm solve (không phải adaptive) có thể handle tốt
    numColors = saSolver.solve(emptyGraph, coloring, 1);
    cout << "Số màu SA (non-adaptive) cho đồ thị rỗng: " << numColors << "\n";
    cout << "---------------------------------\n";
}

// Đã xóa hàm testBronKerboschSolver()

// HÀM MỚI: Kiểm tra RLFSolver
void testRLFSolver() {
    using namespace Algorithms::RLF;
    
    cout << "Kiểm tra RLFSolver..." << endl;

    // Đồ thị C5
    Graph c5_graph(5);
    c5_graph.addEdge(0, 1); c5_graph.addEdge(1, 2); c5_graph.addEdge(2, 3);
    c5_graph.addEdge(3, 4); c5_graph.addEdge(4, 0);
    
    vector<int> c5_coloring;
    RLFSolver rlf_c5_solver;
    int c5_colors = rlf_c5_solver.solve(c5_graph, c5_coloring);
    
    cout << "Đồ thị C5 (chu trình 5 đỉnh):" << endl;
    c5_graph.printGraph();
    cout << "Số màu RLF cho C5: " << c5_colors << endl;
    cout << "Cách tô màu C5:\n";
    rlf_c5_solver.printColoring(c5_coloring);
    cout << "---------------------------------" << endl;

    // Đồ thị từ file
    Graph file_graph = GraphLoader::loadGraphFromFile("test_graph.txt");
    vector<int> file_coloring;
    RLFSolver rlf_file_solver;
    int file_colors = rlf_file_solver.solve(file_graph, file_coloring);
    
    cout << "Đồ thị từ test_graph.txt:" << endl;
    file_graph.printGraph();
    cout << "Số màu RLF cho đồ thị từ file: " << file_colors << endl;
    cout << "Cách tô màu:\n";
    rlf_file_solver.printColoring(file_coloring);
    cout << "---------------------------------" << endl;
}

// HÀM MỚI: Kiểm tra CPSolver
void testCPSolver() {
    cout << "Kiểm tra CPSolver...\n";
    Algorithms::CP::CPSolver cpSolver;
    vector<int> coloring;
    int numColors;

    // Đồ thị C5
    Graph c5(5);
    c5.addEdge(0, 1); c5.addEdge(1, 2); c5.addEdge(2, 3); c5.addEdge(3, 4); c5.addEdge(4, 0);
    cout << "Đồ thị C5 (chu trình 5 đỉnh):\n";
    c5.printGraph();
    numColors = cpSolver.solve(c5, coloring);
    cout << "Số màu CP cho C5: " << numColors << "\n";
    cout << "Cách tô màu C5:";
    for (int i = 0; i < c5.getNumVertices(); ++i) {
        cout << " V" << i << ":" << coloring[i];
    }
    cout << "\n---------------------------------\n";

    // Đồ thị từ test_graph.txt (nếu nhỏ)
    Graph loadedG = GraphLoader::loadGraphFromFile("test_graph.txt");
    if (loadedG.getNumVertices() > 0 && loadedG.getNumVertices() <= 15) {
        cout << "Đồ thị từ test_graph.txt:\n";
        loadedG.printGraph();
        numColors = cpSolver.solve(loadedG, coloring);
        cout << "Số màu CP cho đồ thị từ file: " << numColors << "\n";
        cout << "Cách tô màu:";
        for (int i = 0; i < loadedG.getNumVertices(); ++i) {
            cout << " V" << i << ":" << coloring[i];
        }
        cout << "\n---------------------------------\n";
    }

    // Đồ thị rỗng
    Graph emptyGraph(0);
    numColors = cpSolver.solve(emptyGraph, coloring);
    cout << "Số màu CP cho đồ thị rỗng: " << numColors << "\n";
    cout << "---------------------------------\n";
}

// HÀM MỚI: Chạy tất cả test
void runAllTests() {
    cout << "=== CHẠY TẤT CẢ TEST CASES ===\n";
    testManualGraph();
    testRandomGraphs();
    testGeneratorEdgeCases();
    testGraphLoader();
    testBacktrackingSolver();
    testDSATURSolver();
    testSimulatedAnnealingSolver();
    // Đã xóa: testBronKerboschSolver();
    testRLFSolver();
    testCPSolver();
    cout << "=== HOÀN TẤT TẤT CẢ TEST CASES ===\n";
}

// HÀM MỚI: Benchmark trên một đồ thị
void benchmarkOnGraph(const Graph& graph, const string& graphName) {
    cout << "\n=== BENCHMARK TRÊN " << graphName << " ===\n";
    cout << "Kích thước: " << graph.getNumVertices() << " đỉnh, " 
         << graph.getNumEdges() << " cạnh\n";
    
    vector<int> coloring;
    
    // DSATUR
    auto start = chrono::high_resolution_clock::now();
    Algorithms::DSATUR::DSATURSolver dsaturSolver;
    int dsaturColors = dsaturSolver.solve(graph, coloring);
    auto end = chrono::high_resolution_clock::now();
    auto dsaturTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "DSATUR: " << dsaturColors << " màu, " << dsaturTime << "ms\n";
    
    // RLF
    start = chrono::high_resolution_clock::now();
    Algorithms::RLF::RLFSolver rlfSolver;
    int rlfColors = rlfSolver.solve(graph, coloring);
    end = chrono::high_resolution_clock::now();
    auto rlfTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "RLF: " << rlfColors << " màu, " << rlfTime << "ms\n";
    
    // Simulated Annealing (sử dụng solveAdaptive)
    // Tùy chỉnh tham số nếu cần, ví dụ: tăng iterations cho đồ thị lớn
    if (graph.getNumVertices() <= 100) { // Giới hạn kích thước để SA không chạy quá lâu
        start = chrono::high_resolution_clock::now();
        Algorithms::SimulatedAnnealing::SimulatedAnnealingSolver saSolver;
        // Sử dụng tham số hợp lý cho benchmark:
        // Cân nhắc tăng iterations để SA có cơ hội tìm được số màu tốt hơn
        int saColors = saSolver.solveAdaptive(graph, coloring, 10000, 1000.0, 0.98); 
        end = chrono::high_resolution_clock::now();
        auto saTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Simulated Annealing: " << saColors << " màu, " << saTime << "ms\n";
    } else {
        cout << "Simulated Annealing: Bỏ qua (đồ thị > 100 đỉnh trong benchmark).\n";
    }
    
    // Backtracking
    if (graph.getNumVertices() <= 20) {
        start = chrono::high_resolution_clock::now();
        Algorithms::Backtracking::BacktrackingSolver btSolver;
        int btColors = btSolver.solve(graph, coloring);
        end = chrono::high_resolution_clock::now();
        auto btTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Backtracking: " << btColors << " màu, " << btTime << "ms\n";
    } else {
        cout << "Backtracking: Bỏ qua (đồ thị > 20 đỉnh).\n";
    }
    
    // Constraint Programming
    if (graph.getNumVertices() <= 15) {
        start = chrono::high_resolution_clock::now();
        Algorithms::CP::CPSolver cpSolver;
        int cpColors = cpSolver.solve(graph, coloring);
        end = chrono::high_resolution_clock::now();
        auto cpTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Constraint Programming: " << cpColors << " màu, " << cpTime << "ms\n";
    } else {
        cout << "Constraint Programming: Bỏ qua (đồ thị > 15 đỉnh).\n";
    }
    
    // Đã xóa khối Bron-Kerbosch
}

// HÀM MỚI: Benchmark trên nhiều đồ thị
void runBenchmarks() {
    cout << "=== CHẠY BENCHMARK ===\n";
    
    // Tạo folder
    system("mkdir -p data/medium/performance");
    
    // Benchmark trên đồ thị có sẵn
    ifstream testFile("test_graph.txt");
    if (testFile.good()) {
        testFile.close();
        Graph testGraph = GraphLoader::loadGraphFromFile("test_graph.txt");
        if (testGraph.getNumVertices() > 0) {
            benchmarkOnGraph(testGraph, "TEST_GRAPH");
        }
    }
    
    // Benchmark trên đồ thị generated
    cout << "\n--- Generating test graphs for benchmark ---\n";
    vector<tuple<int, double, string>> testGraphs = {
        {10, 0.4, "Very Small (10 vertices)"},
        {15, 0.3, "Small (15 vertices)"},
        {50, 0.3, "Medium (50 vertices)"},
        {100, 0.2, "Medium Sparse (100 vertices)"},
        // Đối với benchmark chung, SA sẽ rất chậm trên đồ thị 200 đỉnh nếu không tối ưu tham số
        // Nên tôi tạm thời bỏ qua đồ thị 200 đỉnh cho SA trong phần này
        // {200, 0.1, "Large Sparse (200 vertices)"} 
    };
    
    for (const auto& [vertices, prob, name] : testGraphs) {
        Graph g = GraphGenerator::generateRandomGraph(vertices, prob);
        benchmarkOnGraph(g, name);
        
        string filename = "data/medium/performance/benchmark_" + 
                         to_string(vertices) + "_" + to_string((int)(prob*100)) + ".txt";
        GraphLoader::saveGraph(g, filename);
    }
}

// HÀM MỚI: Test trên file cụ thể
void testOnSpecificFile() {
    string filename;
    cout << "Nhập tên file đồ thị: ";
    cin >> filename;

    ifstream fileCheck(filename);
    if (!fileCheck.is_open()) {
        cout << "❌ File không tồn tại!\n";
        return;
    }
    fileCheck.close();

    Graph graph = GraphLoader::loadGraphFromFile(filename);
    if (graph.getNumVertices() == 0) {
        cout << "❌ Lỗi khi đọc file hoặc file trống!\n";
        return;
    }

    cout << "\n=== ĐỒ THỊ TỪ FILE: " << filename << " ===\n";
    cout << "Số đỉnh: " << graph.getNumVertices() 
         << ", Số cạnh: " << graph.getNumEdges() << "\n";
    graph.printGraph();
    cout << "---------------------------------\n";

    vector<int> coloring;
    int numColors;

    // --- DSATUR ---
    cout << "[DSATUR] ";
    Algorithms::DSATUR::DSATURSolver dsaturSolver;
    numColors = dsaturSolver.solve(graph, coloring);
    cout << "Số màu = " << numColors << "\n";
    dsaturSolver.printColoring(coloring);
    cout << "---------------------------------\n";

    // --- RLF ---
    cout << "[RLF] ";
    Algorithms::RLF::RLFSolver rlfSolver;
    numColors = rlfSolver.solve(graph, coloring);
    cout << "Số màu = " << numColors << "\n";
    rlfSolver.printColoring(coloring);
    cout << "---------------------------------\n";

    // --- Simulated Annealing (sử dụng solveAdaptive) ---
    cout << "[Simulated Annealing] ";
    Algorithms::SimulatedAnnealing::SimulatedAnnealingSolver saSolver;
    // Tùy thuộc vào kích thước đồ thị, có thể điều chỉnh iterations
    if (graph.getNumVertices() <= 100) {
         numColors = saSolver.solveAdaptive(graph, coloring, 20000, 1000.0, 0.99); // VD: Tăng iterations cho đồ thị trung bình
    } else if (graph.getNumVertices() <= 500) {
         numColors = saSolver.solveAdaptive(graph, coloring, 50000, 1000.0, 0.99); // VD: Tăng iterations hơn nữa cho đồ thị lớn hơn
    } else {
        cout << "Bỏ qua (đồ thị quá lớn cho SA trong chế độ này).\n";
        numColors = -1; // Đánh dấu là không chạy
    }
    
    if (numColors != -1) {
        cout << "Số màu (SA) = " << numColors 
             << ", xung đột = " << saSolver.countConflicts(graph, coloring) << "\n";
        if (saSolver.countConflicts(graph, coloring) == 0)
            saSolver.printColoring(coloring);
        else
            cout << "❌ Không tìm thấy lời giải hợp lệ.\n";
    }
    cout << "---------------------------------\n";

    // --- Backtracking ---
    if (graph.getNumVertices() <= 20) {
        cout << "[Backtracking] ";
        Algorithms::Backtracking::BacktrackingSolver btSolver;
        numColors = btSolver.solve(graph, coloring);
        cout << "Số màu = " << numColors << "\n";
        btSolver.printColoring(coloring);
        cout << "---------------------------------\n";
    } else {
        cout << "[Backtracking] Bỏ qua (đồ thị > 20 đỉnh).\n";
    }

    // --- Constraint Programming ---
    if (graph.getNumVertices() <= 15) {
        cout << "[Constraint Programming] ";
        Algorithms::CP::CPSolver cpSolver;
        numColors = cpSolver.solve(graph, coloring);
        cout << "Số màu = " << numColors << "\n";
        cpSolver.printColoring(coloring);
        cout << "---------------------------------\n";
    } else {
        cout << "[Constraint Programming] Bỏ qua (đồ thị > 15 đỉnh).\n";
    }

    // Đã xóa khối Bron-Kerbosch
    cout << "✅ Hoàn tất test cho file: " << filename << "\n";
}

// ==================== THÊM CÁC HÀM PHASES TEST ====================

// Hàm kiểm tra tính hợp lệ của coloring
bool isValidColoring(const Graph& graph, const vector<int>& coloring) {
    for (int i = 0; i < graph.getNumVertices(); ++i) {
        for (int neighbor : graph.getNeighbors(i)) {
            if (coloring[i] == coloring[neighbor]) {
                return false;
            }
        }
    }
    return true;
}

// Phase A: Test tính đúng trên đồ thị nhỏ
void testCorrectness() {
    cout << "=== PHASE A: TEST TÍNH ĐÚNG (Small Graphs) ===\n";
    
    vector<string> testFiles = {
        "data/small/correctness/simple_6.txt",
        "data/small/correctness/sparse_8.txt", 
        "data/small/correctness/dense_10.txt",
        "data/small/correctness/complete_5.txt"
    };
    
    for (const auto& file : testFiles) {
        cout << "\n--- Testing: " << file << " ---\n";
        Graph g = GraphLoader::loadGraphFromFile(file);
        
        cout << "Kích thước: " << g.getNumVertices() << " đỉnh, " 
             << g.getNumEdges() << " cạnh\n";
        
        vector<int> coloring;
        
        // DSATUR
        Algorithms::DSATUR::DSATURSolver dsatur;
        int dsaturColors = dsatur.solve(g, coloring);
        cout << "DSATUR: " << dsaturColors << " màu - ";
        cout << (isValidColoring(g, coloring) ? "HỢP LỆ" : "KHÔNG HỢP LỆ") << endl;
        
        // RLF  
        Algorithms::RLF::RLFSolver rlf;
        int rlfColors = rlf.solve(g, coloring);
        cout << "RLF: " << rlfColors << " màu - ";
        cout << (isValidColoring(g, coloring) ? "HỢP LỆ" : "KHÔNG HỢP LỆ") << endl;
        
        // Backtracking (chỉ cho đồ thị nhỏ)
        if (g.getNumVertices() <= 15) {
            Algorithms::Backtracking::BacktrackingSolver bt;
            int btColors = bt.solve(g, coloring);
            cout << "Backtracking: " << btColors << " màu - ";
            cout << (isValidColoring(g, coloring) ? "HỢP LỆ" : "KHÔNG HỢP LỆ") << endl;
        } else {
             cout << "Backtracking: Bỏ qua (đồ thị > 15 đỉnh).\n";
        }
        
        // Constraint Programming (chỉ cho đồ thị rất nhỏ)
        if (g.getNumVertices() <= 10) {
            Algorithms::CP::CPSolver cp;
            int cpColors = cp.solve(g, coloring);
            cout << "Constraint Programming: " << cpColors << " màu - ";
            cout << (isValidColoring(g, coloring) ? "HỢP LỆ" : "KHÔNG HỢP LỆ") << endl;
        } else {
             cout << "Constraint Programming: Bỏ qua (đồ thị > 10 đỉnh).\n";
        }
        
        // Đã xóa khối Bron-Kerbosch
    }
}

// Phase B: Benchmark hiệu năng trên đồ thị trung bình
void runPerformanceBenchmark() {
    cout << "\n=== PHASE B: BENCHMARK HIỆU NĂNG (Medium Graphs) ===\n";
    
    vector<string> benchmarkFiles = {
        "data/medium/performance/sparse_100_010.txt",
        "data/medium/performance/medium_100_030.txt",
        "data/medium/performance/dense_100_060.txt",
        "data/medium/performance/sparse_200_008.txt",
        "data/medium/performance/medium_200_025.txt"
    };
    
    for (const auto& file : benchmarkFiles) {
        cout << "\n--- Benchmarking: " << file << " ---\n";
        Graph g = GraphLoader::loadGraphFromFile(file);
        
        cout << "Kích thước: " << g.getNumVertices() << " đỉnh, " 
             << g.getNumEdges() << " cạnh\n";
        
        vector<int> coloring;
        
        // DSATUR
        auto start = chrono::high_resolution_clock::now();
        Algorithms::DSATUR::DSATURSolver dsatur;
        int dsaturColors = dsatur.solve(g, coloring);
        auto end = chrono::high_resolution_clock::now();
        auto dsaturTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
        cout << "DSATUR: " << dsaturColors << " màu, " << dsaturTime << "ms - ";
        cout << (isValidColoring(g, coloring) ? "HỢP LỆ" : "KHÔNG HỢP LỆ") << endl;
        
        // RLF
        start = chrono::high_resolution_clock::now();
        Algorithms::RLF::RLFSolver rlf;
        int rlfColors = rlf.solve(g, coloring);
        end = chrono::high_resolution_clock::now();
        auto rlfTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
        cout << "RLF: " << rlfColors << " màu, " << rlfTime << "ms - ";
        cout << (isValidColoring(g, coloring) ? "HỢP LỆ" : "KHÔNG HỢP LỆ") << endl;
        
        // Simulated Annealing (sử dụng solveAdaptive)
        // Điều chỉnh iterations và cooling rate cho phù hợp với kích thước đồ thị
        if (g.getNumVertices() <= 200) { // Giới hạn kích thước cho SA ở đây
            start = chrono::high_resolution_clock::now();
            Algorithms::SimulatedAnnealing::SimulatedAnnealingSolver sa;
            // Tăng iterations và làm mát chậm hơn cho đồ thị lớn hơn
            int saColors = sa.solveAdaptive(g, coloring, 50000, 1000.0, 0.99); 
            end = chrono::high_resolution_clock::now();
            auto saTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            
            cout << "Simulated Annealing: " << saColors << " màu, " << saTime << "ms - ";
            cout << (isValidColoring(g, coloring) ? "HỢP LỆ" : "KHÔNG HỢP LỆ") << endl;
        } else {
             cout << "Simulated Annealing: Bỏ qua (đồ thị > 200 đỉnh trong benchmark).\n";
        }
    }
}

// Phase C: Scalability test trên đồ thị lớn
void runScalabilityTest() {
    cout << "\n=== PHASE C: SCALABILITY TEST (Large Graphs) ===\n";
    
    vector<string> largeGraphs = {
        "data/large/scalability/sparse_500_005.txt",
        "data/large/scalability/medium_500_015.txt",
        "data/large/scalability/sparse_1000_002.txt",
        "data/large/scalability/medium_1000_010.txt"
    };
    
    for (const auto& file : largeGraphs) {
        cout << "\n--- Scalability Test: " << file << " ---\n";
        Graph g = GraphLoader::loadGraphFromFile(file);
        
        cout << "Kích thước: " << g.getNumVertices() << " đỉnh, " 
             << g.getNumEdges() << " cạnh\n";
        
        vector<int> coloring;
        
        // Chỉ test các thuật toán nhanh
        auto start = chrono::high_resolution_clock::now();
        Algorithms::DSATUR::DSATURSolver dsatur;
        int dsaturColors = dsatur.solve(g, coloring);
        auto end = chrono::high_resolution_clock::now();
        auto dsaturTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
        cout << "DSATUR: " << dsaturColors << " màu, " << dsaturTime << "ms - ";
        cout << (isValidColoring(g, coloring) ? "HỢP LỆ" : "KHÔNG HỢP LỆ") << endl;
        
        start = chrono::high_resolution_clock::now();
        Algorithms::RLF::RLFSolver rlf;
        int rlfColors = rlf.solve(g, coloring);
        end = chrono::high_resolution_clock::now();
        auto rlfTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
        cout << "RLF: " << rlfColors << " màu, " << rlfTime << "ms - ";
        cout << (isValidColoring(g, coloring) ? "HỢP LỆ" : "KHÔNG HỢP LỆ") << endl;
        
        // Simulated Annealing chỉ cho đồ thị <= 500 đỉnh (và tăng iterations nhiều hơn)
        if (g.getNumVertices() <= 500) {
            start = chrono::high_resolution_clock::now();
            Algorithms::SimulatedAnnealing::SimulatedAnnealingSolver sa;
            // Tăng iterations rất nhiều cho đồ thị 500 đỉnh
            int saColors = sa.solveAdaptive(g, coloring, 100000, 1000.0, 0.995); 
            end = chrono::high_resolution_clock::now();
            auto saTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            
            cout << "Simulated Annealing: " << saColors << " màu, " << saTime << "ms - ";
            cout << (isValidColoring(g, coloring) ? "HỢP LỆ" : "KHÔNG HỢP LỆ") << endl;
        } else {
             cout << "Simulated Annealing: Bỏ qua (đồ thị > 500 đỉnh trong scalability test).\n";
        }
    }
}

// Hàm chạy tất cả phases
void runAllPhases() {
    cout << "BẮT ĐẦU TEST HỆ THỐNG 3 PHASES...\n";
    testCorrectness();          // Phase A
    runPerformanceBenchmark();  // Phase B  
    runScalabilityTest();       // Phase C
    cout << "KẾT THÚC TEST HỆ THỐNG!\n";
}

// HÀM MỚI: Chạy Comprehensive Benchmark
void runComprehensiveAnalysis() {
    ComprehensiveAnalyzer::runComprehensiveAnalysis();
}

// MENU CHÍNH
// SỬA HÀM showMenu()
void showMenu() {
    cout << "\n=== ĐỒ THỊ TÔ MÀU - MENU CHÍNH ===\n";
    cout << "1. Chạy tất cả test cases\n";
    cout << "2. Generate test data (small/medium/large)\n";
    cout << "3. Chạy benchmark performance\n";
    cout << "4. Test trên file cụ thể\n";
    cout << "5. Test từng thuật toán riêng lẻ\n";
    cout << "6. Chạy hệ thống test 3 phases (A+B+C)\n";
    cout << "7. Chạy Comprehensive Benchmark (5 tiêu chí)\n"; // Cập nhật số tiêu chí
    cout << "8. Thoát\n";  
    cout << "Lựa chọn: ";
}

int main() {
    int choice;

    do {
        showMenu();
        cin >> choice;

        switch(choice) {
            case 1:
                runAllTests();
                break;

            case 2:
                cout << "Generating test data...\n";
                TestDataGenerator::generateAllTestData();
                cout << "Đã generate xong test data!\n";
                break;

            case 3:
                runBenchmarks();  // Chạy benchmark cũ
                break;

            case 4:
                testOnSpecificFile();
                break;

            case 5: {
                cout << "\n--- Test riêng lẻ ---\n";
                cout << "1. Backtracking\n2. DSATUR\n3. RLF\n4. Simulated Annealing\n5. Constraint Programming\n"; // Đã xóa Bron-Kerbosch, đánh số lại
                cout << "Lựa chọn: ";
                int algoChoice;
                cin >> algoChoice;

                switch(algoChoice) {
                    case 1: testBacktrackingSolver(); break;
                    case 2: testDSATURSolver(); break;
                    case 3: testRLFSolver(); break;
                    case 4: testSimulatedAnnealingSolver(); break;
                    case 5: testCPSolver(); break; // Bron-Kerbosch đã bị xóa, CP là lựa chọn số 5
                    default: cout << "Lựa chọn không hợp lệ!\n"; break;
                }
                break;
            }

            case 6:
                runAllPhases();
                break;

            case 7:  // Comprehensive Benchmark
                runComprehensiveAnalysis();
                break;

            case 8:
                cout << "Kết thúc chương trình!\n";
                break;

            default:
                cout << "Lựa chọn không hợp lệ!\n";
                break;
        } 

    } while(choice != 8);

    return 0;
}