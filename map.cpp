#include "map.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

    Map::Map(int size_x, int size_y) :
        size_x(size_x),
        size_y(size_y),
        height(size_x* size_y, 0),
        precipitation(size_x* size_y, 0)
    {}

    int Map::getSizeX() const {
        return size_x;
    }

    int Map::getSizeY() const {
        return size_y;
    }

    int Map::index(int x, int y) const {
        return y * size_x + x;
    }

    __int16& Map::heightAt(int x, int y) {
        return height[index(x, y)];
    }

    __int16& Map::precipitationAt(int x, int y) {
        return precipitation[index(x, y)];
    }

    const __int16& Map::heightAt(int x, int y) const {
        return height[index(x, y)];
    }

    const __int16& Map::precipitationAt(int x, int y) const {
        return precipitation[index(x, y)];
    }

