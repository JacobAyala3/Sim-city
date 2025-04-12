#include "simulation.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
using namespace std;

// Helper to check if powerline is adjacent
bool isAdjacentToPowerline(const Region& region, int x, int y) {
    auto& grid = region.getGrid();
    int h = region.getHeight(), w = region.getWidth();
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < h && ny >= 0 && ny < w) {
                char t = grid[nx][ny].type;
                if (t == 'T' || t == '#' || t == 'P') return true;
            }
        }
    }
    return false;
}

// Count neighbors with population >= threshold
int countAdjacentPop(const Region& region, int x, int y, int threshold) {
    auto& grid = region.getGrid();
    int h = region.getHeight(), w = region.getWidth();
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < h && ny >= 0 && ny < w) {
                if (grid[nx][ny].population >= threshold)
                    ++count;
            }
        }
    }
    return count;
}

// Pollution spreads from industrial zones
void applyPollutionSpread(Region& region) {
    auto& grid = region.getMutableGrid();
    int h = region.getHeight(), w = region.getWidth();
    vector<vector<int>> newPollution(h, vector<int>(w, 0));

    for (int x = 0; x < h; ++x) {
        for (int y = 0; y < w; ++y) {
            if (grid[x][y].type == 'I') {
                int srcPol = grid[x][y].population;
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        int nx = x + dx, ny = y + dy;
                        if (nx >= 0 && nx < h && ny >= 0 && ny < w) {
                            int dist = max(abs(dx), abs(dy));
                            int polToAdd = max(0, srcPol - dist);
                            newPollution[nx][ny] += polToAdd;
                        }
                    }
                }
            }
        }
    }

    // Apply new pollution
    for (int x = 0; x < h; ++x)
        for (int y = 0; y < w; ++y)
            grid[x][y].pollution = newPollution[x][y];
}

// Calculate total available workers (residential)
int getWorkers(const Region& region) {
    int total = 0;
    for (auto& row : region.getGrid())
        for (auto& c : row)
            if (c.type == 'R') total += c.population;
    return total;
}

// Calculate available goods (industrial)
int getGoods(const Region& region) {
    int total = 0;
    for (auto& row : region.getGrid())
        for (auto& c : row)
            if (c.type == 'I') total += c.population;
    return total;
}

// Simulate one step of growth
bool simulateGrowth(Region& region, int& workers, int& goods) {
    auto oldGrid = region.getGrid();
    auto& grid = region.getMutableGrid();
    int h = region.getHeight(), w = region.getWidth();
    bool changed = false;

    for (int x = 0; x < h; ++x) {
        for (int y = 0; y < w; ++y) {
            Cell& c = grid[x][y];
            if (c.type == 'R') {
                int p = c.population;
                if (p == 0 && isAdjacentToPowerline(region, x, y)) c.population++;
                else if (p == 0 && countAdjacentPop(region, x, y, 1) >= 1) c.population++;
                else if (p == 1 && countAdjacentPop(region, x, y, 1) >= 2) c.population++;
                else if (p == 2 && countAdjacentPop(region, x, y, 2) >= 4) c.population++;
                else if (p == 3 && countAdjacentPop(region, x, y, 3) >= 6) c.population++;
                else if (p == 4 && countAdjacentPop(region, x, y, 4) >= 8) c.population++;
                if (c.population != oldGrid[x][y].population) changed = true;
            }

            if (c.type == 'I') {
                int p = c.population;
                if ((p == 0 && isAdjacentToPowerline(region, x, y) && workers >= 2) ||
                    (p == 0 && countAdjacentPop(region, x, y, 1) >= 1 && workers >= 2) ||
                    (p == 1 && countAdjacentPop(region, x, y, 1) >= 2 && workers >= 2) ||
                    (p == 2 && countAdjacentPop(region, x, y, 2) >= 4 && workers >= 2)) {
                    c.population++;
                    workers -= 2;
                    changed = true;
                }
            }

            if (c.type == 'C') {
                int p = c.population;
                if ((p == 0 && isAdjacentToPowerline(region, x, y) && workers >= 1 && goods >= 1) ||
                    (p == 0 && countAdjacentPop(region, x, y, 1) >= 1 && workers >= 1 && goods >= 1) ||
                    (p == 1 && countAdjacentPop(region, x, y, 1) >= 2 && workers >= 1 && goods >= 1)) {
                    c.population++;
                    workers--;
                    goods--;
                    changed = true;
                }
            }
        }
    }

    applyPollutionSpread(region);
    return changed;
}

// Main simulation runner
void runSimulation(Region& region, int maxSteps, int refreshRate) {
    int workers = getWorkers(region);
    int goods = getGoods(region);

    for (int step = 1; step <= maxSteps; ++step) {
        bool changed = simulateGrowth(region, workers, goods);
        cout << "Time Step: " << step << "\nAvailable Workers: " << workers << ", Goods: " << goods << "\n";

        if (step % refreshRate == 0)
            region.displayRegion();

        if (!changed) {
            cout << "No change detected. Halting simulation.\n";
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(refreshRate * 1000));
    }
}

