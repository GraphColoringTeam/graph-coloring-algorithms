add_library(max_cut MaxCutApprox.cpp)
target_include_directories(max_cut PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(max_cut graph)
