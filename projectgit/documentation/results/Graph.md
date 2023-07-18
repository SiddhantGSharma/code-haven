# Graph implementation
## Implementation:
The Graph we used to represents airports and routes was an adjacency list. We used the same Graph as used in lab_ml with a few adjustments. We created a Vertex struct that represents each airport and an Edge class that represents a route between two airports. For our Graph class, we used a hash map (std::unordered_map) to store our adjacency list.

## Findings:
Since we used the lab_ml graph as a reference, the methods for our class were easy to implement. Some challenges faced were getting our new Vertex struct to work with std::unordered_map. Since it is a hashmap, we couldn't use the Vertex struct as a key without implementing a hash function for it. In the end, our std::hash() overload simply returned a hash of the 3 digit aiport code. Since the codes are unique to each airport, we didn't need to combine the different members of the Vertex struct. 
