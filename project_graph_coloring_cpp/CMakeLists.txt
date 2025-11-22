cmake_minimum_required(VERSION 3.10)
project(GraphColoring)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_subdirectory(src/graph_representation)
add_subdirectory(src/utils)
add_subdirectory(src/algorithms)

add_executable(main src/main.cpp)

target_link_libraries(main
    graph
    utils
    backtracking_solver
    dsatur_solver
    simulated_annealing_solver
    rlf_solver
    cp_solver
)
