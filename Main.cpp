#include <iostream>
#include "include/DataManager.hpp"
#include "include/ResultsPredictor.hpp"
#include "include/DrivingAnalysis.hpp"
#include "include/StrategyRecommendation.hpp"

using namespace std;

int main() {
    DataManager dataManager;
    ResultsPredictor predictor;
    DrivingAnalysis analysis; // Instance of the new class
    StrategyRecommendation strategy;

    // Load all necessary data as before
    string circuitsFilename = "Database/circuits.csv";
    string racesFilename = "Database/races.csv";
    string driverFilename = "Database/drivers.csv";
    string driverStandingsFilename = "Database/driver_standings.csv";
    string teamFilename = "Database/constructors.csv";
    string teamStandingsFilename = "Database/constructor_standings.csv";
    string resultsInfoFilename = "Database/results.csv";

    map<int, Circuit> circuits = dataManager.loadCircuits(circuitsFilename);
    map<int, Race> races = dataManager.loadRaces(racesFilename, circuits);
    map<int, Driver> drivers = dataManager.loadDrivers(driverFilename);
    map<int, Team> teams = dataManager.loadTeams(teamFilename);
    map<int, DriverStandings> standings = dataManager.loadDriverStandings(driverStandingsFilename, races, drivers);
    map<int, TeamStandings> teamStandings = dataManager.loadTeamStandings(teamStandingsFilename, races, teams);
    map<int, ResultsInfo> resultsInfo = dataManager.loadResultsInfo(resultsInfoFilename, drivers, teams, races);

    while (true) {
        cout << "--- Main Menu ---\n";
        cout << "1. Driver Results Prediction\n";
        cout << "2. Team Results Prediction\n";
        cout << "3. Predict Importance of Start Position\n";
        cout << "4. Top 5 Drivers Analysis\n";
        cout << "5. Generate Report of Top 5 Drivers\n";
        cout << "6. Top 5 Teams Analysis\n";
        cout << "7. Generate Report of Top 5 Teams\n";
        cout << "8. Pit Stop and Tire Strategy Recommendation\n";
        cout << "9. Fuel Strategy Recommendation\n";
        cout << "10. Car Setup Recommendation\n";
        cout << "11. Exit\n";
        cout << "Choose an option: ";
        int choice;
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
            case 1: {
                cout << "1. General Prediction\n";
                cout << "2. Prediction by Circuit\n";
                cout << "Choose an option for the prediction: ";
                int subChoice;
                cin >> subChoice;
                cin.ignore();

                switch (subChoice) {
                    case 1:
                        predictor.predictResults(drivers, standings, races);
                        break;
                    case 2: {
                        string circuitName;
                        cout << "Enter Circuit Name: ";
                        getline(cin, circuitName);
                        predictor.predictResults(drivers, standings, races, circuitName);
                        break;
                    }
                    default:
                        cout << "Invalid option. Please try again.\n";
                        break;
                }
                break;
            }
            case 2: {
                cout << "1. General Prediction\n";
                cout << "2. Prediction by Circuit\n";
                cout << "Choose an option for the prediction: ";
                int subChoice;
                cin >> subChoice;
                cin.ignore();

                switch (subChoice) {
                    case 1:
                        predictor.predictTeamResults(teams, teamStandings, races);
                        break;
                    case 2: {
                        string circuitName;
                        cout << "Enter Circuit Name: ";
                        getline(cin, circuitName);
                        predictor.predictTeamResults(teams, teamStandings, races, circuitName);
                        break;
                    }
                    default:
                        cout << "Invalid option. Please try again.\n";
                        break;
                }
                break;
            }
            case 3:
                predictor.calculateStartPositionImpact(resultsInfo);
                break;
            case 4: {
                int startYear, endYear;
                cout << "Enter start year: ";
                cin >> startYear;
                cout << "Enter end year: ";
                cin >> endYear;
                auto topDrivers = analysis.calculateTopDrivers(startYear, endYear, drivers, resultsInfo, races);
                analysis.printDriverStats(topDrivers);
                break;
            }
            case 5: {
                string filename;
                int startYear, endYear;
                cout << "Enter filename to save report: ";
                getline(cin, filename);
                cout << "Enter start year: ";
                cin >> startYear;
                cout << "Enter end year: ";
                cin >> endYear;
                auto topDrivers = analysis.calculateTopDrivers(startYear, endYear, drivers, resultsInfo, races);
                analysis.saveDriverStatsToFile(topDrivers, filename);
                cout << "Report saved to '" << filename << "'\n";
                break;
            }
            case 6: {
                int startYear, endYear;
                cout << "Enter start year: ";
                cin >> startYear;
                cout << "Enter end year: ";
                cin >> endYear;
                auto topTeams = analysis.calculateTopTeams(startYear, endYear, teams, teamStandings, races);
                analysis.printTeamStats(topTeams);
                break;
            }
            case 7: {
                string filename;
                int startYear, endYear;
                cout << "Enter filename to save report: ";
                getline(cin, filename);
                cout << "Enter start year: ";
                cin >> startYear;
                cout << "Enter end year: ";
                cin >> endYear;
                auto topTeams = analysis.calculateTopTeams(startYear, endYear, teams, teamStandings, races);
                analysis.saveTeamStatsToFile(topTeams, filename);
                cout << "Report saved to '" << filename << "'\n";
                break;
            }
            case 8: {
                string weather;
                int laps;
                double circuitLength;
                cout << "Enter weather conditions (rainy, hot, etc.): ";
                getline(cin, weather);
                cout << "Enter number of laps: ";
                cin >> laps;
                cout << "Enter circuit length in km: ";
                cin >> circuitLength;
                cin.ignore();

                // Display pit stops recommendation
                cout << strategy.recommendPitStops(weather, laps, circuitLength) << endl;
                
                // Display tire type recommendation
                cout << strategy.recommendTireType(weather, laps) << endl;
                break;
            }
            case 9: {
                int laps;
                double circuitLength;
                string weather;
                cout << "Enter weather conditions (rainy, hot, etc.): ";
                getline(cin, weather);
                cout << "Enter number of laps: ";
                cin >> laps;
                cout << "Enter circuit length in km: ";
                cin >> circuitLength;
                cin.ignore();

                // Display fuel strategy recommendation
                cout << strategy.recommendFuelStrategy(laps, circuitLength, weather) << endl;
                break;
            }
            case 10: {
                string circuitType;
                string weather;
                cout << "Enter circuit type (highDownforce, highSpeed, etc.): ";
                getline(cin, circuitType);
                cout << "Enter weather conditions (rainy, hot, etc.): ";
                getline(cin, weather);

                // Display car setup recommendation
                cout << strategy.recommendCarSetup(circuitType, weather) << endl;
                break;
            }
            case 11:
                cout << "Exiting program...\n";
                return 0;
            default:
                cout << "Invalid option. Please try again.\n";
                break;
        }
    }

    return 0;
}
