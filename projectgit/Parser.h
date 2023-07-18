#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include "Node.h"
#include "Graph.h"

class Parser {
    public:
        /**
         * Custom constructor for the Parser object
         * @param filePath The path to the file containing Airport data
         * @param routePath The path to the file containing Routes data
         **/
        Parser(const std::string & filePath, const std::string & routePath);

        /**
         * Get the Verticies (Nodes) from the Airports file
         * @return A vector of Verticies
         **/
        std::vector<Vertex> getNodes() const;

        /**
         * Get a hashmap that maps the 3 character airport code to the specific Airport vertex object
         * @return Hashmap from 3 character code to vertex
         **/
        std::unordered_map<std::string, Vertex> getNodeMap() const;

        /**
         * Get the Graph object that represents the Airport and Routes file
         * @return The graph with the Airports as Verticies and the routes as edges
         **/
        Graph getGraph() const;
    private:
        std::vector<Vertex> nodes;
        std::unordered_map<std::string, Vertex> nodeMap;
        Graph graph;
    private:
        std::vector<std::string> splitLine(std::string input, char delimiter);
        double computeDistance(const Vertex & source, const Vertex & destination);
        double convertToRadian(double angle);
};