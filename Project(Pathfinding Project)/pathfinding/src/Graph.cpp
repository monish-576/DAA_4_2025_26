#include "Graph.h"
#include <queue>
#include <limits>
#include <algorithm>

void Graph::addNode(int id, const std::string& name, int x, int y) {
    nodes[id] = Node(id, name, x, y);
}

void Graph::addEdge(int u, int v, double distance, bool bidirectional) {
    if (!hasNode(u) || !hasNode(v)) return;
    
    // Check if edge already exists, update it if so
    bool foundObj = false;
    for (auto& edge : adjList[u]) {
        if (edge.destId == v) {
            edge.distance = distance;
            foundObj = true;
            break;
        }
    }
    if (!foundObj) {
        adjList[u].push_back(Edge(v, distance));
    }
    
    if (bidirectional) {
        bool foundObjReversed = false;
        for (auto& edge : adjList[v]) {
            if (edge.destId == u) {
                edge.distance = distance;
                foundObjReversed = true;
                break;
            }
        }
        if (!foundObjReversed) {
            adjList[v].push_back(Edge(u, distance));
        }
    }
}

bool Graph::removeEdge(int u, int v, bool bidirectional) {
    if (!hasNode(u) || !hasNode(v)) return false;
    
    bool removed = false;
    auto& edgesU = adjList[u];
    for (auto it = edgesU.begin(); it != edgesU.end(); ++it) {
        if (it->destId == v) {
            edgesU.erase(it);
            removed = true;
            break;
        }
    }
    
    if (bidirectional) {
        auto& edgesV = adjList[v];
        for (auto it = edgesV.begin(); it != edgesV.end(); ++it) {
            if (it->destId == u) {
                edgesV.erase(it);
                break;
            }
        }
    }
    
    return removed;
}

bool Graph::removeNode(int id) {
    if (!hasNode(id)) return false;
    
    // Remove all edges pointing to this node
    for (auto& pair : adjList) {
        auto& edges = pair.second;
        edges.erase(std::remove_if(edges.begin(), edges.end(),
            [id](const Edge& e) { return e.destId == id; }), edges.end());
    }
    
    // Remove node and its outgoing edges
    adjList.erase(id);
    nodes.erase(id);
    return true;
}

bool Graph::blockEdge(int u, int v, bool isBlocked, bool bidirectional) {
    if (!hasNode(u) || !hasNode(v)) return false;
    bool modified = false;
    
    for (auto& edge : adjList[u]) {
        if (edge.destId == v) {
            edge.isBlocked = isBlocked;
            modified = true;
            break;
        }
    }
    
    if (bidirectional) {
        for (auto& edge : adjList[v]) {
            if (edge.destId == u) {
                edge.isBlocked = isBlocked;
                modified = true;
                break;
            }
        }
    }
    
    return modified;
}

Node Graph::getNode(int id) const {
    auto it = nodes.find(id);
    if (it != nodes.end()) {
        return it->second;
    }
    return Node();
}

bool Graph::hasNode(int id) const {
    return nodes.find(id) != nodes.end();
}

const std::unordered_map<int, Node>& Graph::getAllNodes() const {
    return nodes;
}

const std::unordered_map<int, std::vector<Edge>>& Graph::getAdjList() const {
    return adjList;
}

void Graph::clear() {
    nodes.clear();
    adjList.clear();
}

PathResult Graph::dijkstra(int startId, int endId) const {
    PathResult result;
    if (!hasNode(startId) || !hasNode(endId)) return result;

    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;
    
    // Ensure all distances are infinity initially
    for (const auto& pair : nodes) {
        dist[pair.first] = std::numeric_limits<double>::infinity();
    }
    
    // priority_queue stores pairs of (distance, node_id). 
    // greater is used for min-heap
    using PDI = std::pair<double, int>;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI>> pq;

    dist[startId] = 0.0;
    pq.push({0.0, startId});

    while (!pq.empty()) {
        int u = pq.top().second;
        double currentDist = pq.top().first;
        pq.pop();

        if (u == endId) break;
        if (currentDist > dist[u]) continue;

        auto it = adjList.find(u);
        if (it != adjList.end()) {
            for (const Edge& edge : it->second) {
                if (edge.isBlocked) continue; // Skip blocked edges

                int v = edge.destId;
                double weight = edge.distance;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    if (dist[endId] == std::numeric_limits<double>::infinity()) {
        return result; // No path found
    }

    // Reconstruct path
    int curr = endId;
    while (curr != startId) {
        result.path.push_back(curr);
        curr = parent[curr];
    }
    result.path.push_back(startId);
    std::reverse(result.path.begin(), result.path.end());
    
    result.found = true;
    result.totalDistance = dist[endId];
    return result;
}

PathResult Graph::bfs(int startId, int endId) const {
    PathResult result;
    if (!hasNode(startId) || !hasNode(endId)) return result;

    std::unordered_map<int, bool> visited;
    std::unordered_map<int, int> parent;
    std::queue<int> q;

    q.push(startId);
    visited[startId] = true;

    bool found = false;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == endId) {
            found = true;
            break;
        }

        auto it = adjList.find(u);
        if (it != adjList.end()) {
            for (const Edge& edge : it->second) {
                if (edge.isBlocked) continue;

                int v = edge.destId;
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
    }

    if (!found) return result;

    // Output path and compute rough distance
    double totalDist = 0.0;
    int curr = endId;
    std::vector<int> pathReversed;
    while (curr != startId) {
        pathReversed.push_back(curr);
        int p = parent[curr];
        
        // Find edge weight to add strictly for reporting
        auto it = adjList.find(p);
        if (it != adjList.end()) {
            for (const Edge& e : it->second) {
                if (e.destId == curr) {
                    totalDist += e.distance;
                    break;
                }
            }
        }
        curr = p;
    }
    pathReversed.push_back(startId);
    
    result.path = pathReversed;
    std::reverse(result.path.begin(), result.path.end());
    
    result.found = true;
    result.totalDistance = totalDist;
    return result;
}

std::vector<PathResult> Graph::getAlternativePaths(int startId, int endId) const {
    std::vector<PathResult> alternatives;
    
    // First, get the primary shortest path
    PathResult primary = dijkstra(startId, endId);
    if (!primary.found) return alternatives;
    
    alternatives.push_back(primary);
    
    // Extremely simplistic k-shortest paths approach:
    // Block each edge of the primary path one by one, find shortest path, then unblock.
    // Store unique paths (up to 3 total).
    
    // We need a temporary non-const graph for this, or simulate blocking.
    // Let's create a temporary modified graph:
    Graph tempGraph = *this; 
    
    for (size_t i = 0; i < primary.path.size() - 1; i++) {
        int u = primary.path[i];
        int v = primary.path[i+1];
        
        // Back up old block state
        bool wasBlockedU = false;
        bool wasBlockedV = false;
        // Find existing blocking status (just in case they were already blocked somehow, 
        // though Dijkstra wouldn't have used them)
        
        tempGraph.blockEdge(u, v, true, true);
        
        PathResult altPath = tempGraph.dijkstra(startId, endId);
        
        if (altPath.found) {
            // Check if it's unique
            bool isUnique = true;
            for (const auto& existing : alternatives) {
                if (existing.path == altPath.path) {
                    isUnique = false;
                    break;
                }
            }
            if (isUnique) {
                alternatives.push_back(altPath);
            }
        }
        
        // Unblock
        tempGraph.blockEdge(u, v, false, true); // this restores to unblocked
        
        if (alternatives.size() >= 3) break;
    }
    
    return alternatives;
}
