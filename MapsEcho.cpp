// MapsEcho.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <array>
#include <iostream>

#include "SplitMix64.h"
#include "gen.h"
#include "util.h"
#include "map.h"

#define DEFAULT_MAPSIZE_X 32
#define DEFAULT_MAPSIZE_Y 32

#define DEFAULT_RESOLUTION 100 //size of each map point in metres


int main() {
    Map testMap(DEFAULT_MAPSIZE_X, DEFAULT_MAPSIZE_Y);
    Gen::fillBlanketGridMap(testMap, 0);
    /*
    std::array<uint64_t, 100> testArr;
    Splitmix64 random;
    random.seed(0);

    for (int i = 0; i < testArr.size(); i++) {
        testArr[i] = (random.next_int() & 0b111);
        std::cout << testArr[i] << "\n";
    }
    */

    //Gen::mapSnake8Directions(testMap, 16, 16, 16, 1, 0);

    Gen::mapSnake8DirectionsAdditive(testMap, 16, 16, 1024, 1, 0);

    Gen::saveGridMap(testMap);

    return 0;
}

