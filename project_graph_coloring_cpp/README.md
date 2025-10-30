# 🎨 Graph Coloring Algorithms in C++

## ✅ Giới thiệu
Dự án này triển khai và so sánh nhiều thuật toán giải bài toán tô màu đồ thị (Graph Coloring), bao gồm:
- Backtracking
- Greedy (DSATUR)
- Simulated Annealing
- Constraint Programming (OR-Tools)
- Max-Cut/RLF heuristic
- Perfect Graphs (Bron-Kerbosch)

## ✅ Mục tiêu
- Tìm số lượng màu nhỏ nhất để tô các đỉnh sao cho không có hai đỉnh kề nhau trùng màu.
- So sánh thời gian chạy & số màu cần dùng giữa các thuật toán.

## ✅ Cấu trúc thư mục
\`\`\`
src/
 ├── graph_representation/
 ├── algorithms/
 ├── utils/
data/
results/
docs/
\`\`\`

## ✅ Hướng dẫn build
\`\`\`
mkdir build && cd build
cmake ..
make -j4
./main
\`\`\`

## ✅ Input
- Tải bộ benchmark DIMACS (.col) vào \`data/benchmark_graphs/\`
- Hoặc sử dụng GraphGenerator để tạo đồ thị ngẫu nhiên

## ✅ Output
- Lưu kết quả (màu sử dụng, thời gian chạy) vào \`results/*.csv\`

## ✅ Tác giả
- Nhóm nghiên cứu Graph Coloring – C++

