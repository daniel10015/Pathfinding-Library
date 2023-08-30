# Pathfinding Visualizer

This repository contains implementations of popular pathfinding algorithms, which can be used stand-alone or (working on this) visualized and tested using Qt.

There's a focus on algorithms optimized for a robot navigating through unknown or changing terrain

Below are explanations for Data Structures and Design/Structure choices (especially when taken from papers 
where limited source code is available) 

## Introduction
- In Pathfinding/lib you will find 'BinaryHeap.hpp' which is my min heap implementation 
with *custom methods* to optimize its functionality in Pathfinding, while simplpifying 
the templated properties of the priority queue to its type and comparator.
  - For Example: 'BinaryHeap_Astar.hpp' includes a map to access an element's index to optimize A*
    - A_star.cpp{166} Searching open list goes from O(n) to O(1) 

## Dijkstra
### TODO

## A*
### TODO

## LPA*
### Data Structures and Design
- To ensure that memory and space time complexities in my implementation are as close as possible
to what's discussed in the paper, I will explicitly do the following
- Only expand nodes that are relevant using Map<Coordinates, Node*>
- Using class structure for readable function names (headers will describe its functionality)
### Grids
- fieldGrid represents the grid to be processed and calculated to compute the shortest path
  - Walls are negative values
  - floating point values represent the weights (costs are *relative*) so 4->5 is cheaper than 1->4
- pathGrid represents the grid used to find the shortest path by minimizing costs (ties can be arbitrarily broken)
  - Walls are negative values
  - floating point values represent the g-values of the node corresponding to the coordinate
### Expanding a node
- Every pred and succ are the 8-adjacent cells since it's still O(n) time, it won't affect the priority queue
ordering and it won't disrupt the node computations, while maintaining code simplicity

## D* lite
### TODO