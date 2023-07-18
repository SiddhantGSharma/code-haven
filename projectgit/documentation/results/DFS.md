# DFS Traversal - OpenFlights
## Implementation:
The DFS traversal used here traverses all the vertices and labels each edge as DISCOVERY or BACK edges, similar to the one explained in the lecture. Initially, all the vertices and edges are labelled UNEXPLORED. Then traversing through each vertex and its adjacent vertices, if both the vertices have been EXPLORED, the edge between them is labelled BACK, else the edge is labelled DISCOVERY. For easy implementation of this logic, we used a vector of all vertices and a vector of all edges.
## Findings:
While working on the DFS traversal, we found that the logic was really convenient in traversing a graph with disconnected vertices. It also helped in detecting cycles in the graph as each BACK edge denoted a cycle for the graph.