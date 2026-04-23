#include "NavigationSystem.h"
#include "FileIO.h"
#include "Utils.h"
#include <iostream>
#include <vector>
#include <iomanip>

NavigationSystem::NavigationSystem(const std::string& file) : dataFile(file), isAdmin(false), adminPassword("admin123") {
    if (!FileIO::loadGraph(graph, dataFile)) {
        std::cout << "Data file not found. Initializing default Chandigarh University map...\n";
        FileIO::populateDefaultGraph(graph);
        FileIO::saveGraph(graph, dataFile);
    }
}

int NavigationSystem::findClosestNodeByName(const std::string& name) {
    auto nodes = graph.getAllNodes();
    for (const auto& pair : nodes) {
        if (Utils::fuzzyMatch(pair.second.name, name)) {
            return pair.first;
        }
    }
    return -1;
}

void NavigationSystem::displayMap() {
    std::cout << "\n=============================================\n";
    std::cout << "          CHANDIGARH UNIVERSITY MAP          \n";
    std::cout << "=============================================\n";
    
    // Simple ASCII 2D Map approach.
    // Dimensions: 80 cols, 24 rows
    const int cols = 80;
    const int rows = 24;
    std::vector<std::string> grid(rows, std::string(cols, ' '));
    
    auto nodes = graph.getAllNodes();
    for (const auto& pair : nodes) {
        const Node& n = pair.second;
        if (n.y >= 0 && n.y < rows && n.x >= 0 && n.x < cols - 4) {
            std::string label = "[" + std::to_string(n.id) + "]";
            for(size_t i=0; i<label.size(); i++) {
                grid[n.y][n.x + i] = label[i];
            }
        }
    }
    
    for (int i=0; i<rows; i++) {
        std::cout << grid[i] << "\n";
    }
    
    std::cout << "\nLegend:\n";
    for (const auto& pair : nodes) {
        std::cout << "[" << pair.first << "] " << pair.second.name << "\n";
    }
}

void NavigationSystem::printRouteDetails(int startId, int endId, const PathResult& result, bool showAlternatives) {
    if (!result.found) {
        std::cout << "No path found from " << graph.getNode(startId).name << " to " << graph.getNode(endId).name << ".\n";
        return;
    }

    std::cout << "\n--- ROUTE ---\n";
    std::cout << "Path: ";
    for (size_t i = 0; i < result.path.size(); ++i) {
        std::cout << graph.getNode(result.path[i]).name;
        if (i < result.path.size() - 1) std::cout << " -> ";
    }
    std::cout << "\nTotal Distance: " << result.totalDistance << " meters.\n";
    std::cout << "Estimated Walking Time: " << Utils::formatTime(result.totalDistance, 5.0) << "\n";

    if (showAlternatives) {
        std::vector<PathResult> alts = graph.getAlternativePaths(startId, endId);
        if (alts.size() > 1) {
            std::cout << "\nAlternative Routes:\n";
            for (size_t i = 1; i < alts.size(); ++i) {
                std::cout << "Alt " << i << ": ";
                for (size_t j = 0; j < alts[i].path.size(); ++j) {
                    std::cout << graph.getNode(alts[i].path[j]).name;
                    if (j < alts[i].path.size() - 1) std::cout << " -> ";
                }
                std::cout << " (" << alts[i].totalDistance << "m)\n";
            }
        }
    }
}

void NavigationSystem::findShortestRoute() {
    std::cin.ignore();
    std::string startQuery, endQuery;
    
    std::cout << "\nEnter starting location (name or ID): ";
    std::getline(std::cin, startQuery);
    
    std::cout << "Enter destination location (name or ID): ";
    std::getline(std::cin, endQuery);

    int startId = -1, endId = -1;
    
    // Try parse as integer first
    try { startId = std::stoi(startQuery); } catch(...) { startId = findClosestNodeByName(startQuery); }
    try { endId = std::stoi(endQuery); } catch(...) { endId = findClosestNodeByName(endQuery); }
    
    if (startId == -1 || !graph.hasNode(startId)) {
        std::cout << "Start location not found.\n";
        return;
    }
    if (endId == -1 || !graph.hasNode(endId)) {
        std::cout << "Destination not found.\n";
        return;
    }

    std::cout << "Choose algorithm (1: Dijkstra [Shortest Distance], 2: BFS [Fewest Turns]): ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        PathResult res = graph.dijkstra(startId, endId);
        printRouteDetails(startId, endId, res, true);
    } else {
        PathResult res = graph.bfs(startId, endId);
        printRouteDetails(startId, endId, res, false);
    }
}

void NavigationSystem::searchLocation() {
    std::cin.ignore();
    std::string q;
    std::cout << "Enter location name to search: ";
    std::getline(std::cin, q);
    
    auto nodes = graph.getAllNodes();
    bool foundObj = false;
    std::cout << "Search Results:\n";
    for (const auto& pair : nodes) {
        if (Utils::fuzzyMatch(pair.second.name, q)) {
            std::cout << "[" << pair.first << "] " << pair.second.name << " (Coordinates: " << pair.second.x << "," << pair.second.y << ")\n";
            foundObj = true;
        }
    }
    if (!foundObj) {
        std::cout << "No matches found.\n";
    }
}

void NavigationSystem::adminMenu() {
    std::cin.ignore();
    std::cout << "Enter Admin Password: ";
    std::string p;
    std::getline(std::cin, p);

    if (p != adminPassword) {
        std::cout << "Access Denied.\n";
        return;
    }

    isAdmin = true;
    while (isAdmin) {
        std::cout << "\n--- ADMIN MENU ---\n";
        std::cout << "1. Add Location\n";
        std::cout << "2. Add Path\n";
        std::cout << "3. Block/Unblock Path (Traffic)\n";
        std::cout << "4. Save & Exit Admin Mode\n";
        std::cout << "Choice: ";
        
        int choice;
        if (!(std::cin >> choice)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }

        if (choice == 1) {
            int id, x, y;
            std::string name;
            std::cout << "Enter new ID: "; std::cin >> id;
            std::cin.ignore();
            std::cout << "Enter name: "; std::getline(std::cin, name);
            std::cout << "Enter x coordinate [0-80]: "; std::cin >> x;
            std::cout << "Enter y coordinate [0-24]: "; std::cin >> y;
            
            graph.addNode(id, name, x, y);
            std::cout << "Location added.\n";
            
        } else if (choice == 2) {
            int u, v;
            double dist;
            std::cout << "Enter Source ID: "; std::cin >> u;
            std::cout << "Enter Dest ID: "; std::cin >> v;
            std::cout << "Enter Distance (meters): "; std::cin >> dist;
            
            graph.addEdge(u, v, dist);
            std::cout << "Path added.\n";
            
        } else if (choice == 3) {
            int u, v;
            int block;
            std::cout << "Enter Path Source ID: "; std::cin >> u;
            std::cout << "Enter Path Dest ID: "; std::cin >> v;
            std::cout << "Enter 1 to block, 0 to unblock: "; std::cin >> block;
            
            if (graph.blockEdge(u, v, block == 1)) {
                std::cout << "Path " << (block == 1 ? "blocked" : "unblocked") << ".\n";
            } else {
                std::cout << "Path not found.\n";
            }
        } else if (choice == 4) {
            FileIO::saveGraph(graph, dataFile);
            std::cout << "Data saved. Exiting admin mode.\n";
            isAdmin = false;
        }
    }
}

void NavigationSystem::run() {
    bool running = true;
    while (running) {
        std::cout << "\n=============================================\n";
        std::cout << "    CU CAMPUS ROUTE & NAVIGATION SYSTEM      \n";
        std::cout << "=============================================\n";
        std::cout << "1. Display Campus Map\n";
        std::cout << "2. Find Shortest Route\n";
        std::cout << "3. Search Location\n";
        std::cout << "4. Admin Tools\n";
        std::cout << "5. Exit\n";
        std::cout << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1: displayMap(); break;
            case 2: findShortestRoute(); break;
            case 3: searchLocation(); break;
            case 4: adminMenu(); break;
            case 5: running = false; break;
            default: std::cout << "Invalid choice.\n";
        }
    }
}
