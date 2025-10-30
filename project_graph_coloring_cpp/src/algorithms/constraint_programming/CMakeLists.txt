add_library(cp_solver CP_Solver.cpp)
target_include_directories(cp_solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Ví dụ nếu dùng OR-Tools:
# target_link_libraries(cp_solver ortools)
