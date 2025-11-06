// src/utils/GraphLoader.cpp 

#include "GraphLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;
unique_ptr<Graph> loadGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Loi: Khong the mo file " << filename << ". Kiem tra duong dan file." << endl;
return unique_ptr<Graph>(nullptr);    }

    int V = 0, E = 0; 
   string line;

    if (getline(file, line)) {
        stringstream ss(line);
        if (!(ss >> V >> E)) V = 0; 
    }

    if (V <= 0) {
        cerr << "Loi: So luong dinh khong hop le hoac khong doc duoc." << endl;
return unique_ptr<Graph>(nullptr);    }

    auto g =make_unique<Graph>(V);

    int u, v;
    while (getline(file, line)) {
        stringstream ss(line);
        if (ss >> u >> v) {
            try {
                g->addEdge(u, v);
            } catch (const out_of_range& e) {
                cerr << "Canh (" << u << ", " << v << ") bi bo qua: " << e.what() << endl;
            }
        }
    }

    file.close();
    return g;
}