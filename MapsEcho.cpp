// MapsEcho.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "util.h"
#include "map.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <chrono>

#define DEFAULT_MAPSIZE_X 100
#define DEFAULT_MAPSIZE_Y 100

#define DEFAULT_RESOLUTION 100 //size of each map point in metres


int main() {
    Map testMap(DEFAULT_MAPSIZE_X, DEFAULT_MAPSIZE_Y);
    
    testMap.heightAt(4, 3) = 2;
    std::cout << "Height at 4, 4 is " << testMap.heightAt(4, 4) << "\n";
    std::cout << "Height at 4, 3 is " << testMap.heightAt(4, 3) << "\n";

    printGridMap(testMap);
    saveGridMap(testMap);
    fillBlanketGridMap(testMap, 1);

    return 0;
}

