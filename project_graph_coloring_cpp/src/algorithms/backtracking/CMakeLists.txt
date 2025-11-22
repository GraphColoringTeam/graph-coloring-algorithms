add_library(backtracking_solver STATIC
    BacktrackingSolver.cpp
)

target_include_directories(backtracking_solver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../graph_representation
)

target_link_libraries(backtracking_solver PRIVATE
    graph
)
