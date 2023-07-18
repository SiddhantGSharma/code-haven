#pragma once
#include <vector>
#include <iostream>
#include "../Graph.h"

namespace algorithms {
    /**
     * Run the Dijkstra algorithm on one vertex
     * @param graph Pointer to graph to use
     * @param src The source vertex
     * @return A vector of verticies that are reachable from this node and their distance
     **/
    std::pair<std::vector<Vertex>, double> calculateDijkstra(Graph *graph, Vertex src);

    /**
     * Run the Dijkstra algorithm to find a path from one vertex to a destination
     * @param graph Pointer to graph to use
     * @param src The source vertex
     * @param dest The destination vertex
     * @return A vector of verticies that represents the path from the source to the destination along with the total distance for the walk
     **/
    std::pair<std::vector<Vertex>, double> calculateDijkstra(Graph* graph, Vertex src, Vertex dest);

    /**
     * Run the A star search algorithm to find a path from one vertex to a destination
     * @param graph Pointer to graph to use
     * @param src The source vertex
     * @param dest The destination vertex
     * @return A vector of verticies that represents the path from the source to the destination along with the total distance for the walk
     **/
    std::pair<std::vector<Vertex>, double> calculateAStar(Graph* graph, Vertex src, Vertex dest);
}