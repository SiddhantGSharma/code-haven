# Dijkastra's Algorithm - OpenFlights
## Implementation:
Our method included using a min-heap which would pay attention to the current distances from the source airport to the adjacent airport. This also mean that when we did update the possible distances of airports we needed to heapify once again to make sure we had the closest airport on the top of our list. The great thing about this is the fact that our algorithm correctly found the closest airports and also the farthests ones too in the most optimized way. 

One thing to note about our algorithm is the fact that we only return the distances from the source Airport and also provide the index of that airport based off of the provided dataset.

## Findings:
One of the interesting things that we found while implementing our algorithm is the fact that some airports aren't connected at all to our source in any way. This is really interesting since that means that there might be a whole disconnected graph or just a one off airport that doesn't really take public airplanes. This is great because we can actually use this algorithm to single out those isolated airports too. Even though that is not the direct intention of this algorithm.

