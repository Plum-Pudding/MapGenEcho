#include "util.h"
#include "map.h"
#include <iostream>
#include <chrono>
#include <fstream>

void fillBlanketGridMap(Map& map, int target) {
    auto fillGridMap_Start = std::chrono::high_resolution_clock::now();
    for (int x = 0; x < map.getSizeX(); x++) {
        for (int y = 0; y < map.getSizeY(); y++) {
            map.heightAt(x, y) = target;
        }
    }
    auto fillGridMap_End = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fillGridMap_duration = fillGridMap_End - fillGridMap_Start;
    std::cout << "Blanket fill grid execution time: " << fillGridMap_duration << "\n";
}

void printGridMap(const Map& map) {
    auto printGridMap_Start = std::chrono::high_resolution_clock::now();

    for (int x = 0; x < map.getSizeX(); x++) {
        for (int y = 0; y < map.getSizeY(); y++) {
            std::cout << map.heightAt(x, y);
        }
        std::cout << "\n";
    }

    auto printGridMap_End = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> printGridMap_duration = printGridMap_End - printGridMap_Start;
    std::cout << "Print grid execution time: " << printGridMap_duration << "\n";

}

//Uncompressed write as a .csv
void saveGridMap(const Map& map) {
    auto saveGridMap_start = std::chrono::high_resolution_clock::now();
    std::ofstream resultFile;
    resultFile.open("map.csv");

    for (int x = 0; x < map.getSizeX(); x++) {
        for (int y = 0; y < map.getSizeY(); y++) {
            resultFile << map.heightAt(x, y) << ',';
        }
        resultFile << "\n";
    }
    resultFile << ";";


    auto saveGridMap_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> saveGridMap_duration = saveGridMap_end - saveGridMap_start;
    std::cout << "Wrote map to file in " << saveGridMap_duration << "\n";
}