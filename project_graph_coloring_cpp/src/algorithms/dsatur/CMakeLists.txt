add_library(dsatur_solver STATIC
    DSATURSolver.cpp
)

target_include_directories(dsatur_solver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../graph_representation
)

target_link_libraries(dsatur_solver PRIVATE
    graph
)