/**
  * Taken from Sean Massung (lab_ml) for use in our Final Project
 */
#pragma once

#include <list>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <climits>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <set>
#include <sstream>
#include <vector>

#include "Edge.h"

using std::cerr;
using std::cout;
using std::endl;
using std::vector;
using std::set;
using std::string;
using std::to_string;
using std::vector;
using std::pair;
using std::make_pair;
using std::unordered_map;


/**
 * Represents a graph; used by the GraphTools class.
 *
 */
class Graph
{
public:
    /**
     * Constructor to create an empty graph.
     * @param weighted - specifies whether the graph is a weighted graph or
     *  not
     */
    Graph(bool weighted);

    /**
     * Constructor to create an empty graph.
     * @param weighted - specifies whether the graph is a weighted graph or
     *  not
     * @param directed - specifies whether the graph is directed
     */
    Graph(bool weighted, bool directed);

    /**
     * Gets all adjacent vertices to the parameter vertex.
     * @param source - vertex to get neighbors from
     * @return a vector of vertices
     */
    vector<Vertex> getAdjacent(Vertex source) const;

    /**
     * Returns one vertex in the graph. This function can be used
     *  to find a random vertex with which to start a traversal.
     * @return a vertex from the graph
     */
    Vertex getStartingVertex() const;

    /**
     * Gets all vertices in the graph.
     * @return a vector of all vertices in the graph
     */
    vector<Vertex> getVertices() const;
    

    /**
     * Gets an edge between two vertices.
     * @param source - one vertex the edge is connected to
     * @param destination - the other vertex the edge is connected to
     * @return - if exist, return the corresponding edge
     *         - if edge doesn't exist, return Edge()
     */
    Edge getEdge(Vertex source, Vertex destination) const;

    /**
     * Gets all the edges in the graph.
     * @return a vector of all the edges in the graph
     */
    vector<Edge> getEdges() const;

    /**
     * Checks if the given vertex exists.
     * @return - if Vertex exists, true
     *         - if Vertex doesn't exist, return false
     */
    bool vertexExists (Vertex v) const;

    /**
     * Sets the label of the Vertex v.
     * @param v - given vertex to set label of
     * @param l - Label of the vertex
     * @return - if vertex exists, set the label to the corresponding vertex, return vertex with new label
     *         - if vertex doesn't exist, return InvalidVertex
     */
    Vertex setVertexLabel(Vertex v, string l);

    /**
     * Gets the vertex label of the Vertex v.
     * @param v - one vertex the edge is connected to
     * @return - if vertex exists, return vertex label
     *         - if vertex doesn't exist, return InvalidLabel
     */
    string getVertexLabel(Vertex v) const;

    /**
     * Checks if edge exists between two vertices exists.
     * @return - if Edge exists, true
     *         - if Edge doesn't exist, return false
     */
    bool edgeExists(Vertex source, Vertex destination) const;

    /**
     * Sets the edge label of the edge between vertices u and v.
     * @param source - one vertex the edge is connected to
     * @param destination - the other vertex the edge is connected to
     * @return - if edge exists, set the label to the corresponding edge(if not directed, set the reverse one too), return edge with new label
     *         - if edge doesn't exist, return InvalidEdge
     */
    Edge setEdgeLabel(Vertex source, Vertex destination, string label);

    /**
     * Gets the edge label of the edge between vertices u and v.
     * @param source - one vertex the edge is connected to
     * @param destination - the other vertex the edge is connected to
     * @return - if edge exists, return edge label
     *         - if edge doesn't exist, return InvalidLabel
     */
    string getEdgeLabel(Vertex source, Vertex destination) const;

    /**
     * Gets the weight of the edge between two vertices.
     * @param source - one vertex the edge is connected to
     * @param destination - the other vertex the edge is connected to
     * @return - if edge exists, return edge wright
     *         - if doesn't, return InvalidWeight
     */
    int getEdgeWeight(Vertex source, Vertex destination) const;

    /**
     * Inserts a new vertex into the graph and initializes its label as "".
     * @param v - the name for the vertex
     */
    void insertVertex(Vertex v);

    /**
     * Removes a given vertex from the graph.
     * @param v - the vertex to remove
     * @return - if v exists, return v
     *         - if not, return InvalidVertex;
     */
    Vertex removeVertex(Vertex v);

    /**
     * Inserts an edge between two vertices.
     * A boolean is returned for use with the random graph generation.
     * Hence, an error is not thrown when it fails to insert an edge.
     * @param source - one vertex the edge is connected to
     * @param destination - the other vertex the edge is connected to
     * @return whether inserting the edge was successful
     */
    bool insertEdge(Vertex source, Vertex destination);

    /**
     * Removes the edge between two vertices.
     * @param source - one vertex the edge is connected to
     * @param destination - the other vertex the edge is connected to
     * @return - if edge exists, remove it and return removed edge
     *         - if not, return InvalidEdge
     */
    Edge removeEdge(Vertex source, Vertex destination);

    /**
     * Sets the weight of the edge between two vertices.
     * @param source - one vertex the edge is connected to
     * @param destination - the other vertex the edge is connected to
     * @param weight - the weight to set to the edge
     * @return - if edge exists, set edge weight and return  edge with new weight
     *         - if not, return InvalidEdge
     */
    Edge setEdgeWeight(Vertex source, Vertex destination, int weight);

    /**
     * DFS traversal of the entire graph. Labels each Vertex as EXPLORED
     * and each Edge as DISCOVERY or BACK edges. 
     */
    void DFS();

    bool isDirected() const;

    void clear();


    const static Vertex InvalidVertex;
    const static Edge InvalidEdge;
    const static int InvalidWeight;
    const static string InvalidLabel;

private:
    /**
     * Helper function for DFS Traversal of the graph
     * @param v - the currently visited vertex 
     */
    void DFS(Vertex v);

    mutable unordered_map<Vertex, unordered_map<Vertex, Edge>> adjacency_list;

    bool weighted;
    bool directed;
    int picNum;
    string picName;


    /**
     * Returns whether a given vertex exists in the graph.
     * @param v - the vertex to check
     * @param functionName - the name of the calling function to return
     *  in the event of an error
     */
    bool assertVertexExists(Vertex v, string functionName) const;

    /**
     * Returns whether thee edge exists in the graph.
     * @param source - one vertex
     * @param destination - another vertex
     * @param functionName - the name of the calling function to return
     *  in the event of an error
     */
    bool assertEdgeExists(Vertex source, Vertex destination, string functionName) const;


    /**
     * Prints a graph error and quits the program.
     * The program is exited with a segfault to provide a stack trace.
     * @param message - the error message that is printed
     */
    void error(string message) const;
};
