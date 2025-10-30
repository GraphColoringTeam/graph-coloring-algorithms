add_library(backtracking Backtracking.cpp)
target_include_directories(backtracking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(backtracking graph)
