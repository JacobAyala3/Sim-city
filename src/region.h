#ifndef REGION_H
#define REGION_H
#include <vector>
#include <string>
#include "cell.h"
using namespace std;
class Region {
public:
    bool loadRegion(const string& filename);
    void displayRegion() const;
    int getWidth() const;
    int getHeight() const;
    //getters
    const vector<vector<Cell>>& getGrid() const { return grid; }
    vector<vector<Cell>>& getMutableGrid() { return grid; }

private:
    vector<vector<Cell>> grid;
};
#endif