#pragma once
#include <vector>
#include <cstdint>

class Map{
public:
    Map(int32_t size_x, int32_t size_y);

    int32_t getSizeX() const;

    int32_t getSizeY() const;

    double getAspectRatio() const;

    int32_t index(int32_t x, int32_t y) const;

    uint16_t& heightAt(int32_t x, int32_t y);

    uint16_t& precipitationAt(int32_t x, int32_t y);

    uint16_t& plateNumAt(int32_t x, int32_t);

    const uint16_t& heightAt(int32_t x, int32_t y) const;

    const uint16_t& precipitationAt(int32_t x, int32_t y) const;

    const uint16_t& plateNumAt(int32_t x, int32_t y) const;

private:
    int32_t size_x, size_y;
    double aspectRatio; // x/y

    //Stored values
    std::vector<uint16_t> height;
    std::vector<uint16_t> precipitation;

    //Used during mapgen -- maybe store later with just plateNum in an optimised way (just via a dictionary? i.e. plate num 5 has bearing 270, density 128 etc)
    std::vector<uint16_t> plateNum;
    std::vector<uint16_t> plateBearing;
    std::vector<uint8_t> plateVelocity;
    std::vector<uint8_t> plateDensity;

    };

