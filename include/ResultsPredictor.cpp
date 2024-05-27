#include "ResultsPredictor.hpp"
#include <iostream>
#include <algorithm>
#include <cmath> // Necesario para log y max
#include <numeric>
#include <vector>

using namespace std;

void ResultsPredictor::predictResults(const map<int, Driver>& drivers, const map<int, DriverStandings>& standings, const map<int, Race>& races, const string& circuitName) {
    vector<string> driverNames;
    string inputName;
    cout << "Ingrese los nombres de los conductores (escriba 'fin' para terminar):" << endl;
    while (true) {
        getline(cin, inputName);
        if (inputName == "fin") {
            break;
        }
        driverNames.push_back(inputName);
    }

    map<int, pair<double, double>> driverPoints;
    for (const auto& entry : standings) {
        const DriverStandings& ds = entry.second;
        if (!ds.driver || !ds.race || find(driverNames.begin(), driverNames.end(), ds.driver->fullName) == driverNames.end()) {
            continue;
        }

        // Filtra por circuito si se proporciona un nombre de circuito
        if (!circuitName.empty() && (!ds.race->circuit || ds.race->circuit->name != circuitName)) {
            continue;
        }

        int currentYear = 2023;
        double yearsSinceRace = currentYear - ds.race->year + 1;
        double weight = 1.0 / max(1.0, log(yearsSinceRace));  // Uso de logaritmo para suavizar la penalización
        driverPoints[ds.driver->driverId].first += ds.points * weight;
        driverPoints[ds.driver->driverId].second += weight;
    }

    vector<pair<double, int>> weightedAverages;
    for (const auto& dp : driverPoints) {
        if (dp.second.second > 0) {
            double average = dp.second.first / dp.second.second;
            weightedAverages.push_back(make_pair(average, dp.first));
        }
    }

    sort(weightedAverages.rbegin(), weightedAverages.rend());

    cout << "Pronóstico de resultados basado en el desempeño pasado" << (circuitName.empty() ? "" : " para el circuito '" + circuitName + "'") << ":" << endl;
    for (const auto& wa : weightedAverages) {
        cout << "Conductor ID " << wa.second << " (" << drivers.at(wa.second).fullName << "): " << wa.first << " puntos" << endl;
    }
}

void ResultsPredictor::predictTeamResults(const map<int, Team>& teams, const map<int, TeamStandings>& standings, const map<int, Race>& races, const string& circuitName) {
    vector<string> teamNames;
    string inputName;
    cout << "Ingrese los nombres de los equipos (escriba 'fin' para terminar):" << endl;
    while (true) {
        getline(cin, inputName);
        if (inputName == "fin") {
            break;
        }
        teamNames.push_back(inputName);
    }

    map<int, pair<double, double>> teamPoints; // teamId -> (suma ponderada de puntos, suma de pesos)
    for (const auto& entry : standings) {
        const TeamStandings& ts = entry.second;
        if (!ts.team || !ts.race || find(teamNames.begin(), teamNames.end(), ts.team->name) == teamNames.end()) {
            continue;
        }

        // Filtra por circuito si se proporciona un nombre de circuito
        if (!circuitName.empty() && (!ts.race->circuit || ts.race->circuit->name != circuitName)) {
            continue;
        }

        int currentYear = 2023;
        double yearsSinceRace = currentYear - ts.race->year + 1;
        double weight = 1.0 / max(1.0, log(yearsSinceRace)); // Uso de logaritmo para suavizar la penalización
        teamPoints[ts.team->teamId].first += ts.points * weight;
        teamPoints[ts.team->teamId].second += weight;
    }

    vector<pair<double, int>> weightedAverages;
    for (const auto& tp : teamPoints) {
        if (tp.second.second > 0) {
            double average = tp.second.first / tp.second.second;
            weightedAverages.push_back(make_pair(average, tp.first));
        }
    }

    sort(weightedAverages.rbegin(), weightedAverages.rend());

    cout << "Pronóstico de resultados para equipos" << (circuitName.empty() ? "" : " para el circuito '" + circuitName + "'") << ":" << endl;
    for (const auto& wa : weightedAverages) {
        cout << "Equipo ID " << wa.second << " (" << teams.at(wa.second).name << "): " << wa.first << " puntos" << endl;
    }
}

double ResultsPredictor::calculatePearsonCorrelation(const vector<int>& x, const vector<int>& y) {
    int n = x.size();
    double sum_x = accumulate(x.begin(), x.end(), 0.0);
    double sum_y = accumulate(y.begin(), y.end(), 0.0);
    double sum_x2 = inner_product(x.begin(), x.end(), x.begin(), 0.0);
    double sum_y2 = inner_product(y.begin(), y.end(), y.begin(), 0.0);
    double sum_xy = inner_product(x.begin(), x.end(), y.begin(), 0.0);

    double numerator = n * sum_xy - sum_x * sum_y;
    double denominator = sqrt((n * sum_x2 - sum_x * sum_x) * (n * sum_y2 - sum_y * sum_y));

    return (denominator == 0) ? 0 : numerator / denominator;
}

void ResultsPredictor::calculateStartPositionImpact(const map<int, ResultsInfo>& resultsInfo) {
    vector<int> startPositions;
    vector<int> finalPositions;

    for (const auto& entry : resultsInfo) {
        const ResultsInfo& info = entry.second;
        if (info.driver && info.race) { // Asegúrate de que los datos son válidos
            startPositions.push_back(info.grid);
            finalPositions.push_back(info.position); // Asume que position es un string que se puede convertir a int
        }
    }

    double correlation = calculatePearsonCorrelation(startPositions, finalPositions);
    cout << "Pearson Correlation: " << correlation << endl;
    if (correlation > 0) {
        cout << "Based on the correlation being positive, a better initial position indicates a better final position." << endl;
    } else if (correlation < 0) {
        cout << "Based on the correlation being negative, it might be a better initial position does not indicate a better final position" << endl;
    } else {
        cout << "There is no clear correlation between initial position and final position" << endl;
    }
}