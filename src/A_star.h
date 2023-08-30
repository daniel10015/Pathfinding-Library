#pragma once
#include <vector>
#include <stdint.h>
using std::vector;


// coordinates begin in error state (-1, -1)
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

struct Node 
{
    Node* parent;
    Coordinates coordinates;
    float g = (float)INT32_MAX; // actual distance from start
    float h = (float)INT32_MAX; // estimated distance to goal
    float f = (float)INT32_MAX; // f = g + h
    float weight = 1;
};

bool A_star(const vector<vector<float>>& grid, vector<Coordinates>& path, const vector<int>& hunter, const vector<int>& target, int edges = 8);