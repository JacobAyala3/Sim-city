#include <iostream>
#include <fstream>
#include "region.h"
#include "simulation.h"

using namespace std;

int main() {
    string configFile;

    //gets the name of the configuration file
    cout << "Enter config filename: ";
    cin >> configFile;
    ifstream file(configFile);
    if (!file) {
        cerr << "Error loading the config file." << endl;
        return 1;
    }

    // Read configs
    string regionFile;
    int maxTimeSteps, refreshRate;

    getline(file, regionFile);
    file >> maxTimeSteps >> refreshRate;

    // Load region layout
    Region region;
    if (!region.loadRegion(regionFile)) {
        cerr << "Error loading the region file." << endl;
        return 1;
    }

    // Shows the starting state
    cout << "\nInitial region state this is time step 0:" << endl;
    region.displayRegion();
    runSimulation(region, maxTimeSteps, refreshRate);

    //lets the user analyze some of the region
    int x1, y1, x2, y2;

    cout << "\nEnter coordinates to analyze input them like this (x1 y1 x2 y2): ";
    cin >> x1 >> y1 >> x2 >> y2;

    while (x1 < 0 || y1 < 0 || x2 >= region.getHeight() || y2 >= region.getWidth()) {
        cout << "wrong cords try again: ";
        cin >> x1 >> y1 >> x2 >> y2;
    }
    int rPop = 0, iPop = 0, cPop = 0, totalPol = 0;
    const auto& grid = region.getGrid();

    // Loop
    for (int i = x1; i <= x2; ++i) {
        for (int j = y1; j <= y2; ++j) {
            const Cell& c = grid[i][j];
            if (c.type == 'R') rPop += c.population;
            if (c.type == 'I') iPop += c.population;
            if (c.type == 'C') cPop += c.population;
            totalPol += c.pollution;
        }
    }

    // Outputs
    cout << "\nRegion analysis is :\n";
    cout << "  residential Population: " << rPop << endl;
    cout << "  industrial Population:" << iPop << endl;
    cout << "  commercial Population: " << cPop << endl;
    cout << "  Total Pollution isssss : " << totalPol << endl;

    return 0;
}

