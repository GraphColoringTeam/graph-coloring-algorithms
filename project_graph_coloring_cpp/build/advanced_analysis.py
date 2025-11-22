import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import os
from tabulate import tabulate

print('=== GIAI ĐOẠN 2: PHÂN TÍCH CHUYÊN SÂU ===')
print('=' * 60)

# Đọc kết quả
df = pd.read_csv('comprehensive_results.csv')

# Phân loại graph theo density
def classify_density(vertices, edges):
    if vertices == 0:
        return 'Empty'
    max_possible_edges = vertices * (vertices - 1) / 2
    density = edges / max_possible_edges
    if density < 0.1: return 'Rất thưa (<0.1)'
    elif density < 0.3: return 'Thưa (0.1-0.3)'
    elif density < 0.6: return 'Trung bình (0.3-0.6)'
    else: return 'Dày (≥0.6)'

df['Density_Class'] = df.apply(lambda x: classify_density(x['Vertices'], x['Edges']), axis=1)

# Phân loại kích thước chi tiết
def detailed_size_class(vertices):
    if vertices <= 10: return 'Rất nhỏ (1-10)'
    elif vertices <= 50: return 'Nhỏ (11-50)'
    elif vertices <= 200: return 'Trung bình (51-200)'
    else: return 'Lớn (>200)'

df['Size_Class'] = df['Vertices'].apply(detailed_size_class)

# ==================== BẢNG TỔNG HỢP CHÍNH ====================

print('\n📈 BẢNG TỔNG HỢP HIỆU SUẤT THUẬT TOÁN')
print('=' * 90)

summary_data = []
for algo in df['Algorithm'].unique():
    algo_data = df[df['Algorithm'] == algo]
    
    avg_time = algo_data['Time(ms)'].mean()
    avg_colors = algo_data['Colors'].mean()
    success_rate = (algo_data['Valid'] == 'Yes').mean() * 100
    avg_memory = algo_data['MemoryUsageKB'].mean()
    
    # Tính hiệu suất tương đối so với DSATUR (baseline)
    dsatur_colors = df[df['Algorithm'] == 'DSATUR']['Colors'].mean()
    dsatur_time = df[df['Algorithm'] == 'DSATUR']['Time(ms)'].mean()
    
    color_ratio = (avg_colors / dsatur_colors - 1) * 100  # % so với DSATUR
    time_ratio = (avg_time / dsatur_time) if dsatur_time > 0 else float('inf')
    
    # Đánh giá chất lượng
    if color_ratio <= 0:
        quality_rating = "🎯 TỐT NHẤT"
    elif color_ratio <= 10:
        quality_rating = "✅ TỐT"
    elif color_ratio <= 20:
        quality_rating = "⚠️ TRUNG BÌNH"
    else:
        quality_rating = "❌ KÉM"
    
    # Đánh giá tốc độ
    if time_ratio <= 1:
        speed_rating = "🚀 NHANH NHẤT"
    elif time_ratio <= 5:
        speed_rating = "✅ NHANH"
    elif time_ratio <= 20:
        speed_rating = "⚠️ CHẬM"
    else:
        speed_rating = "🐌 RẤT CHẬM"
    
    summary_data.append([
        algo,
        f"{avg_colors:.1f}",
        f"{color_ratio:+.1f}%",
        quality_rating,
        f"{avg_time:.1f} ms",
        f"{time_ratio:.1f}x",
        speed_rating,
        f"{success_rate:.0f}%",
        f"{avg_memory:.0f} KB"
    ])

# Sắp xếp theo chất lượng (số màu)
summary_data.sort(key=lambda x: float(x[1]))

headers = [
    "Thuật toán", 
    "Số màu TB", 
    "So DSATUR", 
    "Đánh giá CL", 
    "Thời gian TB", 
    "So DSATUR", 
    "Đánh giá TC", 
    "Tỷ lệ thành công", 
    "Bộ nhớ TB"
]

print(tabulate(summary_data, headers=headers, tablefmt="grid", stralign="center"))

# ==================== BẢNG THEO LOẠI GRAPH ====================

print('\n\n🎯 BẢNG HIỆU SUẤT THEO ĐỘ DÀY ĐỒ THỊ')
print('=' * 80)

density_table = []
for density in ['Rất thưa (<0.1)', 'Thưa (0.1-0.3)', 'Trung bình (0.3-0.6)', 'Dày (≥0.6)']:
    density_data = df[df['Density_Class'] == density]
    if density_data.empty:
        continue
        
    row = [density]
    for algo in ['DSATUR', 'RLF', 'SimulatedAnnealing']:
        algo_data = density_data[density_data['Algorithm'] == algo]
        if not algo_data.empty:
            avg_colors = algo_data['Colors'].mean()
            avg_time = algo_data['Time(ms)'].mean()
            row.append(f"{avg_colors:.1f} màu\n{avg_time:.1f} ms")
        else:
            row.append("N/A")
    density_table.append(row)

density_headers = ["Độ dày", "DSATUR", "RLF", "Simulated Annealing"]
print(tabulate(density_table, headers=density_headers, tablefmt="grid"))

# ==================== BẢNG THEO KÍCH THƯỚC ====================

print('\n\n📊 BẢNG HIỆU SUẤT THEO KÍCH THƯỚC ĐỒ THỊ')
print('=' * 80)

size_table = []
for size_class in ['Rất nhỏ (1-10)', 'Nhỏ (11-50)', 'Trung bình (51-200)', 'Lớn (>200)']:
    size_data = df[df['Size_Class'] == size_class]
    if size_data.empty:
        continue
        
    row = [size_class]
    for algo in ['Backtracking', 'ConstraintProgramming', 'DSATUR', 'RLF', 'SimulatedAnnealing']:
        algo_data = size_data[size_data['Algorithm'] == algo]
        if not algo_data.empty:
            avg_colors = algo_data['Colors'].mean()
            success_rate = (algo_data['Valid'] == 'Yes').mean() * 100
            if algo in ['Backtracking', 'ConstraintProgramming'] and size_class != 'Rất nhỏ (1-10)':
                row.append("⏸️ KHÔNG CHẠY")
            else:
                row.append(f"{avg_colors:.1f} màu\n{success_rate:.0f}% thành công")
        else:
            row.append("N/A")
    size_table.append(row)

size_headers = ["Kích thước", "Backtracking", "Constraint Prog", "DSATUR", "RLF", "Simulated Annealing"]
print(tabulate(size_table, headers=size_headers, tablefmt="grid"))

# ==================== KẾT LUẬN & ĐỀ XUẤT ====================

print('\n\n💡 KẾT LUẬN & ĐỀ XUẤT')
print('=' * 60)

recommendations = [
    ["🎯 CHẤT LƯỢNG TỐT NHẤT", "Backtracking, Constraint Programming", "Chỉ cho graph rất nhỏ (≤10 đỉnh)"],
    ["⚡ CÂN BẰNG TỐT NHẤT", "DSATUR", "Nhanh, chất lượng tốt, ổn định mọi graph size"],
    ["🔄 ĐÁNG TIN CẬY", "RLF", "Chất lượng gần DSATUR, hơi chậm hơn"],
    ["🔬 THỬ NGHIỆM", "Simulated Annealing", "Chất lượng tốt nhưng rất chậm, cần tối ưu"],
    ["📈 GRAPH LỚN", "DSATUR, RLF", "Scalability tốt, memory usage thấp"],
    ["🎨 GRAPH NHỎ", "Tất cả thuật toán", "Có thể dùng Backtracking/CP cho kết quả tối ưu"]
]

rec_headers = ["Mục tiêu", "Thuật toán đề xuất", "Ghi chú"]
print(tabulate(recommendations, headers=rec_headers, tablefmt="grid"))

print(f'\n📋 TỔNG SỐ TEST: {len(df)}')
print(f'📁 SỐ GRAPH: {df["Graph"].nunique()}')
print(f'🎯 THUẬT TOÁN: {", ".join(df["Algorithm"].unique())}')

print('\n✅ Phân tích hoàn thành!')