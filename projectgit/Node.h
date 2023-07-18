#pragma once
#include <string>
#include <list>

struct Node {
    std::string name;
    std::string code;
    double latitude;
    double longitude;
    std::list<Node *> adjacencyList;
};