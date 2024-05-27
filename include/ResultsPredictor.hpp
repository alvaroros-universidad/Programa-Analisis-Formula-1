#ifndef RESULTS_PREDICTOR_HPP
#define RESULTS_PREDICTOR_HPP

#include <string>
#include <map>
#include <vector>
#include "Driver.hpp"
#include "DriverStandings.hpp"
#include "Race.hpp"
#include "Team.hpp"
#include "TeamStandings.hpp"
#include "ResultsInfo.hpp"

using namespace std;

class ResultsPredictor {
private:
    double calculatePearsonCorrelation(const vector<int>& x, const vector<int>& y);
public:
    void predictResults(const map<int, Driver>& drivers, const map<int, DriverStandings>& standings, const map<int, Race>& races, const string& circuitName = "");
    void predictTeamResults(const map<int, Team>& teams, const map<int, TeamStandings>& standings, const map<int, Race>& races, const string& circuitName = "");
    void calculateStartPositionImpact(const map<int, ResultsInfo>& resultsInfo);
};

#endif // RESULTS_PREDICTOR_HPP