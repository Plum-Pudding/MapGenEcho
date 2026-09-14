#pragma once
#include <vector>

class Map{
public:
    Map(int32_t size_x, int32_t size_y);

    int32_t getSizeX() const;

    int32_t getSizeY() const;

    double getAspectRatio() const;

    int32_t index(int32_t x, int32_t y) const;

    int16_t& heightAt(int32_t x, int32_t y);

    int16_t& precipitationAt(int32_t x, int32_t y);

    int16_t& tempValueAt(int32_t x, int32_t);

    const int16_t& heightAt(int32_t x, int32_t y) const;

    const int16_t& precipitationAt(int32_t x, int32_t y) const;

    const int16_t& tempValueAt(int32_t x, int32_t y) const;

private:
    int32_t size_x, size_y;
    double aspectRatio; // x/y

    std::vector<int16_t> tectonicDirection;
    std::vector<int16_t> height;
    std::vector<int16_t> precipitation;
    std::vector<int16_t> tempValue;
    std::vector<int16_t> features; //To store unique features later
};

