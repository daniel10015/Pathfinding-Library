# Single agent pathfinding 

- This repository contains implementations of popular pathfinding algorithms, which can be used stand-alone or 
can be used in the following main[ALGORITHM].cpp files which are always in verbose mode

- There's a focus on algorithms optimized for a robot navigating through unknown or changing terrain, such as [Dijkstra](#dijkstra) [A*](#a), and [LPA*](#lpa)

Below are explanations for Data Structures and Design/Structure choices (especially when taken from papers 
where limited source code is available). Less explanation will be given for dijkstra and A* since they're already
well documented.

NOTE: all source code is defaulted in verbose mode

## Introduction
- In Pathfinding/lib you will find 'BinaryHeap.hpp' which is my min heap implementation 
with *custom methods* to optimize its functionality in Pathfinding, while simplpifying 
the templated properties of the priority queue to its type and comparator. Other variations
of the BinaryHeap for different algorithms are also listed there
  - For Example: 'BinaryHeap_Astar.hpp' includes a map to access an element's index to optimize A*
    - A_star.cpp{166} Searching open list goes from O(n) to O(1)


## Usage
- Makefile is located in ~/build, make that your current directory then `make` to create object files and executables
- Run the executables for a given pathfinding algorithm
- (OPTIONAL) on use Bash script `clean.sh` to remove object files in `~/bin` and `~/build` 

## Dependencies
- Custom min heaps are in ~/lib which are required for `A_star.cpp` and `LPA_star.cpp` to compile correctly
- `BinaryHeapA_star.hpp` is required for `A_star.cpp` and `BinaryHeapLPA_star.hpp` is required for `LPA_star`.cpp


## Dijkstra
### 

## A*
### Data Structures 
- Created `Coordinate` struct used for holding coordinates in `Node`, particularly for hashing to a node
- Optimized openlist(node) lookup from O(n) to O(1) using custom a min heap (located in ~/lib/). 
- Used maps for `known_nodes` and `closed_list` since we need to keep track of nodes for a given coordinate

## LPA*
### source
- [Click here for the original paper](https://www.cs.cmu.edu/~maxim/files/aij04.pdf)
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
- Optimizations can be made such that pred/succ searches can be made with same assumption used for outlining 
shortest path on terrain after search is complete such as `cost + heuristic(next_node)`
  - This is what `getActiveSuccessors LPA_star.cpp{308}` does

## D* lite
### TODO