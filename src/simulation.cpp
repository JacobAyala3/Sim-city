#include "simulation.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void runSimulation(Region& region, int maxSteps, int refreshRate) {
    for (int step = 1; step <= maxSteps; ++step) {
        //wDevo rules 
        cout << "Time Step: " << step << endl;
        region.displayRegion();
        this_thread::sleep_for(chrono::milliseconds(refreshRate * 1000));
    }
}
