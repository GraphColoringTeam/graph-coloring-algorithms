#include "DSATURColoring.h"
#include <iostream>
using namespace std;

int main() {
    vector<pair<int, int>> edges = {
        {0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 4}
    };
    DSATURColoring coloring(5, edges);

    vector<int> colors;
    int num_colors = coloring.colorGraph(colors);

    cout << "Tổng số màu: " << num_colors << endl;
    for (int i = 0; i < colors.size(); ++i)
        cout << "Đỉnh " << i << " -> Màu " << colors[i] << endl;
}
