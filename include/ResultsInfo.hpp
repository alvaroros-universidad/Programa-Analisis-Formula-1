#ifndef RESULTS_INFO_HPP
#define RESULTS_INFO_HPP

#include "Driver.hpp"
#include "Team.hpp"
#include "Race.hpp"
#include <string>

using namespace std;

class ResultsInfo {
public:
    int resultId;
    const Driver* driver;
    const Team* team;
    const Race* race;    // Agregando referencia a la carrera
    int grid;
    int position;
    int points;

    // Constructor predeterminado
    ResultsInfo();

    // Constructor con parámetros
    ResultsInfo(int resultId, const Driver* driver, const Team* team, const Race* race, int grid, int position, int points);
};

#endif // RESULTS_INFO_HPP