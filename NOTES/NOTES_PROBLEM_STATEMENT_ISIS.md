# Goals and Problem statement when designing ISIS

ISIS (Intermediate System to Intermediate System) is a IGP routing protocol used by routers to exchange routing information within a single Autonomous System (AS).

## Problem statement

- How to get shortest cost information between routers.
- How to ensure no loops.
- How to learn the network topology (the graph of routers)

## Generic steps to build a Routing Table

1. __Link State Package (LSP):__
Build input graph/neighbour/nbr DB for each router with network topology information using hello packages (Point to point routers). The DB is incapsulated in LSP.
LSP is shared between all router in the topology. The collection of all LSP that every routers hold are called LSP-DB aka Network graph.

2. __Apply Shortest Path Algorithm (SPA):__ 

LSP-DB aka Network graph used to calculate (SPA).
Allgorithm to use: Dijkstra Algorithm

3. __Routing table calculation:__
Create routing tables from previous steps by mapping nodes to 
the nodes destination address in the table calculated from step 2.

## Dijkstra's algorithm

7 steps of Dijkstra's algorithm:

1. Assign a tentative distance value to every node: set it to zero for the source node and to infinity for all other nodes.

2. Set the initial visited node to be the source node.

3. For the current visited node, consider all of its neighbors and calculate their tentative distances. The tentative distance is the distance from the source node to the neighbor node through the current visited node. If this tentative distance is less than the current distance of the neighbor node, update its distance value to the tentative value.

4. Mark the current visited node as visited.

5. From the set of unvisited nodes, select the node with the smallest distance value, and make it the next visited node.

6. Repeat steps 3-5 until the destination node is marked as visited or there are no more unvisited nodes.

7. Once the destination node is marked as visited, the algorithm can backtrack from the destination node to the source node, following the path with the smallest total distance.

Dijkstra's algorithm is guaranteed to find the shortest path between the source node and any other node in the graph, provided that the graph has no negative-weight edges.