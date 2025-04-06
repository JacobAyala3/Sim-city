#include <iostream>
#include <fstream>
#include "region.h"

using namespace std;

int main() {
    string configFile;
    cout << "Enter config filename: ";
    cin >> configFile;

    ifstream file(configFile);
    if (!file) {
        cerr << "Error: could not open config file." << endl;
        return 1;
    }

    string regionFile;
    int maxTimeSteps, refreshRate;

    getline(file, regionFile);
    file >> maxTimeSteps >> refreshRate;

    Region region;
    if (!region.loadRegion(regionFile)) {
        cerr << "Error loading region file." << endl;
        return 1;
    }

    cout << "Initial Region State (Time Step 0):" << endl;
    region.displayRegion();

    return 0;
}