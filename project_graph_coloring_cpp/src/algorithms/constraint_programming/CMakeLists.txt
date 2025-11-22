add_library(cp_solver STATIC
    CPSolver.cpp
)

target_include_directories(cp_solver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../graph_representation # Để CPSolver có thể include Graph.h
    ${ortools_INCLUDE_DIRS} # Thêm include directories của OR-Tools
)

target_link_libraries(cp_solver PRIVATE
    graph
    ${ortools_LIBRARIES} # Liên kết với thư viện OR-Tools
)