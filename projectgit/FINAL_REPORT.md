# Parser - OpenFlights
## Implementation:
The Parser reads in the airports.dat file and the routes.dat file to generate a Graph structure. We do this by processing each line of the airports and routes file. We split the line into a vector using a comma as a delimiter in order to seperate different attributes in the CSV format. Since we know the layout of the CSV file we can extract the name, 3 digit code, the longitude and the latitude of each airport. We can fill this data into our Vertex struct and insert this newly processed Vertex in the graph. For the routes file, we similarly process each line, splitting the line into a vector using a comma as a delimiter, to obtain the source and destination. We simply add an edge using the Graph class between the appropriate Verticies. We also implemented a function which computes the distance between 2 latitude and longitude locations using the Haversine formula. We use this calculated distance as the weight of that edge.

## Findings:
There were a couple of challenges we ran into as we developed this Parser. One challenge was that splitting the lines in the airports file using a comma did not guarantee that we obtain a vector of attribution from the CSV. For example, multiple lines had a comma embedded in the name value of the airport. This would cause our string to double conversion to fail. We worked around this by using a try and catch statement which allows us to work around conversion exceptions. A couple of findings I noticed was there were alot of military airbases in this data file which did not have a standard 3 digit code, we skipped these airports to only consider commercial airports. Futhermore, in the routes file, we noticed that a couple of airports referenced in the routes file, were not contained in the airports file. This led us to presume 
that either the airports or the routes file were outdated.

# Graph implementation
## Implementation:
The Graph we used to represents airports and routes was an adjacency list. We used the same Graph as used in lab_ml with a few adjustments. We created a Vertex struct that represents each airport and an Edge class that represents a route between two airports. For our Graph class, we used a hash map (std::unordered_map) to store our adjacency list.

## Findings:
Since we used the lab_ml graph as a reference, the methods for our class were easy to implement. Some challenges faced were getting our new Vertex struct to work with std::unordered_map. Since it is a hashmap, we couldn't use the Vertex struct as a key without implementing a hash function for it. In the end, our std::hash() overload simply returned a hash of the 3 digit aiport code. Since the codes are unique to each airport, we didn't need to combine the different members of the Vertex struct. 

# DFS Traversal
## Implementation:
The DFS traversal used here traverses all the vertices and labels each edge as DISCOVERY or BACK edges, similar to the one explained in the lecture. Initially, all the vertices and edges are labelled UNEXPLORED. Then traversing through each vertex and its adjacent vertices, if both the vertices have been EXPLORED, the edge between them is labelled BACK, else the edge is labelled DISCOVERY. For easy implementation of this logic, we used a vector of all vertices and a vector of all edges.
## Findings:
While working on the DFS traversal, we found that the logic was really convenient in traversing a graph with disconnected vertices. It also helped in detecting cycles in the graph as each BACK edge denoted a cycle for the graph.

# Dijkastra's Algorithm
## Implementation:
Our method included using a min-heap which would pay attention to the current distances from the source airport to the adjacent airport. This also mean that when we did update the possible distances of airports we needed to heapify once again to make sure we had the closest airport on the top of our list. The great thing about this is the fact that our algorithm correctly found the closest airports and also the farthests ones too in the most optimized way. 

One thing to note about our algorithm is the fact that we only return the distances from the source Airport and also provide the index of that airport based off of the provided dataset.

## Findings:
One of the interesting things that we found while implementing our algorithm is the fact that some airports aren't connected at all to our source in any way. This is really interesting since that means that there might be a whole disconnected graph or just a one off airport that doesn't really take public airplanes. This is great because we can actually use this algorithm to single out those isolated airports too. Even though that is not the direct intention of this algorithm.

# A* Algorithm
## Implementation:
The A star search algorithm is incredibly similar to the Dijkstra algorithm, hence we resued a lot of our code. The only difference between the A star search and Dijkstra is that A star includes a heuristic term which allows the algorithm to prioritize adjacent nodes on more than just distance. In our case, instead of just using the distance from the previous visited Vertex to the current Vertex we can use the distance from the previous vertex to the current vertex plus the distance from the current Vertex to the destination Vertex. This heuristic allows the algorithm to pick a path that has the best probability of encountering the destination, which should result in a faster runtime compared to Dijkstra.    

## Findings:
After implementing A star, we decided to test it on the same cases which we tested our implementation of Dijkstra's algorithm. We noticed that if there was a direct connection between the source and destination, then the Dijkstra and A star algorithm return the same result. However, when there is no direct path, we noticed that the A star algorithm returns a path with fewer verticies at the expense of a slightly larger distance. For example, our Dijkstra algorithm reports a path distance of 700 km going through 51 airports to get from YYZ to ORD. On the other hand, A star reports a path distance of 720 km going through 24 airports to get from YYZ to ORD. This marginal increase in distance results in a runtime speed up since we go through half the number of Verticies. After observing this kind of speedup multiple times, it's become quite apparent to us why A star search is so prominent in game development. It finds a good short path in a low amount of time.
