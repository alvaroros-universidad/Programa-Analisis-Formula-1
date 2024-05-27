#include "ResultsInfo.hpp"

// Constructor predeterminado
ResultsInfo::ResultsInfo()
    : resultId(0), driver(nullptr), team(nullptr), race(nullptr), grid(0), position(0), points(0) {}

// Constructor con parámetros
ResultsInfo::ResultsInfo(int resultId, const Driver* driver, const Team* team, const Race* race, int grid, int position, int points)
    : resultId(resultId), driver(driver), team(team), race(race), grid(grid), position(position), points(points) {}