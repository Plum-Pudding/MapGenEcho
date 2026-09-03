#pragma once
#include "map.h"

namespace Gen {
	void fillBlanketGridMap(Map& map, int16_t target);

	void printGridMap(const Map& map);

	void saveGridMap(const Map& map);

	void generateMap(Map& map);

	void mapProcedureA(Map& map, int32_t startX, int32_t startY, int32_t stepCount, int16_t targetChange);

	char8_t checkIfSurrounded4D(Map& map, int32_t startX, int32_t startY);

	char8_t checkIfSurrounded8D(Map& map, int32_t startX, int32_t startY);
}