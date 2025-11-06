cmake_minimum_required(VERSION 3.10) 
project(GraphColoringProject CXX)
add_subdirectory(src/graph_representation) 
#add_subdirectory(src/algorithms) 
add_subdirectory(src/utils) 

add_executable(main src/main.cpp)

# target_include_directories(utils ...) # KHÔNG NÊN CÓ LỆNH target_include_directories Ở ĐÂY
target_include_directories(utils PUBLIC 
    "${CMAKE_SOURCE_DIR}/src"
)
target_link_libraries(main
    graph 
    # backtracking 
    # dsatur
    # simulated_annealing 
    # cp_solver
    # max_cut
    # bronkerbosch
    utils
)