add_library(dsatur DSATUR.cpp)
target_include_directories(dsatur PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dsatur graph)
