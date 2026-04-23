#ifndef FILE_IO_H
#define FILE_IO_H

#include "Graph.h"
#include <string>

namespace FileIO {
    bool loadGraph(Graph& graph, const std::string& filename);
    bool saveGraph(const Graph& graph, const std::string& filename);
    void populateDefaultGraph(Graph& graph);
}

#endif // FILE_IO_H
