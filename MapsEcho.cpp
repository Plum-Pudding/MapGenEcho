// MapsEcho.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "gen.h"
#include "map.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <chrono>

#define DEFAULT_MAPSIZE_X 128
#define DEFAULT_MAPSIZE_Y 128

#define DEFAULT_RESOLUTION 100 //size of each map point in metres


int main() {
    Map testMap(DEFAULT_MAPSIZE_X, DEFAULT_MAPSIZE_Y);
    
    Gen::saveGridMap(testMap);


    return 0;
}

