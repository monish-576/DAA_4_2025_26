#ifndef GRAPH_H
#define GRAPH_H

#include "Models.h"
#include <unordered_map>
#include <vector>

struct PathResult {
    bool found;
    std::vector<int> path;
    double totalDistance;
    
    PathResult() : found(false), totalDistance(0.0) {}
};

class Graph {
private:
    std::unordered_map<int, Node> nodes;
    std::unordered_map<int, std::vector<Edge>> adjList;

public:
    void addNode(int id, const std::string& name, int x, int y);
    void addEdge(int u, int v, double distance, bool bidirectional = true);
    bool removeNode(int id);
    bool removeEdge(int u, int v, bool bidirectional = true);
    
    // Toggle traffic block on an edge
    bool blockEdge(int u, int v, bool isBlocked, bool bidirectional = true);

    // Getters
    Node getNode(int id) const;
    bool hasNode(int id) const;
    const std::unordered_map<int, Node>& getAllNodes() const;
    const std::unordered_map<int, std::vector<Edge>>& getAdjList() const;
    void clear();

    // Algorithms
    PathResult dijkstra(int startId, int endId) const;
    PathResult bfs(int startId, int endId) const;
    
    // Multi-route (returns up to 3 paths)
    std::vector<PathResult> getAlternativePaths(int startId, int endId) const;
};

#endif // GRAPH_H
