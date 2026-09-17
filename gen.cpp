#define JC_VORONOI_IMPLEMENTATION

#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <array>

#include "gen.h"
#include "map.h"
#include "SplitMix64.h"
#include "jc_voronoi.h"

#include "raylib-cpp\raylib.hpp"

#define TEMP_NUM 6

namespace Gen {
    void fillBlanketGridMap(Map& map, int16_t target) {
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

    void printGridMapHeight(const Map& map) {
        auto printGridMap_Start = std::chrono::high_resolution_clock::now();
        
        for (int y = 0; y < map.getSizeY(); y++) {
            for (int x = 0; x < map.getSizeX(); x++) {
                std::cout << map.heightAt(x, y);
            }
            std::cout << "\n";
        }

        auto printGridMap_End = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> printGridMap_duration = printGridMap_End - printGridMap_Start;
        std::cout << "Print grid execution time: " << printGridMap_duration << "\n";

    }

    void printGridMapPlateNum(const Map& map) {
        auto printGridMap_Start = std::chrono::high_resolution_clock::now();
       
        for (int y = 0; y < map.getSizeY(); y++) {
            for (int x = 0; x < map.getSizeX(); x++) {
                std::cout << map.plateNumAt(x, y) << " ";
            }
            std::cout << "\n";
        }
    
        auto printGridMap_End = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> printGridMap_duration = printGridMap_End - printGridMap_Start;
        std::cout << "Print grid execution time: " << printGridMap_duration << "\n";

    }

    //Uncompressed write as a .csv
    void saveGridMapHeight(const Map& map) {
        auto saveGridMap_start = std::chrono::high_resolution_clock::now();
        std::ofstream resultFile;
        resultFile.open("map.csv");


        for (int y = 0; y < map.getSizeY(); y++) {
            for (int x = 0; x < map.getSizeX(); x++) {
                resultFile << map.heightAt(x, y) << ',';
            }
            resultFile << "\n";
        }
        resultFile << ";";


        auto saveGridMap_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> saveGridMap_duration = saveGridMap_end - saveGridMap_start;
        std::cout << "Wrote map to file in " << saveGridMap_duration << "\n";
    }

    void saveGridMapPlateNum(const Map& map) {
        auto saveGridMap_start = std::chrono::high_resolution_clock::now();
        std::ofstream resultFile;
        resultFile.open("mapNum.csv");

        
        for (int y = 0; y < map.getSizeY(); y++) {
            for (int x = 0; x < map.getSizeX(); x++) {
                resultFile << map.plateNumAt(x, y) << ',';
            }
            resultFile << "\n";
        }
        
        resultFile << ";";

        auto saveGridMap_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> saveGridMap_duration = saveGridMap_end - saveGridMap_start;
        std::cout << "Wrote map to file in " << saveGridMap_duration << "\n";
    }

    void generateMap(Map& map) {
        /*
        * Method A: Connecting lines?
        * Steps:
        * - Generate random points on the map --use a working layer? Or just rawdog it on the tectonics layer?
        * - Draw lines connecting the points to form basic tecplate shapes-- how to avoid intersecting lines?
        * - roughen up the lines with noise
        * - Floodfill the resultant shapes with a random direction
        */

        /*
        * Method B: Points as centres of tecplates
        * Steps:
        * - Generate random points on the map as centres of plates
        * - somehow flood out the space and draw borders of plates
        *   - Expanding on this algorithm, could we just calculate the middle point between lines, then connect those middle points?
        *   - This ^ would get real complex real fast
        *   - How to avoid intersecting lines?
        * - Roughen up the lines a bit
        * - Fill the plates with a random direction
        */

        /*
        * Method C: Another messy but more organic way of generating plates?
        * Steps:
        * - Some kind of recursive node system? (How much memory will this use? Speed concerns?)
        * - Maybe a kind of snake game style thing? Will not be multithreadable
        */

        //Eventually we will need some kind of organic looking flood algorithm-- will be useful for many things later

    }

    void drawOrthogonalLine(Map& map, int32_t targetVal, double point1_x, double point1_y, double point2_x, double point2_y) {
        auto startTime = std::chrono::high_resolution_clock::now();
        int32_t maxX = map.getSizeX();
        int32_t maxY = map.getSizeY();

        uint32_t cellsChanged = 0;

        double realPoint1_x = point1_x * static_cast<double>(maxX);
        double realPoint1_y = point1_y * static_cast<double>(maxY);
        double realPoint2_x = point2_x * static_cast<double>(maxX);
        double realPoint2_y = point2_y * static_cast<double>(maxY);

        

        //origin cells are filled
        map.plateNumAt(static_cast<int32_t>(realPoint1_x), static_cast<int32_t>(realPoint1_y)) = targetVal; //change back to targetVal after debug
        map.plateNumAt(static_cast<int32_t>(realPoint2_x), static_cast<int32_t>(realPoint2_y)) = targetVal;
        cellsChanged = cellsChanged + 2;

        //ok this works for north to south lines, but south to north is funky-- todo: fix this & check if current fix actually fixes this
        //North to south 

        //check if line is NW/SE or NE/SW
        if ((realPoint1_x >= realPoint2_x && realPoint1_y >= realPoint2_y ) || (realPoint1_x < realPoint2_x && realPoint1_y < realPoint2_y)) {//NW/SE
            std::cout << "Line is NW or SE\n";
            if (realPoint1_x > realPoint2_x) { //NW: swap points to SE
                double tempPoint_x = realPoint1_x;
                double tempPoint_y = realPoint1_y;

                realPoint1_x = realPoint2_x;
                realPoint2_x = tempPoint_x;

                realPoint1_y = realPoint2_y;
                realPoint2_y = tempPoint_y;
                std::cout << "Line is NW, swapped points around.\n";
            }
            //procedure to fill cells east and south of intersections
            //find line equation y = mx + c
            const double m = (realPoint2_y - realPoint1_y) / (realPoint2_x - realPoint1_x);
            // c = -m(x) + y
            const double c = (-1.0 * m * realPoint1_x) + realPoint1_y;
            //Inverse line equation (can this be optimised?) x = (y-c)/m
            const double mInv = (realPoint2_x - realPoint1_x) / (realPoint2_y - realPoint1_y);

            double point1BorderDistance_x = (static_cast<int32_t>(realPoint1_x) + 1) - realPoint1_x; //Distance from point 1x to east x intercept
            double point2BorderDistance_x = realPoint2_x - static_cast<int32_t>(realPoint2_x);
            double point1BorderDistance_y = (static_cast<int32_t>(realPoint1_y) + 1) - realPoint1_y; //Distance from point 1y to south y intercept
            double point2BorderDistance_y = realPoint2_y - static_cast<int32_t>(realPoint2_y);

            double curY = (m * realPoint1_x) + c - m; //set start y for checks
            double curX = ((realPoint1_y - c) / m) - mInv; //set start X for checks

            for (double x = (realPoint1_x + 1.0 - point1BorderDistance_x); x < realPoint2_x; x = x + 1.0) {
                //get y value of intercept with x, cast to int then fill corresponding cell (y=mx+c)
                /*
                double y = (m * x) + c;
                map.plateNumAt(static_cast<int32_t>(x), static_cast<int32_t>(y)) = targetVal;
                //std::cout << " x=" << x << " y=" << y << "\n";
                //std::cout << "cell: " << static_cast<int32_t>(x) << " " << static_cast<int32_t>(y) << "\n";
                */

                curY = curY + m; //cheaper computation than multiplication ^ (might accrue floating point error though)
                map.plateNumAt(static_cast<int32_t>(x), static_cast<int32_t>(curY)) = targetVal;
                //std::cout << " x=" << x << " y=" << curY << "\n";
                //std::cout << "cell: " << static_cast<int32_t>(x) << " " << static_cast<int32_t>(curY) << "\n";

                cellsChanged++;
            }
            //every y intersection fills the corresponding south cell

            for (double y = realPoint1_y + 1.0 - point1BorderDistance_y; y < realPoint2_y; y = y + 1.0) {
                //get x value of intercept with y, cast to int and the fill corresponding cell (x=(y-c)/m) todo: try to optimise this divison away s
                /*
                double x = (y - c) / m;
                map.plateNumAt(static_cast<int32_t>(x), static_cast<int32_t>(y)) = targetVal;
                //std::cout << " x=" << x << " y=" << y << "\n";
                //std::cout << "cell: " << static_cast<int32_t>(x) << " " << static_cast<int32_t>(y) << "\n";
                */

                curX = curX + mInv; //cheaper computation than division ^ (might have compounding error though)
                map.plateNumAt(static_cast<int32_t>(curX), static_cast<int32_t>(y)) = targetVal;
                //std::cout << " x=" << curX << " y=" << y << "\n";
                //std::cout << "cell: " << static_cast<int32_t>(curX) << " " << static_cast<int32_t>(y) << "\n";
                cellsChanged++;
            }
        }
        else if ((realPoint1_x < realPoint2_x && realPoint1_y > realPoint2_y) || (realPoint1_x > realPoint2_x && realPoint1_y < realPoint2_y)) {//NE/SW
            std::cout << "Line is NE or SW\n";
            if (realPoint1_x > realPoint2_x) { //SW: swap points to NE
                double tempPoint_x = realPoint1_x;
                double tempPoint_y = realPoint1_y;

                realPoint1_x = realPoint2_x;
                realPoint2_x = tempPoint_x;

                realPoint1_y = realPoint2_y;
                realPoint2_y = tempPoint_y;
                std::cout << "Line is SW, swapped points around.\n";
                std::cout << "New points are [" << realPoint1_x << "," << realPoint1_y << "] [" << realPoint2_x << ", " << realPoint2_y << "]\n";
            }
            //find line equation y = mx + c
            const double m = (realPoint2_y - realPoint1_y) / (realPoint2_x - realPoint1_x);
            // c = -m(x) + y
            const double c = (-1.0 * m * realPoint1_x) + realPoint1_y;
            //Inverse line equation (can this be optimised?) x = (y-c)/m
            const double mInv = (realPoint2_x - realPoint1_x) / (realPoint2_y - realPoint1_y);

            double point1BorderDistance_x = (static_cast<int32_t>(realPoint1_x) + 1) - realPoint1_x; //Distance from point 1x to east x intercept
            double point2BorderDistance_x = realPoint2_x - static_cast<int32_t>(realPoint2_x);
            double point1BorderDistance_y = (static_cast<int32_t>(realPoint1_y) + 1) - realPoint1_y; //Distance from point 1y to south y intercept
            double point2BorderDistance_y = realPoint2_y - static_cast<int32_t>(realPoint2_y);

            double curY = (m * realPoint1_x) + c - m; //set start y for checks
            double curX = ((realPoint1_y - c) / m) - mInv; //set start X for checks

            //procedure to fill cells east and north of intersections
            for (double x = (realPoint1_x + 1.0 - point1BorderDistance_x); x < realPoint2_x; x = x + 1.0) {
                curY = curY + m; //cheaper computation than multiplication ^ (might accrue floating point error though)
                map.plateNumAt(static_cast<int32_t>(x), static_cast<int32_t>(curY)) = targetVal;
                //std::cout << " x=" << x << " y=" << curY << "\n";
                //std::cout << "cell: " << static_cast<int32_t>(x) << " " << static_cast<int32_t>(curY) << "\n";

                cellsChanged++;
            }
            //every y intersection fills the corresponding south cell

            for (double y = realPoint1_y + 1.0 - point1BorderDistance_y; y < realPoint2_y; y = y + 1.0) {
                curX = curX + mInv; //cheaper computation than division ^ (might have compounding error though)
                map.plateNumAt(static_cast<int32_t>(curX), static_cast<int32_t>(y) - 1) = targetVal;
                //std::cout << " x=" << curX << " y=" << y << "\n";
                //std::cout << "cell: " << static_cast<int32_t>(curX) << " " << static_cast<int32_t>(y) << "\n";
                cellsChanged++;
            }
        }
        else { //totally vertical or horizonal todo: check if we can get rid of this and use the two above cases
            std::cout << "No line direction???\n";
        }


        //every x intersection fills the corresponding east cell
        
        /*
        for (int32_t x = static_cast<int32_t>(realPoint1_x); x < static_cast<int32_t>(realPoint2_x); x++) {
            //get y value of intercept with x, cast to int then fill corresponding cell (y=mx+c)
            int32_t y = static_cast<int32_t>((m * x) + c); //okay so this works but not the addition (Find out why it's further north than actual point)
            map.plateNumAt(x, y) = targetVal;
            
            //curY = curY + m; //cheaper computation than multiplication ^ (might have compounding floating point error though)
            //map.plateNumAt(x, static_cast<int32_t>(curY)) = targetVal;
            
        }*/
        
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = endTime - startTime;

        std::cout << "Drew line from [" << realPoint1_x << ", " << realPoint1_y << "] to [" << realPoint2_x << ", " << realPoint2_y << "]\n";
        std::cout << "Drawn " << cellsChanged << "cells in:" << duration << "\n";
    }

    void mapSnake4Directions(Map& map, int32_t startX, int32_t startY, int32_t stepCount, int16_t targetChange, uint64_t seed) {
        //Does a snake game thing idk along 4 directions, maybe we can use this to draw plate borders

        //Set up quick rng
        Splitmix64 random(seed);

        int32_t currentX = startX;
        int32_t currentY = startY;
        int32_t nextX = startX;
        int32_t nextY = startY;

        bool headIsStuck = false;

        //Fill the starting cell
        map.heightAt(currentX, currentY) = targetChange;

        //Main loop
        while (headIsStuck == false) {
            for (int i = 0; i < 10 && stepCount > 0; i++) {
                //pick a random direction (N,S,E,W)
                switch (random.next_int() & 3) {
                case 0:
                    //N
                    nextY++;
                    break;
                case 1:
                    //S
                    nextY--;
                    break;
                case 2:
                    //E
                    nextX++;
                    break;
                case 3:
                    //W
                    nextX--;
                    break;
                default:
                    //default to north
                    nextY++;
                    break;
                }

                //check if cell value is already the same
                //If not, change the current coords to new cell and fill 
                //If yes, pick another direction
                std::cout << map.heightAt(nextX, nextY);
                if (map.heightAt(nextX, nextY) != targetChange) {
                    currentX = nextX;
                    currentY = nextY;
                    map.heightAt(currentX, currentY) = targetChange;
                    i = 0;
                }
                else {
                    //Re-centre next cell
                    nextX = currentX;
                    nextY = currentY;

                }
                stepCount--;
            }
            //If the random attempts fail 10 times in a row then check if the head is stuck 
            headIsStuck = checkIfSurrounded4D(map, currentX, currentY);
        }
        //If we escape this loop it means the head has hit a dead end or is stuck

    }

    void mapSnake8Directions(Map& map, int32_t startX, int32_t startY, int32_t stepCount, int16_t targetChange, uint64_t seed) {
        //Does a snake game thing idk along 8 directions, maybe we can use this to draw plate borders

        //So it actually does n number of steps
        stepCount++;

        //Set up quick rng
        Splitmix64 random(seed);

        int32_t currentX = startX;
        int32_t currentY = startY;
        int32_t nextX = startX;
        int32_t nextY = startY;

        bool headIsStuck = false;

        //Fill the starting cell
        map.heightAt(currentX, currentY) = targetChange;

        //Main loop
        while (headIsStuck == false && stepCount > 0) {
            for (int i = 0; i < 16 && stepCount > 0; i++) {
                //pick a random direction (N,S,E,W)
                int64_t rand_num = random.next_int() & 0b111;
                switch (rand_num) {
                case 0:
                    //N
                    nextY++;
                    break;
                case 1:
                    //S
                    nextY--;
                    break;
                case 2:
                    //E
                    nextX++;
                    break;
                case 3:
                    //W
                    nextX--;
                    break;
                case 4:
                    //NE
                    nextY++;
                    nextX++;
                    break;
                case 5:
                    //NW
                    nextY++;
                    nextX--;
                    break;
                case 6:
                    //SE
                    nextY--;
                    nextX++;
                    break;
                case 7:
                    //SW
                    nextY--;
                    nextX--;
                    break;
                default:
                    //Default to north
                    nextY++;
                    break;
                }

                //check if cell value is already the same
                //If not, change the current coords to new cell and fill 
                //If yes, pick another direction
                if (map.heightAt(nextX, nextY) != targetChange) {
                    currentX = nextX;
                    currentY = nextY;
                    map.heightAt(currentX, currentY) = targetChange;
                    i = 0;
                }
                else {
                    //Re-centre next cell
                    nextX = currentX;
                    nextY = currentY;

                }
                stepCount--;
            }
            //If the random attempts fail 10 times in a row then check if the head is stuck 
            headIsStuck = checkIfSurrounded8D(map, currentX, currentY);
        }
        //If we escape this loop it means the head has hit a dead end or is stuck
    }

    void mapSnake8DirectionsAdditive(Map& map, int32_t startX, int32_t startY, int32_t stepCount, int16_t targetAddChange, uint64_t seed) {
        auto startTime = std::chrono::high_resolution_clock::now();
        //Does a snake game thing idk along 8 directions, maybe we can use this to draw plate borders

        bool issueFlag = false;

        int32_t mapMaxX = map.getSizeX() - 1;
        int32_t mapMaxY = map.getSizeY() - 1;

        //So it actually does n number of steps
        stepCount++;

        //Set up quick rng
        Splitmix64 random(seed);

        int32_t nextX = startX;
        int32_t nextY = startY;

        //Add to the starting cell
        map.heightAt(nextX, nextY) = map.heightAt(nextX, nextY) + targetAddChange;

        //Main loop
        for (int i = stepCount; i > 0; i--) {
            //pick a random direction (N,S,E,W)
            int64_t rand_num = random.next_int() & 0b111;
            switch (rand_num) {
            case 0:
                //N
                //std::cout << nextX << ',' << nextY << ":N\n";
                nextY++;
                break;
            case 1:
                //S
                //std::cout << nextX << ',' << nextY << ":S\n";
                nextY--;
                break;
            case 2:
                //E
                //std::cout << nextX << ',' << nextY << ":E\n";
                nextX++;
                break;
            case 3:
                //W
                //std::cout << nextX << ',' << nextY << ":W\n";
                nextX--;
                break;
            case 4:
                //NE
                //std::cout << nextX << ',' << nextY << ":NE\n";
                nextY++;
                nextX++;
                break;
            case 5:
                //NW
                //std::cout << nextX << ',' << nextY << ":NW\n";
                nextY++;
                nextX--;
                break;
            case 6:
                //SE
                //std::cout << nextX << ',' << nextY << ":SE\n";
                nextY--;
                nextX++;
                break;
            case 7:
                //SW
                //std::cout << nextX << ',' << nextY << ":SW\n";
                nextY--;
                nextX--;
                break;
            default:
                //Default to north
                //std::cout << nextX << ',' << nextY << ":Def N\n";
                nextY++;
                break;
            }

            //todo: add check to ensure thing does not go out of bounds
            //if out of bounds, rerun the random direction
            //todo: How to make sure we aren't adding extra steps if both X and Y are out of bounds
            //todo: Check the performance cost of adding another layer of if checks

            /*
            if (nextX < 0 || nextY < 0 || nextX > mapMaxX || nextY > mapMaxY) { //Max 4 clock cycles(?)
                if (nextX < 0 || nextX > mapMaxX) { //Max 2 clock cycles
                    if (nextX < 0) { //+1 cycle
                        nextX++; //+1 cycle
                    }
                    else {
                        nextY--; //+1 cycle
                    }
                }
                if (nextY < 0 || nextY > mapMaxY) { //Max 2 clock cycles
                    if (nextY < 0) { //+1 cycle
                        nextY++; //+1 cycle
                    }
                    else {
                        nextY--; //+1 cycle
                    }
                i++; //+1 cycle
                continue; //? cycles
                }
            */

            /*
            if (nextX < 0) { //Max 2, min 1 cycles
                nextX++; //+1 cycle
                issueFlag = true; //+1 cycle
            }
            else if (nextX > mapMaxX) {
                nextX--; //+1 cycle
                issueFlag = true; //+1 cycle
            }
            if (nextY < 0) { //Max 2, min 1 cycles
                nextY++; //+1 cycle
                issueFlag = true; //+1 cycle
            }
            else if (nextY > mapMaxY) { //+1 cycle
                nextY--; //+1 cycle
                issueFlag = true; //+1 cycle
            }

            if (issueFlag == true) {//+1 cycle
                i++; //+1 cycle
                issueFlag = false; //+1 cycle
                continue; //? cycles
            }
            */

            /*
            if (nextX < 0 || nextY < 0 || nextX > mapMaxX || nextY > mapMaxY) { //Max 4 clock cycles(?)
                if (nextX < 0) { //+1 cycle
                    nextX++; //+1 cycle
                }
                if (nextY < 0) { //+1 cycle
                    nextY++; //+1 cycle
                }
                if (nextX > mapMaxX) { //+1 cycle
                    nextX--; //+1 cycle
                }
                if (nextY > mapMaxY) { //+1 cycle
                    nextY--; //+1 cycle
                }
                i++; //+1 cycle
                continue; //? cycles
            }
            */

            if (nextX < 0 || nextY < 0 || nextX > mapMaxX || nextY > mapMaxY) { //Max 4, min 1 clock cycles(?)
                if (nextX < 0) { //Max 2, min 1 cycles
                    nextX++; //+1 cycle
                }
                else if (nextX > mapMaxX) {
                    nextX--; //+1 cycle
                }
                if (nextY < 0) { //Max 2, min 1 cycles
                    nextY++; //+1 cycle
                }
                else if (nextY > mapMaxY) {
                    nextY--; //+1 cycle
                }
                i++; //+1 cycle
                continue; //x cycles
            }


            map.heightAt(nextX, nextY) = map.heightAt(nextX, nextY) + targetAddChange;

        }
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration <double, std::milli> executionDuration = endTime - startTime;
        std::cout << "Executed " << (stepCount - 1) << " steps in " << executionDuration << "\n";
    }

    char8_t checkIfSurrounded4D(Map& map, int32_t startX, int32_t startY) {
        int16_t currentValue = map.heightAt(startX, startY);
        bool isSurrounded = false;

        //Check cardinal 4 directions NSEW
        //North
        if (map.heightAt(startX, startY + 1) != currentValue) {
            return isSurrounded;
        }
        //South
        else if (map.heightAt(startX, startY - 1) != currentValue) {
            return isSurrounded;
        }
        //East 
        else if (map.heightAt(startX + 1, startY) != currentValue) {
            return isSurrounded;
        }
        //West
        else {
            isSurrounded = true;
            return isSurrounded;
        }
    }

    char8_t checkIfSurrounded8D(Map& map, int32_t startX, int32_t startY) {
        int16_t currentValue = map.heightAt(startX, startY);
        bool isSurrounded = false;

        //Check 8 directions NSEW and NE, NW, SE, SW
        //North
        if (map.heightAt(startX, startY + 1) != currentValue) {
            return isSurrounded;
        }
        //South
        else if (map.heightAt(startX, startY - 1) != currentValue) {
            return isSurrounded;
        }
        //East 
        else if (map.heightAt(startX + 1, startY) != currentValue) {
            return isSurrounded;
        }
        //West
        else if (map.heightAt(startX - 1, startY) != currentValue) {
            return isSurrounded;
        }
        //Northeast
        else if (map.heightAt(startX + 1, startY + 1) != currentValue) {
            return isSurrounded;
        }
        //Northwest
        else if (map.heightAt(startX + 1, startY - 1) != currentValue) {
            return isSurrounded;
        }
        //Southeast
        else if (map.heightAt(startX - 1, startY + 1) != currentValue) {
            return isSurrounded;
        }
        //Southwest
        else {
            isSurrounded = true;
            return isSurrounded;
        }
    }

    //Voronoi workflow

    void generateVoronoiCells(Map& map, int32_t voronoiCellCount, int32_t cellsToMerge, uint64_t seed) {
        //Generate point coordinates count equal to (voronoiCellCount + cells to merge) --establish a good approx number for nice looking shapes
        //Generate voronoi with jc_voronoi lib 
        //Get edges
        Splitmix64 rng(seed);

        const bool drawPreview = false;

        const int32_t numCellsToGen = voronoiCellCount + cellsToMerge;
        float mapAspectRatio = (float)map.getAspectRatio();
        float rand_x;
        float rand_y;

        //jcv setup
        jcv_rect boundingBox = { {0.0f, 0.0f}, {mapAspectRatio, mapAspectRatio} };
        jcv_diagram diagram;
        std::vector<jcv_point> points;
        const jcv_site* sites;
        jcv_edge_iter edge_iter;
        jcv_edge graph_edge;

        //std::fill(&diagram, sizeof(jcv_diagram), 0);
        memset(&diagram, 0, sizeof(jcv_diagram));

        //generate random points
        points.reserve(numCellsToGen);
        for (int i = 0; i < numCellsToGen; i++) {
            jcv_point tempPoint;

            rand_x = static_cast<float>(rng.next_double_factor()) * mapAspectRatio;
            rand_y = static_cast<float>(rng.next_double_factor());
            
            tempPoint.x = rand_x;
            tempPoint.y = rand_y;

            points.push_back(tempPoint);
            std::cout << "RNG Point at [" << rand_x << "," << rand_y << "]\n";
        }

        //draw points (debug)
        if (drawPreview) {
            const int dispCols = 16;
            const int dispRows = 8; //why tf is this shit flipped todo: fix this
            int tempX, tempY;
            std::array<std::array<char, dispRows>, dispCols> thingView{}; //x, y

            for (int y = 0; y < dispRows; y++) {
                for (int x = 0; x < dispCols; x++) {
                    thingView[x][y] = '.';
                }
            }
            for (int i = 0; i < numCellsToGen; i++) {
                //Put an 'x' for each generated point
                tempX = static_cast<int>((points[i].x / mapAspectRatio) * dispCols);
                tempY = static_cast<int>(points[i].y * dispRows);
                thingView[tempX][tempY] = 'X';
            }

            for (int y = 0; y < dispRows; y++) {
                for (int x = 0; x < dispCols; x++) {
                    std::cout << thingView[x][y];
                }
                std::cout << "\n";
            }
        }

        jcv_diagram_generate(numCellsToGen, points.data(), &boundingBox, 0, &diagram);
    
        sites = jcv_diagram_get_sites(&diagram);

        //Retrieve the edges from sites and draw them
        uint16_t randPlateNum;

        for (int i = 0; i < diagram.numsites; i++) {
            jcv_site_get_edges(&diagram, &sites[i], &edge_iter);
            while (jcv_edge_next(&edge_iter, &graph_edge)) {
                std::cout << "Edge: [" << graph_edge.pos[0].x << ", " << graph_edge.pos[0].y << "] to [" << graph_edge.pos[1].x << ", " << graph_edge.pos[1].y << "]\n";
                //draw line algorithm
                double gradient = (graph_edge.pos[0].y - graph_edge.pos[1].y) / (graph_edge.pos[0].x - graph_edge.pos[1].x);
                int32_t span_x = static_cast<int32_t>(graph_edge.pos[0].x - graph_edge.pos[1].x); //We might lose some data here, watch for gaps
                int32_t span_y = static_cast<int32_t>(graph_edge.pos[0].y - graph_edge.pos[1].y);

                for (int x = 0; x < span_x; x++) {

                }

            }
        }
        
        //Draw edges on map cells temp layer with no aliasing using a random value
        


        //Flood fill cells with a value from an edge (colour)
        //Join cellsToJoin count random adjacent cells and flood fill overwrite their numbers (to get more complex shapes)
    }

    void generateVoronoiTecPlates(Map& map, uint64_t seed) {
        //To be run after generateVoronoiCells()
        //Assign a random direction bearing 0-359 to each unique 'colour' compound cell
        //Assign a random direction velocity 0-255 to each unique 'colour' compound cell
        //Assign a random density 0-255
    }

    void generateTecPlateHeightMap(Map& map, uint64_t seed) {
        //To be run after TecPlates layers are generated
        //Compare neighbour cells (check o complexity of this) bearing, velocity and density to establish outcome heightmap
        //Divergent boundaries-- minimal height diff, volcanic island drift
        //Transverse-- small gorges, minimal topological effect
        //Convergent subductive (if large enough density diff)-- Mountains + trench
        //Convergent non-subductive (if similar density)-- Huge mountain ranges

    }

    void generateTecPlatesGeology(Map& map, uint64_t seed) {
        //To be run after heightmaps
        //Compare neighbour cells to generate geology (and other feature flags)
        std::cout << "Nothing";
    }

    void generateTecPlates() {

    }
}
