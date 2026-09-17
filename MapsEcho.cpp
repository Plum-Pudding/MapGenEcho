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

    //Gen::generateVoronoiCells(testMap, 2);
    
    //Gen::drawOrthogonalLine2(testMap, 5, 0.1012, 0.2412, 0.7837, 0.9281);
    Gen::drawOrthogonalLine(testMap, 5, 0.9012, 0.1412, 0.1837, 0.9281);
    //Gen::drawOrthogonalLine2(testMap, 5, 0.1012, 0.9412, 0.9837, 0.1281);

    Gen::saveGridMapHeight(testMap);
    Gen::saveGridMapPlateNum(testMap);

    return 0;
}

