#include "catch.hpp"

#include "../Parser.h"
#include "../algorithms/Dijkstra.h"
#include "../Graph.h"

Parser parser = Parser("datasets/airports.dat", "datasets/routes.dat");
Graph graph = parser.getGraph();
std::unordered_map<std::string, Vertex> nodeMap = parser.getNodeMap();

TEST_CASE("Parser returns some Airports") {
    REQUIRE(parser.getNodes().size() > 0);
}

TEST_CASE("Parser returns the name of the Airports properly") {
    REQUIRE(nodeMap["ORD"].name == "Chicago O'Hare International Airport");
    REQUIRE(nodeMap["YYZ"].name == "Lester B. Pearson International Airport");
    REQUIRE(nodeMap["EVE"].name == "Harstad/Narvik Airport, Evenes");
    REQUIRE(nodeMap["HGI"].name == "Paloich Airport, Heliport");
}

TEST_CASE("Parser NodeMap Edge Exists") {
    Vertex src = parser.getNodeMap()["MDW"];
    Vertex dest = parser.getNodeMap()["ORD"];
    REQUIRE(graph.edgeExists(src, dest) == false); 
    REQUIRE(graph.edgeExists(nodeMap["CMI"], dest) == true); 
}

TEST_CASE("Dijkstra Direct Route") {
    Vertex src = parser.getNodeMap()["CMI"];
    Vertex dest = parser.getNodeMap()["ORD"];
    auto pair = algorithms::calculateDijkstra(&graph, src, dest);
    REQUIRE(pair.first.size() == 2); 
}

TEST_CASE("Dijkstra No Route") {
    Vertex src = parser.getNodeMap()["CMI"];
    Vertex dest = parser.getNodeMap()["NOAIRPORT"];
    auto pair = algorithms::calculateDijkstra(&graph, src, dest);
    REQUIRE(pair.second == -1); 
}

TEST_CASE("AStar Direct Route") {
    Vertex src = parser.getNodeMap()["CMI"];
    Vertex dest = parser.getNodeMap()["ORD"];
    auto pair = algorithms::calculateAStar(&graph, src, dest);
    REQUIRE(pair.first.size() == 2); 
}

TEST_CASE("AStar No Route") {
    Vertex src = parser.getNodeMap()["CMI"];
    Vertex dest = parser.getNodeMap()["NOAIRPORT"];
    auto pair = algorithms::calculateAStar(&graph, src, dest);
    REQUIRE(pair.second == -1); 
}