add_library(rlf_solver STATIC
    RLFSolver.cpp
)

target_include_directories(rlf_solver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../graph_representation
)

target_link_libraries(rlf_solver PRIVATE
    graph
)