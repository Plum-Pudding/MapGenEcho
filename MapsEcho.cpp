// MapsEcho.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <array>
#include <iostream>

#include "SplitMix64.h"
#include "gen.h"
#include "util.h"
#include "map.h"

#define DEFAULT_MAPSIZE_X 128
#define DEFAULT_MAPSIZE_Y 128

#define DEFAULT_RESOLUTION 100 //size of each map point in metres


int main() {
    Map testMap(DEFAULT_MAPSIZE_X, DEFAULT_MAPSIZE_Y);
    Gen::fillBlanketGridMap(testMap, 0);

    //Gen::generateVoronoiCells(testMap, 2);
    
    Gen::drawOrthogonalLine2(testMap, 5, 0.12, 0.20, 0.82, 0.76);

    Gen::saveGridMapHeight(testMap);
    Gen::saveGridMapPlateNum(testMap);

    return 0;
}

