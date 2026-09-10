
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
}
