#include "region.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

bool Region::loadRegion(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<Cell> row;
        string cellValue;
        while (getline(ss, cellValue, ',')) {
            Cell c;
            c.type = cellValue.empty() ? '-' : cellValue[0];
            row.push_back(c);
        }
        grid.push_back(row);
    }

    return true;
}

void Region::displayRegion() const {
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            if ((cell.type == 'R' || cell.type == 'C' || cell.type == 'I') && cell.population > 0) {
                cout << cell.population;
            } else {
                cout << cell.type;
            }
        }
        cout << endl;
    }
}

int Region::getWidth() const {
    return grid.empty() ? 0 : grid[0].size();
}

int Region::getHeight() const {
    return grid.size();
}