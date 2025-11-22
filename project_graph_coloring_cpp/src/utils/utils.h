#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>

using namespace std;

// Hàm tiện ích in kết quả tô màu
inline void printColoring(const vector<int>& colors) {
    cout << "Kết quả tô màu:\n";
    for (size_t i = 0; i < colors.size(); ++i) {
        cout << "Đỉnh " << i << ": màu " << colors[i] << "\n";
    }
}

#endif // UTILS_H