
#include <iostream>
#include <chrono>
#include <fstream>

#include "gen.h"
#include "map.h"
#include "SplitMix64.h"

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
                case 1:
                    //S
                    nextY--;
                case 2:
                    //E
                    nextX++;
                case 3:
                    //W
                    nextX--;
                default: 
                    //default to north
                    nextY++;
                }

                //check if cell value is already the same
                //If not, change the current coords to new cell and fill 
                //If yes, pick another direction
                if (map.tempValueAt(nextX, nextY) != targetChange) {
                    currentX = nextX;
                    currentY = nextY;
                    map.tempValueAt(currentX, currentY) = targetChange;
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
            for (int i = 0; i < 16 && stepCount > 0; i++) {
                //pick a random direction (N,S,E,W)
                switch (random.next_int() & 0b111) {
                case 0:
                    //N
                    nextY++;
                case 1:
                    //S
                    nextY--;
                case 2:
                    //E
                    nextX++;
                case 3:
                    //W
                    nextX--;
                case 4:
                    //NE
                    nextY++;
                    nextX++;
                case 5:
                    //NW
                    nextY++;
                    nextX--;
                case 6:
                    //SE
                    nextY--;
                    nextX++;
                case 7:
                    //SW
                    nextY--;
                    nextX--;
                default:
                    //Default to north
                    nextY++;
                }

                //check if cell value is already the same
                //If not, change the current coords to new cell and fill 
                //If yes, pick another direction
                if (map.tempValueAt(nextX, nextY) != targetChange) {
                    currentX = nextX;
                    currentY = nextY;
                    map.tempValueAt(currentX, currentY) = targetChange;
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

    char8_t checkIfSurrounded4D(Map& map, int32_t startX, int32_t startY) {
        int16_t currentValue = map.tempValueAt(startX, startY);
        bool isSurrounded = false;

        //Check cardinal 4 directions NSEW
        //North
        if (map.tempValueAt(startX, startY + 1) != currentValue) {
            return isSurrounded;
        }
        //South
        else if (map.tempValueAt(startX, startY - 1) != currentValue) {
            return isSurrounded;
        }
        //East 
        else if (map.tempValueAt(startX + 1, startY) != currentValue) {
            return isSurrounded;
        }
        //West
        else {
            isSurrounded = true;
            return isSurrounded;
        }
    }

    char8_t checkIfSurrounded8D(Map& map, int32_t startX, int32_t startY) {
        int16_t currentValue = map.tempValueAt(startX, startY);
        bool isSurrounded = false;

        //Check 8 directions NSEW and NE, NW, SE, SW
        //North
        if (map.tempValueAt(startX, startY + 1) != currentValue) {
            return isSurrounded;
        }
        //South
        else if (map.tempValueAt(startX, startY - 1) != currentValue) {
            return isSurrounded;
        }
        //East 
        else if (map.tempValueAt(startX + 1, startY) != currentValue) {
            return isSurrounded;
        }
        //West
        else if (map.tempValueAt(startX - 1, startY) != currentValue) {
            return isSurrounded;
        }
        //Northeast
        else if (map.tempValueAt(startX + 1, startY + 1) != currentValue) {
            return isSurrounded;
        }
        //Northwest
        else if (map.tempValueAt(startX + 1, startY - 1) != currentValue) {
            return isSurrounded;
        }
        //Southeast
        else if (map.tempValueAt(startX - 1, startY + 1) != currentValue) {
            return isSurrounded;
        }
        //Southwest
        else {
            isSurrounded = true;
            return isSurrounded;
        }
    }
}
