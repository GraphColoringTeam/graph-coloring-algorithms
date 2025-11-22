# Tạo một INTERFACE library để gom các solver
add_library(algorithms INTERFACE)

# Thêm các thư mục con
add_subdirectory(backtracking)
add_subdirectory(dsatur)
add_subdirectory(constraint_programming)
add_subdirectory(simulated_annealing)
add_subdirectory(rlf)

# Liên kết các solver vào INTERFACE library
target_link_libraries(algorithms INTERFACE
    backtracking_solver
    dsatur_solver
    cp_solver
    simulated_annealing_solver
    rlf_solver
)
