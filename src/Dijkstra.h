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
    Node* prev = nullptr;
    float distance = (float)INT32_MAX; // infinite distance (assumed unreachable)
    Coordinates coordinates; // X, Y
    float weight = (float)1; // 1 is no weight, negative is wall, INT32_MAX is max drag but still transversible
};

bool dijkstra(const vector<vector<float>>& grid, vector<Coordinates>& path, const vector<int>& hunter, const vector<int>& target);