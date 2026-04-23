#include "FileIO.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace FileIO {

    void populateDefaultGraph(Graph& graph) {
        // Populating Chandigarh University Mock Data
        graph.clear();

        // 1. Administration Block: (40, 12)
        // 2. Main Library: (40, 8)
        // 3. Block A: (25, 8)
        // 4. Block B: (25, 4)
        // 5. Block C: (55, 8)
        // 6. Block D: (55, 4)
        // 7. Nek Chand Boys Hostel: (65, 12)
        // 8. Zakir Boys Hostel: (70, 16)
        // 9. Sukhna Girls Hostel: (15, 12)
        // 10. Tagore Girls Hostel: (10, 16)
        // 11. Sports Ground: (40, 2)
        // 12. Food Court: (40, 18)
        // 13. Auditorium: (25, 18)
        
        graph.addNode(1, "Administration Block", 40, 12);
        graph.addNode(2, "Main Library", 40, 8);
        graph.addNode(3, "Academic Block A", 25, 8);
        graph.addNode(4, "Academic Block B", 25, 4);
        graph.addNode(5, "Academic Block C", 55, 8);
        graph.addNode(6, "Academic Block D", 55, 4);
        graph.addNode(7, "Nek Chand Boys Hostel", 65, 12);
        graph.addNode(8, "Zakir Boys Hostel", 70, 16);
        graph.addNode(9, "Sukhna Girls Hostel", 15, 12);
        graph.addNode(10, "Tagore Girls Hostel", 10, 16);
        graph.addNode(11, "Sports Ground", 40, 2);
        graph.addNode(12, "Food Court", 40, 18);
        graph.addNode(13, "Auditorium", 25, 18);

        // Edges (distances are simulated in meters)
        graph.addEdge(1, 2, 200);   // Admin - Library
        graph.addEdge(2, 3, 350);   // Library - Block A
        graph.addEdge(3, 4, 150);   // Block A - Block B
        graph.addEdge(2, 5, 350);   // Library - Block C
        graph.addEdge(5, 6, 150);   // Block C - Block D
        
        graph.addEdge(5, 7, 400);   // Block C - Nek Chand
        graph.addEdge(7, 8, 300);   // Nek Chand - Zakir
        
        graph.addEdge(3, 9, 400);   // Block A - Sukhna
        graph.addEdge(9, 10, 300);  // Sukhna - Tagore
        
        graph.addEdge(4, 11, 250);  // Block B - Sports
        graph.addEdge(6, 11, 250);  // Block D - Sports
        graph.addEdge(2, 11, 400);  // Library - Sports
        
        graph.addEdge(1, 12, 300);  // Admin - Food Court
        graph.addEdge(9, 12, 500);  // Sukhna - Food Court
        graph.addEdge(1, 13, 350);  // Admin - Auditorium
        graph.addEdge(12, 13, 200); // Food Court - Auditorium
        graph.addEdge(7, 12, 800);  // Nek Chand - Food Court (Long path)
    }

    bool loadGraph(Graph& graph, const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        graph.clear();
        std::string line;
        int mode = 0; // 1 = nodes, 2 = edges

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            if (line == "# NODES") {
                mode = 1;
                continue;
            } else if (line == "# EDGES") {
                mode = 2;
                continue;
            }

            std::stringstream ss(line);
            if (mode == 1) {
                int id, x, y;
                std::string name;
                ss >> id >> x >> y;
                // Get the rest of the line as name
                std::getline(ss, name);
                name.erase(0, name.find_first_not_of(" \t")); // trim leading
                graph.addNode(id, name, x, y);
            } else if (mode == 2) {
                int u, v;
                double dist;
                bool isBlocked;
                ss >> u >> v >> dist >> isBlocked;
                graph.addEdge(u, v, dist, false); // Add unidirectional multiple times based on save file
                graph.blockEdge(u, v, isBlocked, false);
            }
        }
        
        return true;
    }

    bool saveGraph(const Graph& graph, const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return false;

        file << "# NODES\n";
        auto nodes = graph.getAllNodes();
        for (const auto& pair : nodes) {
            const Node& n = pair.second;
            file << n.id << " " << n.x << " " << n.y << " " << n.name << "\n";
        }

        file << "# EDGES\n";
        auto adjList = graph.getAdjList();
        for (const auto& pair : adjList) {
            int u = pair.first;
            for (const Edge& e : pair.second) {
                file << u << " " << e.destId << " " << e.distance << " " << e.isBlocked << "\n";
            }
        }

        return true;
    }

}
