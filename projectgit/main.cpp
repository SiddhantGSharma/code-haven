#include "Parser.h"
#include "algorithms/Dijkstra.h"
#include "Graph.h"

int main() {
    Parser parser("datasets/airports.dat", "datasets/routes.dat");
    Vertex node = parser.getNodeMap()["ADQ"];
    std::cout << node.name << " , " << node.code << " , " << node.longitude << " , " << node.latitude << "\n";

    Graph graph = parser.getGraph();
    Vertex src = parser.getNodeMap()["MDW"];
    Vertex dest = parser.getNodeMap()["ORD"];
    std::cout << graph.edgeExists(src, dest) << std::endl;
    auto pair = algorithms::calculateDijkstra(&graph, src, dest);
    std::cout << "DIJKSTRA'S:" << std::endl;
    std::cout << "Distance from src to dest: " <<  pair.second << std::endl;
    std::cout << "Number of airports to go through: " << pair.first.size() - 1 << std::endl;

    auto pair2 = algorithms::calculateAStar(&graph, src, dest);
    std::cout << "ASTAR:" << std::endl;
    std::cout << "Distance from src to dest: " <<  pair2.second << std::endl;
    std::cout << "Number of airports to go through: " << pair2.first.size() - 1 << std::endl;
    return 0;
}
