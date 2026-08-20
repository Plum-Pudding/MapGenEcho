// MapsEcho.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "util.cpp"
#include <iostream>
#include <vector>
#include <string>
#include <array>

#define DEFAULT_MAPSIZE_X 32
#define DEFAULT_MAPSIZE_Y 32

#define DEFAULT_RESOLUTION 100 //size of one point in metres

class Map {
public:
    Map(int size_x, int size_y) :
        size_x(size_x),
        size_y(size_y),
        height(size_x * size_y, 0),
        precipitation(size_x * size_y, 0)
    {}

    int getSizeX() const {
        return size_x;
    }

    int getSizeY() const {
        return size_y;
    }

    int index(int x, int y) const {
        return y * size_x + x;
    }

    __int16& heightAt(int x, int y) {
        return height[index(x, y)];
    }

    __int16& precipitationAt(int x, int y) {
        return precipitation[index(x, y)];
    }

    const __int16& heightAt(int x, int y) const {
        return height[index(x, y)];
    }

    const __int16& precipitationAt(int x, int y) const {
        return precipitation[index(x, y)];
    }

private:
    int size_x, size_y;

    std::vector<__int16> height;
    std::vector<__int16> precipitation;
};

void printGridMap(const Map& map) {
    for (int x = 0; x < map.getSizeX(); x++) {
        for (int y = 0; y < map.getSizeY(); y++) {
            std::cout << map.heightAt(x, y);
        }
        std::cout << "\n";
    }
}

int main() {
    Map testMap(DEFAULT_MAPSIZE_X, DEFAULT_MAPSIZE_Y);

    testMap.heightAt(4, 3) = 2;
    std::cout << "Height at 4, 4 is " << testMap.heightAt(4, 4) << "\n";
    std::cout << "Height at 4, 3 is " << testMap.heightAt(4, 3) << "\n";

    printGridMap(testMap);

    return 0;
}

