#pragma once
#include <vector>
#include <stdint.h>
#include <unordered_map>
using std::vector;
using std::unordered_map;
#define SQRT2 1.4142135f

bool isFloatEqual(float a, float b);

bool isFloatNotEqual(float a, float b);

struct Coordinates
{
    int X = -1;
    int Y = -1;
public:
    bool operator==(const Coordinates& b) const
    {
        if(this->X == b.X && this->Y == b.Y)
            return true;
        else
            return false;
    }
};

// Hash function for Coordinates
// Used for map which tracks relavent coordinates->nodes
class Coordinate_Hash
{
public:
	size_t operator()( const Coordinates& key ) const
	{
        //unsigned int
		size_t hashVal = 0;
        if(key.X >= 0 && key.Y >= 0)
		    hashVal = 0.5 * (key.X + key.Y) * (key.X + key.Y + 1) + key.X; // will always result in integer

		return hashVal;
	}
};

struct Node
{
    float g = (float)INT32_MAX; // estimated dist to start
    float rhs = (float)INT32_MAX; // (potentially more informed) dist to start  
    float h = 0.0f; // heuristic
    Coordinates coordinates; // location on 2-D grid
    float weight = 1;
};

// Compares keys between two nodes
// Used in priority queue
class CalculateKey
{
public:
    bool operator()(const Node* a, const Node* b) 
    {
        bool isBigger = false;
        if(std::min(a->g, a->rhs) + a->h > std::min(b->g, b->rhs) + b->h) // 1st component of vector
            isBigger = true;
        else if((std::min(a->g, a->rhs) + a->h == std::min(b->g, b->rhs) + b->h) && (std::min(a->g, a->rhs) > std::min(b->g, b->rhs))) // 2nd component of vector
            isBigger = true;
        return isBigger;
    }
};


#include "../lib/BinaryHeap_LPAstar.hpp"
class LPA_star
{
// private variables
private:
    Node* start;
    Node* goal;
    vector<vector<float>> fieldGrid;
    vector<vector<float>> pathGrid;
    unordered_map<Coordinates, Node*, Coordinate_Hash> nodes;
    BinaryHeap<Node*, CalculateKey> U;


// public methods 
public:
    LPA_star();
    bool run(const vector<vector<float>>& grid, vector<Coordinates>& path, const vector<int>& hunter, const vector<int>& target);
    ~LPA_star(); // deallocates every heap node stored in nodes

// private methods to modularize the code 
private:
    float heuristic(const Coordinates& a);
    float cost(const Node* a, const Node* b);
    float calculateMinRHS(const Node* node);
    void updateVertex(Node* vertex);
    int getSuccessors(const Node* node, vector<Node*>& successors);
    int getActiveSuccessors(const Node* node, vector<Node*>& successors);
    void computeShortestPath();
    void initializeFieldGrid(const vector<vector<float>>& grid);
    void initializePathGrid(const vector<vector<float>>& grid);
    void updateGrid();
    void printGrid();
    void changeField(vector<Node*>& changedNodes);
    void updateNodeValues(Node* node);
};