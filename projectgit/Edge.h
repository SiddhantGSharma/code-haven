#pragma once

#include <string>
#include <limits.h>
#include <iostream>
#include <unordered_map>

using std::string;

//Vertex of our Graph, represents an Aiport
struct Vertex {
    std::string name;
    std::string code;
    std::string label;
    double latitude;
    double longitude;
    int index;
    bool operator==(const Vertex& a) const {
        return (code == a.code);
    }
    friend bool operator<(const Vertex& v1, const Vertex& v2) {
        return v1.code < v2.code;
    }
};

// hash function for Vertex to use as Key in unordered map
namespace std {
template <> struct std::hash<Vertex> {
    size_t operator() (const Vertex& vertex)  const {
        //the airport codes are unique so no need to combine other members
        return hash<std::string>()(vertex.code);
    }
};
}

/**
 * Edge in a graph; used by the Graph class, represents a route between two Airports.

 * Taken from Sean Massung (lab_ml) for use in our Final Project
 */
class Edge
{
  public:

    Vertex source; /**< The source of the edge **/
    Vertex dest; /**< The destination of the edge **/

    /**
     * Parameter constructor for unweighted graphs.
     * @param u - one vertex the edge is connected to
     * @param v - the other vertex it is connected to
     */
    Edge(Vertex u, Vertex v)
        : source(u), dest(v), label(""), weight(-1) {}

    /**
     * Parameter constructor for unweighted graphs.
     * @param u - one vertex the edge is connected to
     * @param v - the other vertex it is connected to
     * @param lbl - the edge label
     */
    Edge(Vertex u, Vertex v, string lbl)
        : source(u), dest(v), label(lbl), weight(-1) {}

    /**
     * Parameter constructor for weighted graphs.
     * @param u - one vertex the edge is connected to
     * @param v - the other vertex it is connected to
     * @param w - the weight of the edge
     * @param lbl - the edge label
     */
    Edge(Vertex u, Vertex v, double w, string lbl)
        : source(u), dest(v), label(lbl), weight(w) {}

    /**
     * Default constructor.
     */
    Edge() : source({"", "", "", 0, 0, 0}), dest({"", "", "", 0, 0, 0}), label(""), weight(-1) {}

    /**
     * Compares two Edges.
     * operator< is defined so Edges can be sorted with std::sort.
     * @param other - the edge to compare with
     * @return whether the current edge is less than the parameter
     */
    bool operator<(const Edge& other) const
    {
        return weight < other.weight;
    }

    /**
     * Gets edge label.
     */
    string getLabel() const
    {
        return this->label;
    }

    /**
     * Gets edge weight.
     */
    double getWeight() const
    {
        return this->weight;
    }

    /**
     * Compares two edges' source and dest.
     * @param other - the edge to compare with
     */
    bool operator==(Edge& other) const
    {
        if (!(this->source == other.source))
            return false;
        if (!(this->dest == other.dest))
            return false;
        return true;
    }

private:
    string label; /**< The edge label **/
    double weight; /**< The edge weight ie. distance between two Airports **/

};
