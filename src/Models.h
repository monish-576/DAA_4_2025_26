#ifndef MODELS_H
#define MODELS_H

#include <string>

struct Edge {
    int destId;
    double distance;
    bool isBlocked;

    Edge(int dest, double dist) : destId(dest), distance(dist), isBlocked(false) {}
};

struct Node {
    int id;
    std::string name;
    int x;
    int y;

    Node() : id(-1), name(""), x(0), y(0) {}
    Node(int i, std::string n, int x_coord, int y_coord) 
        : id(i), name(n), x(x_coord), y(y_coord) {}
};

#endif // MODELS_H
