#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
void loadConfig(const string &configFile, string &regionFile, int &maxSteps, int &refreshRate) {
    ifstream file(configFile);
    if (!file) {
        cout << "I couldnt open the config file, can you please try to give the name of the file again or try another name." << endl;
        exit(1);
    }
    getline(file, regionFile);
    file >> maxSteps >> refreshRate;
    file.close();
}
// this will read the config file
vector<vector<char>> loadRegion(const string &regionFile) {
    ifstream file(regionFile);
    if (!file) {
        cout << "Somthing went wrong I cannot open the region file" << endl;
        exit(1);
    }
    vector<vector<char>> map;
    string line;
    while (getline(file, line)) {
        vector<char> row(line.begin(), line.end());
        map.push_back(row);
    }
    file.close();
    return map;
}
// This reads the file that should be the region
void showRegion(const vector<vector<char>> &map) {
    cout << "Here is the beginner layout of the new city you made: " << endl;
    for (const auto &row : map) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}
// This func should print out the original region.
int main() {
    string configFile, regionFile;
    int maxSteps, refreshRate;
    cout << "hello, enter the name of the config file your trying to get: ";
    cin >> configFile;
    loadConfig(configFile, regionFile, maxSteps, refreshRate);
    vector<vector<char>> map = loadRegion(regionFile);
    showRegion(map);
    return 0;
}
