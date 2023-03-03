# Notes IGP: Interior Gateway Protocol

IGP is routing protocols used by routers to exchange routing information within a single Autonomous System (AS).

## Autonomous System (AS)
An Autonomous System (AS) is a collection of interconnected networks that operate under a common administrative domain. Within an AS, routers use IGP protocols to share information about the topology of the network and the routing paths between nodes. This information is used to determine the most efficient path for forwarding packets within the network.

## Examples IGP Protocols:

__Routing Information Protocol (RIP):__ 
A simple distance-vector protocol that uses hop count as a metric to determine the best path between nodes.

__Open Shortest Path First (OSPF):__
A link-state protocol that uses a more complex metric based on the cost of the links between nodes.

__Intermediate System to Intermediate System (ISIS):__
A link-state protocol similar to OSPF.


## Routing in multiple Autonomous System (AS)
IGP protocols are designed for use within a single AS, and are not typically used for routing between multiple ASes. For that purpose, Border Gateway Protocol (BGP), an exterior gateway protocol (EGP), is commonly used.

## ISIS vs OSPF

- ISIS used in conjuction with OSI

- OSPF used in conjuction with TCP/IP

Protocol Type: ISIS is a link-state protocol, while OSPF is a hybrid protocol that combines both link-state and distance-vector elements.

Network Hierarchy: ISIS supports a two-level hierarchy, whereas OSPF supports a three-level hierarchy. This means that ISIS is better suited for smaller networks, while OSPF is more suitable for larger networks with complex topologies.

Metric Calculation: ISIS uses a variable-length subnet mask (VLSM) to calculate metrics, while OSPF uses a fixed-length subnet mask (FLSM). This makes OSPF more efficient in terms of metric calculation and faster in terms of network convergence.

Convergence: OSPF has faster network convergence times compared to ISIS because it uses a designated router (DR) to minimize the amount of flooding that takes place in the network.

Protocol Overhead: ISIS has a higher protocol overhead than OSPF because it requires more network resources to maintain its hierarchical structure.

Overall, both ISIS and OSPF are effective routing protocols, but the choice between the two depends on the specific requirements of the network. For smaller networks, ISIS may be a better choice, while larger and more complex networks may benefit from OSPF's hierarchical structure and faster convergence times.

## Theory IGP protocols

- __Adjaceney mgmt (Each router no its neighbour):__
    - Sending and recieving heloo packages periodically
    - Update neighbours state machines

- __Building link state DBs:__
    - Building link state packets.
    - Flooding link state packets.
    - Build a view of network topology (a Graph).

- __Running SPF algoritm:__

- __Route Calculation:__

## LSDB (Link State Database)

In computer networking, LSDB stands for Link State Database. It is a database maintained by each router in a network that stores information about the entire network topology. The information in the LSDB is used by link-state routing protocols, such as OSPF (Open Shortest Path First) and ISIS (Intermediate System to Intermediate System), to calculate the shortest path between any two nodes in the network.

Each router in the network collects information about the state of its connected links and the state of its neighboring routers. This information is stored in the router's LSDB and is used to build a complete view of the network topology. The LSDB contains information about each router in the network, including its IP address, the state of its links, and the cost of each link.

The LSDB is constantly updated as routers exchange link-state updates with their neighbors. When a change occurs in the network, such as a link failure, the affected router updates its LSDB and propagates the update to its neighbors. The neighboring routers then update their LSDBs and recalculate the shortest path to each destination.

The LSDB is a critical component of link-state routing protocols because it enables routers to make informed routing decisions based on the complete network topology. By maintaining a complete view of the network, the LSDB enables routers to select the shortest and most efficient path between any two nodes in the network, and it enables the network to quickly adapt to changes in the topology.

## SPF (Shortest Path First) algorithm

The SPF (Shortest Path First) algorithm is a key component of link-state routing protocols used in computer networks. It is used to calculate the shortest path between two nodes on a network, and it is used in routing protocols such as OSPF (Open Shortest Path First) and IS-IS (Intermediate System to Intermediate System).

The SPF algorithm works by constructing a graph of the network, with each node representing a network device such as a router, and each edge representing a physical link between devices. Each node maintains a link-state database that contains information about the entire network topology, including the cost of each link and the state of each device.

Once the link-state database is constructed, the SPF algorithm is used to calculate the shortest path between any two nodes in the network. This is done by iteratively calculating the cost of each possible path from the source node to all other nodes in the network, and selecting the path with the lowest cost.

The SPF algorithm ensures that the network always has a loop-free and efficient routing topology. It also enables the network to adapt quickly to changes in the topology, such as link failures or new devices joining the network

## ECMP (Equal Cost Multi-Path)

ECMP stands for Equal Cost Multi-Path, which is a routing technique used in computer networking to distribute traffic across multiple paths that have the same cost. ECMP is often used in conjunction with link-state routing protocols such as OSPF (Open Shortest Path First) and IS-IS (Intermediate System to Intermediate System).

When multiple paths are available with the same cost, ECMP allows the router to use all of them to distribute traffic among them evenly. For example, if there are three paths available with the same cost, ECMP would send one-third of the traffic through each path. This can improve network performance by increasing the available bandwidth and providing redundancy in case of link failures.

ECMP works by maintaining multiple next-hop addresses for a given destination in the router's routing table. When a packet arrives at the router, the router selects one of the available next-hop addresses based on a hashing algorithm that uses information from the packet header, such as the source and destination IP addresses. The router then forwards the packet to the selected next-hop address.

ECMP is widely used in data center networks to distribute traffic across multiple paths between servers and switches. It is also used in enterprise networks and service provider networks to improve network performance and provide redundancy. However, it requires careful configuration and monitoring to ensure that traffic is evenly distributed among the available paths and to detect and react to any link failures.