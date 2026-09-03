#pragma once

class Map{
public:
    Map(int size_x, int size_y);

    int getSizeX() const;

    int getSizeY() const;

    int index(int x, int y) const;

    __int16& heightAt(int x, int y);

    __int16& precipitationAt(int x, int y);

    const __int16& heightAt(int x, int y) const;

    const __int16& precipitationAt(int x, int y) const;

private:
    int size_x, size_y;

    std::vector<__int16> tectonicDirection;
    std::vector<__int16> height;
    std::vector<__int16> precipitation;
};

