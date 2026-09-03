#include "map.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

    Map::Map(int32_t size_x, int32_t size_y) :
        size_x(size_x),
        size_y(size_y),
        height(size_x* size_y, 0),
        precipitation(size_x* size_y, 0)
    {}

    int32_t Map::getSizeX() const {
        return size_x;
    }

    int32_t Map::getSizeY() const {
        return size_y;
    }

    int32_t Map::index(int32_t x, int32_t y) const {
        return y * size_x + x;
    }

    int16_t& Map::heightAt(int32_t x, int32_t y) {
        return height[index(x, y)];
    }

    int16_t& Map::precipitationAt(int32_t x, int32_t y) {
        return precipitation[index(x, y)];
    }

    int16_t& Map::tempValueAt(int32_t x, int32_t y) {
        return tempValue[index(x, y)];
    }

    const int16_t& Map::heightAt(int32_t x, int32_t y) const {
        return height[index(x, y)];
    }

    const int16_t& Map::precipitationAt(int32_t x, int32_t y) const {
        return precipitation[index(x, y)];
    }

    const int16_t& Map::tempValueAt(int32_t x, int32_t y) const {
        return tempValue[index(x, y)];
    }

