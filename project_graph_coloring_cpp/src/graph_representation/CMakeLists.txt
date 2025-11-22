add_library(graph STATIC
    Graph.cpp
    GraphGenerator.cpp
    GraphLoader.cpp
)

target_include_directories(graph PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # Để các module khác có thể include Graph.h
)