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
