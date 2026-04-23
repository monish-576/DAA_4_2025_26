#ifndef NAVIGATION_SYSTEM_H
#define NAVIGATION_SYSTEM_H

#include "Graph.h"
#include <string>

class NavigationSystem {
private:
    Graph graph;
    std::string dataFile;
    bool isAdmin;
    std::string adminPassword;

    void displayMap();
    void findShortestRoute();
    void searchLocation();
    void adminMenu();
    void printRouteDetails(int startId, int endId, const PathResult& result, bool showAlternatives = false);
    
    int findClosestNodeByName(const std::string& name);

public:
    NavigationSystem(const std::string& file);
    void run();
};

#endif // NAVIGATION_SYSTEM_H
