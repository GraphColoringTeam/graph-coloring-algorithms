add_library(simulated_annealing_solver STATIC
    SimulatedAnnealingSolver.cpp
)

target_include_directories(simulated_annealing_solver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../graph_representation
)

target_link_libraries(simulated_annealing_solver PRIVATE
    graph
)