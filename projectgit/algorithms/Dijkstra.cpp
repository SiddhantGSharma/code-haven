#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "Dijkstra.h"
#include "../Graph.h"
#include <cmath>
// Code inspired by https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/

namespace algorithms {
bool compare(std::pair<Vertex, double> left, std::pair<Vertex, double> right) {
    return left.second > right.second;
}

bool isInHeap(std::vector<std::pair<Vertex, double>> *min_heap, Vertex v) {
    for (std::pair<Vertex, double> &pair : *min_heap) {
        if (pair.first.index == v.index) {
            return true;
        }
    }
    return false;
}

/**
 * Helper function that updates the heap
 **/
void updateHeap(std::vector<std::pair<Vertex, double>> *min_heap, double value, Vertex key) {
    for (std::pair<Vertex, double> &pair : *min_heap) {
        if (key.index == pair.first.index) {
            pair.second = value;
        }
    }
    std::make_heap(min_heap->begin(), min_heap->end(), compare);
}

std::pair<std::vector<Vertex>, double> algorithms::calculateDijkstra(Graph* graph, Vertex src) {
    std::vector<Vertex> vertices = graph->getVertices();
    std::vector<std::vector<Vertex>> paths(1);
    int num_vertices = vertices.size();

    std::unordered_map<int, double> dist;
    std::vector<std::pair<Vertex, double>> min_heap;

    for (int v = 0; v < num_vertices; v++) {
        min_heap.push_back({vertices[v], __DBL_MAX__});
        dist.insert({vertices[v].index, min_heap[v].second});
    }

    dist[src.index] = 0;
    std::make_heap(min_heap.begin(), min_heap.end(), compare);

    while(!min_heap.empty()) {
        std::pair<Vertex, double> pair = min_heap.front();
        Vertex current_vertex = pair.first;
        paths[0].push_back(current_vertex);
        std::pop_heap(min_heap.begin(), min_heap.end(), compare);
        min_heap.pop_back();

        auto adjacent = graph->getAdjacent(pair.first);
        int num_not_updated = 0;
        for (Vertex &adj_vertex : adjacent) {
            double distance = graph->getEdgeWeight(current_vertex, adj_vertex);
            if (isInHeap(&min_heap, adj_vertex) && dist[current_vertex.index] != __DBL_MAX__ && distance + dist[current_vertex.index] < dist[adj_vertex.index]) {
                dist[adj_vertex.index] = dist[current_vertex.index] + distance;
                updateHeap(&min_heap, dist[adj_vertex.index], adj_vertex);
            } else {
                num_not_updated++;
            }
        }
        if (num_not_updated == adjacent.size()) {
            paths.push_back(paths[0]);
        }
    }

    for (auto &pair : dist) {
        if (pair.second != __DBL_MAX__) {
            std::cout << pair.first << " " << pair.second << std::endl;
        }
    }

    for (auto &path : paths) {
        std::cout << path.size() << std::endl;
    }

    return {};
}

std::pair<std::vector<Vertex>, double> algorithms::calculateDijkstra(Graph* graph, Vertex src, Vertex dest) {
    // Get verticies and initalize path
    std::vector<Vertex> vertices = graph->getVertices();
    std::vector<Vertex> path;
    int num_vertices = vertices.size();

    // Generate temp structures to hold distance and cost information
    std::unordered_map<std::string, double> dist;
    std::vector<std::pair<Vertex, double>> min_heap;

    // Initialize the heap with all verticies setting their distance to MAX except the source vertex
    for (int v = 0; v < num_vertices; v++) {
        if (vertices[v] == src) {
            min_heap.push_back({vertices[v], 0});
        } else {
            min_heap.push_back({vertices[v], __DBL_MAX__});
        }
        dist.insert({vertices[v].code, min_heap[v].second});
    }

    dist[src.code] = 0;
    std::make_heap(min_heap.begin(), min_heap.end(), compare);
    
    // While there are still elements to explore
    while(!min_heap.empty()) {
        // Get the min value vertex
        std::pair<Vertex, double> pair = min_heap.front();
        Vertex current_vertex = pair.first;
        path.push_back(current_vertex);
        std::pop_heap(min_heap.begin(), min_heap.end(), compare);
        min_heap.pop_back();

        // Stop if the current vertex is the destination
        if (current_vertex == dest) {
            return {path, dist[current_vertex.code]};
        }

        auto adjacent = graph->getAdjacent(pair.first);
        for (Vertex &adj_vertex : adjacent) {
            // For every vertex compute distance and A star heuristic (distance + distance to dest)

            double distance = graph->getEdgeWeight(current_vertex, adj_vertex);
            if (isInHeap(&min_heap, adj_vertex) && dist[current_vertex.code] != __DBL_MAX__ && distance + dist[current_vertex.code] < dist[adj_vertex.code]) {
                // Update heap accordingly
                dist[adj_vertex.code] = dist[current_vertex.code] + distance;
                updateHeap(&min_heap, dist[adj_vertex.code], adj_vertex);
            } 
        }
    }

    return {path, -1};
}

double convertToRadian(double angle) {
    return angle * (std::atan(1.0)*4) / 180;
}

double computeDistance(const Vertex & source, const Vertex & destination) {
    double lat1 = convertToRadian(source.latitude); 
    double long1 = convertToRadian(source.longitude); 
    double lat2 = convertToRadian(destination.latitude); 
    double long2 = convertToRadian(destination.longitude); 
      
    // Haversine Formula 
    double dlong = long2 - long1; 
    double dlat = lat2 - lat1; 
  
    double ans = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2), 2); 
  
    ans = 2 * asin(sqrt(ans)); 
  
    // Radius of Earth in  
    // Kilometers, R = 6371 
    // Use R = 3956 for miles 
    double R = 6371; 
      
    // Calculate the result 
    ans = ans * R; 
    return ans;
}

std::pair<std::vector<Vertex>, double> algorithms::calculateAStar(Graph* graph, Vertex src, Vertex dest) {
    // Get verticies and initalize path
    std::vector<Vertex> vertices = graph->getVertices();
    std::vector<Vertex> path;
    int num_vertices = vertices.size();

    // Generate temp structures to hold distance and cost information
    std::unordered_map<std::string, double> dist;
    std::unordered_map<std::string, double> cost;
    std::vector<std::pair<Vertex, double>> min_heap;

    // Initialize the heap with all verticies setting their distance to MAX except the source vertex
    for (int v = 0; v < num_vertices; v++) {
        if (vertices[v] == src) {
            min_heap.push_back({vertices[v], 0});
        } else {
            min_heap.push_back({vertices[v], __DBL_MAX__});
        }
        dist.insert({vertices[v].code, min_heap[v].second});
        cost.insert({vertices[v].code, min_heap[v].second});
    }

    dist[src.code] = 0;
    cost[src.code] = 0;
    std::make_heap(min_heap.begin(), min_heap.end(), compare);

    // While there are still elements to explore
    while(!min_heap.empty()) {
        // Get the min value vertex
        std::pair<Vertex, double> pair = min_heap.front();
        Vertex current_vertex = pair.first;
        path.push_back(current_vertex);
        std::pop_heap(min_heap.begin(), min_heap.end(), compare);
        min_heap.pop_back();

        // Stop if the current vertex is the destination
        if (current_vertex == dest) {
            return {path, dist[current_vertex.code]};
        }

        auto adjacent = graph->getAdjacent(pair.first);
        for (Vertex &adj_vertex : adjacent) {
            // For every vertex compute distance and A star heuristic (distance + distance to dest)
            double vertexCost = graph->getEdgeWeight(current_vertex, adj_vertex) + computeDistance(current_vertex, dest);
            double distance = graph->getEdgeWeight(current_vertex, adj_vertex);
            if (isInHeap(&min_heap, adj_vertex) && cost[current_vertex.code] != __DBL_MAX__ && distance + cost[current_vertex.code] < cost[adj_vertex.code]) {
                // Update heap accordingly
                dist[adj_vertex.code] = dist[current_vertex.code] + distance;
                cost[adj_vertex.code] = dist[current_vertex.code] + distance + vertexCost;
                updateHeap(&min_heap, cost[adj_vertex.code], adj_vertex);
            } 
        }
    }

    return {path, -1};
}
}