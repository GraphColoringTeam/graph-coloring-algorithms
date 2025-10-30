add_library(bronkerbosch BronKerbosch.cpp)
target_include_directories(bronkerbosch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bronkerbosch graph)
