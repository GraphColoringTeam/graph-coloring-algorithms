#!/bin/bash

BASE="project_graph_coloring_cpp"

# Root CMakeLists
cat << 'EOF' > $BASE/CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(GraphColoring)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

enable_testing()
EOF

# CMake cho src
mkdir -p $BASE/src
cat << 'EOF' > $BASE/src/CMakeLists.txt
add_subdirectory(graph_representation)
add_subdirectory(algorithms)
add_subdirectory(utils)

add_executable(main main.cpp)
target_link_libraries(main
    graph
    backtracking
    dsatur
    simulated_annealing
    cp_solver
    max_cut
    bronkerbosch
    utils
)
EOF

# Graph Representation
mkdir -p $BASE/src/graph_representation
cat << 'EOF' > $BASE/src/graph_representation/CMakeLists.txt
add_library(graph Graph.cpp)
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
EOF

# Algorithms
mkdir -p $BASE/src/algorithms
cat << 'EOF' > $BASE/src/algorithms/CMakeLists.txt
add_subdirectory(backtracking)
add_subdirectory(greedy)
add_subdirectory(simulated_annealing)
add_subdirectory(constraint_programming)
add_subdirectory(max_cut_approx)
add_subdirectory(perfect_graphs)
EOF

# Backtracking
mkdir -p $BASE/src/algorithms/backtracking
cat << 'EOF' > $BASE/src/algorithms/backtracking/CMakeLists.txt
add_library(backtracking Backtracking.cpp)
target_include_directories(backtracking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(backtracking graph)
EOF

# Greedy / DSATUR
mkdir -p $BASE/src/algorithms/greedy
cat << 'EOF' > $BASE/src/algorithms/greedy/CMakeLists.txt
add_library(dsatur DSATUR.cpp)
target_include_directories(dsatur PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dsatur graph)
EOF

# Simulated Annealing
mkdir -p $BASE/src/algorithms/simulated_annealing
cat << 'EOF' > $BASE/src/algorithms/simulated_annealing/CMakeLists.txt
add_library(simulated_annealing SimulatedAnnealing.cpp)
target_include_directories(simulated_annealing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(simulated_annealing graph)
EOF

# Constraint Programming
mkdir -p $BASE/src/algorithms/constraint_programming
cat << 'EOF' > $BASE/src/algorithms/constraint_programming/CMakeLists.txt
add_library(cp_solver CP_Solver.cpp)
target_include_directories(cp_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Ví dụ nếu dùng OR-Tools:
# target_link_libraries(cp_solver ortools)
EOF

# Max Cut Approx
mkdir -p $BASE/src/algorithms/max_cut_approx
cat << 'EOF' > $BASE/src/algorithms/max_cut_approx/CMakeLists.txt
add_library(max_cut MaxCutApprox.cpp)
target_include_directories(max_cut PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(max_cut graph)
EOF

# Bron Kerbosch
mkdir -p $BASE/src/algorithms/perfect_graphs
cat << 'EOF' > $BASE/src/algorithms/perfect_graphs/CMakeLists.txt
add_library(bronkerbosch BronKerbosch.cpp)
target_include_directories(bronkerbosch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bronkerbosch graph)
EOF

# Utils
mkdir -p $BASE/src/utils
cat << 'EOF' > $BASE/src/utils/CMakeLists.txt
add_library(utils GraphGenerator.cpp GraphLoader.cpp)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
EOF

echo "✅ Generated multi-module CMakeLists!"