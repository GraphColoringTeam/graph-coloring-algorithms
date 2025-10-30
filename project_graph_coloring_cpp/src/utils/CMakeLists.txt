add_library(utils GraphGenerator.cpp GraphLoader.cpp)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
