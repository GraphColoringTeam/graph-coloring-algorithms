
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import numpy as np

print('=== ENHANCED GRAPH COLORING BENCHMARK ANALYSIS ===')
print('=== GIAI ĐOẠN 1: ĐẦY ĐỦ METRICS ===')

df = pd.read_csv('comprehensive_results.csv')
print(f'Total records: {len(df)}')
print(f'Algorithms: {", ".join(df["Algorithm"].unique())}')
print(f'Graph files: {len(df["Graph"].unique())}')

df['Graph_Name'] = df['Graph'].apply(lambda x: os.path.basename(x))

def classify_graph_size(vertices):
    if vertices <= 15: return 'Small (≤15)'
    elif vertices <= 100: return 'Medium (16-100)' 
    else: return 'Large (>100)'

df['Graph_Size'] = df['Vertices'].apply(classify_graph_size)

print('\nGraph size distribution (CHUẨN HÓA):')
print(df['Graph_Size'].value_counts())

# ==================== METRICS TỔNG QUAN ====================
success_rate = df.groupby('Algorithm')['Valid'].apply(lambda x: (x == 'Yes').mean() * 100)
print('\n1. SUCCESS RATE (%):')
for algo, rate in success_rate.items():
    print(f'   {algo:<20}: {rate:.1f}%')

# 2. Memory Usage Analysis
if 'MemoryUsageKB' in df.columns:
    memory_usage = df.groupby('Algorithm')['MemoryUsageKB'].agg(['mean', 'max'])
    print('\n2. MEMORY USAGE (KB):')
    for algo in memory_usage.index:
        avg_mem = memory_usage.loc[algo, 'mean']
        max_mem = memory_usage.loc[algo, 'max']
        print(f'   {algo:<20}: Avg={avg_mem:.0f}KB, Max={max_mem:.0f}KB')

# 3. Time Statistics
time_stats = df.groupby('Algorithm')['Time(ms)'].agg(['mean', 'std', 'min', 'max'])
print('\n3. RUNTIME STATISTICS (ms):')
for algo in time_stats.index:
    avg_time = time_stats.loc[algo, 'mean']
    std_time = time_stats.loc[algo, 'std'] 
    max_time = time_stats.loc[algo, 'max']
    print(f'   {algo:<20}: Avg={avg_time:.1f}±{std_time:.1f}, Max={max_time:.1f}')

# 4. Colors Used
color_stats = df.groupby('Algorithm')['Colors'].agg(['mean', 'std', 'min', 'max'])
print('\n4. COLORS USED:')
for algo in color_stats.index:
    avg_colors = color_stats.loc[algo, 'mean']
    std_colors = color_stats.loc[algo, 'std']
    min_colors = color_stats.loc[algo, 'min']
    print(f'   {algo:<20}: Avg={avg_colors:.1f}±{std_colors:.1f}, Min={min_colors}')

# ==================== VISUALIZATION MỚI ====================

# 1. Memory vs Time Scatter Plot
if 'MemoryUsageKB' in df.columns:
    plt.figure(figsize=(12, 8))
    for algo in df['Algorithm'].unique():
        algo_data = df[df['Algorithm'] == algo]
        plt.scatter(algo_data['Time(ms)'], algo_data['MemoryUsageKB'], 
                   label=algo, alpha=0.7, s=60)
    plt.xlabel('Time (ms)')
    plt.ylabel('Memory Usage (KB)')
    plt.title('Time vs Memory Usage by Algorithm\n(Đánh giá trade-off thời gian và bộ nhớ)')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('time_memory_tradeoff.png', dpi=300, bbox_inches='tight')
    print('\nSaved: time_memory_tradeoff.png')

# 2. Success Rate Bar Chart
plt.figure(figsize=(10, 6))
success_data = df.groupby('Algorithm')['Valid'].apply(lambda x: (x == 'Yes').mean() * 100)
success_data.plot(kind='bar', color='lightgreen')
plt.title('Success Rate by Algorithm\n(Tỷ lệ tìm được coloring hợp lệ)')
plt.ylabel('Success Rate (%)')
plt.xticks(rotation=45)
plt.ylim(0, 105)
plt.tight_layout()
plt.savefig('success_rate.png', dpi=300, bbox_inches='tight')
print('Saved: success_rate.png')

# 3. Memory Usage Comparison
if 'MemoryUsageKB' in df.columns:
    plt.figure(figsize=(12, 6))
    sns.boxplot(data=df, x='Algorithm', y='MemoryUsageKB')
    plt.title('Memory Usage Distribution by Algorithm\n(Phân phối sử dụng bộ nhớ)')
    plt.xticks(rotation=45)
    plt.ylabel('Memory Usage (KB)')
    plt.tight_layout()
    plt.savefig('memory_usage.png', dpi=300, bbox_inches='tight')
    print('Saved: memory_usage.png')

# 4. Performance Profile (Cumulative Distribution)
plt.figure(figsize=(12, 8))
for algo in df['Algorithm'].unique():
    algo_data = df[df['Algorithm'] == algo]
    if len(algo_data) > 0:
        sorted_times = np.sort(algo_data['Time(ms)'])
        y_vals = np.arange(1, len(sorted_times) + 1) / len(sorted_times)
        plt.plot(sorted_times, y_vals, label=algo, linewidth=2)

plt.xlabel('Time (ms)')
plt.ylabel('Cumulative Probability')
plt.title('Performance Profile: Time Distribution\n(Phân phối thời gian thực thi)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('performance_profile.png', dpi=300, bbox_inches='tight')
print('Saved: performance_profile.png')

print('\n✅ Visualization completed! Check the generated PNG files for detailed analysis.')
