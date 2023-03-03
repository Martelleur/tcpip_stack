# Notes IGP: Interior Gateway Protocol

IGP is routing protocols used by routers to exchange routing information within a single Autonomous System (AS).

## Autonomous System (AS)
An Autonomous System (AS) is a collection of interconnected networks that operate under a common administrative domain. Within an AS, routers use IGP protocols to share information about the topology of the network and the routing paths between nodes. This information is used to determine the most efficient path for forwarding packets within the network.

## Examples IGP protocols:

__Routing Information Protocol (RIP):__ 
A simple distance-vector protocol that uses hop count as a metric to determine the best path between nodes.

__Open Shortest Path First (OSPF):__
A link-state protocol that uses a more complex metric based on the cost of the links between nodes.

__Intermediate System to Intermediate System(IS-IS):__
A link-state protocol similar to OSPF, but used primarily in larger networks.


## Routing in multiple Autonomous System (AS)
IGP protocols are designed for use within a single AS, and are not typically used for routing between multiple ASes. For that purpose, Border Gateway Protocol (BGP), an exterior gateway protocol (EGP), is commonly used.