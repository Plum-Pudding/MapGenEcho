#pragma once
#include "map.h"

namespace Gen {
	void fillBlanketGridMap(Map& map, int16_t target);

	void printGridMap(const Map& map);

	void saveGridMap(const Map& map);

	void generateMap(Map& map);

	void drawAliasedLine(Map& map, double point1_x, double point1_y, double point2_x, double point2_y);

	void mapSnake4Directions(Map& map, int32_t startX = 0, int32_t startY = 0, int32_t stepCount = 1, int16_t targetChange = 1, uint64_t seed = 0);

	void mapSnake8Directions(Map& map, int32_t startX = 0, int32_t startY = 0, int32_t stepCount = 1, int16_t targetChange = 1, uint64_t seed = 0);

	void mapSnake8DirectionsAdditive(Map& map, int32_t startX = 0, int32_t startY = 0, int32_t stepCount = 1, int16_t targetChange = 1, uint64_t seed = 0);

	char8_t checkIfSurrounded4D(Map& map, int32_t startX, int32_t startY);

	char8_t checkIfSurrounded8D(Map& map, int32_t startX, int32_t startY);

	//Voronoi workflow
	void generateVoronoiCells(Map& map, int32_t voronoiCellCount, int32_t cellsToMerge = 0, uint64_t seed = 0);

	void generateVoronoiTecPlates(Map& map, uint64_t seed = 0);

	void generateTecPlateHeightMap(Map& map, uint64_t seed = 0);

	void generateTecPlatesGeology(Map& map, uint64_t seed = 0);
}