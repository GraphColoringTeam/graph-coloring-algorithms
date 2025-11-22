# src/utils/CMakeLists.txt
add_library(utils
    TestDataGenerator.cpp
    TestRunner.cpp
    Benchmark.cpp 
    ComprehensiveAnalyzer.cpp
)

target_include_directories(utils PUBLIC
    ${CMAKE_SOURCE_DIR}/src/graph_representation
    ${CMAKE_SOURCE_DIR}/src/algorithms 
)

# Thêm dependencies đến các algorithms nếu cần
target_link_libraries(utils graph)