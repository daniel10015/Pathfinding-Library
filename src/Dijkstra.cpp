#include <vector>
#include <queue>
#include <unordered_map>
#include <set>
#include "Dijkstra.h"

#include <iostream>

using std::vector;

class Node_Hash
{
public:
	size_t operator()( const Coordinates& key ) const
	{
        //unsigned int
		size_t hashVal = 0;
        if(key.X >= 0 && key.Y >= 0)
		    hashVal = 0.5 * (key.X + key.Y) * (key.X + key.Y + 1) + key.X; //will always result in integer

		return hashVal;
	}
};


class Dijkstra_Node_Compare
{
public:
	bool operator() (const Node* node0, const Node* node1) const
	{
		if (node0->distance > node1->distance)
		{
			return true;
		}
		return false;
	}
};

float getAdjDistance(const Node* current_node, const Coordinates& next_node, const float weight)
{
    float distance;
    // diagonal distance has same x/y abs(differential)  
    if(abs(current_node->coordinates.X - next_node.X) == abs(current_node->coordinates.Y - next_node.Y))
        distance = current_node->distance + (1.4142857 * weight);
    else
        distance = current_node->distance + weight;
    return distance;
}


// returns true if path is constructed, if unable to construct path then false
// second parameter should be empty (the function will clear all contents, if any are present)
// classic dikjstra's algorithm: searches the 8 boxes around hunter until target is found 
// similar to BFS, instead of storing next nodes in queue, store in low-bound PQ because weighted graph 
// Path is built out of custom Coordinate struct (in dijkstra.h)
bool dijkstra(const vector<vector<float>>& grid, vector<Coordinates>& path, const vector<int>& hunter, const vector<int>& target)
{
    bool searching = true;
    bool found = false;
    // parameters for creating a low-bound PQ
    std::priority_queue <Node*, vector<Node*>, Dijkstra_Node_Compare> next_search;
    std::unordered_map< Coordinates, Node*, Node_Hash> unvisited;
    std::unordered_map< Coordinates, Node*, Node_Hash> visited;


    const int height = grid.size();
    const int width = grid[0].size();
    vector<int> temp_mover;

    std::cout << "width/height" << width << '/' << height << '\n';

    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            std::cout << "(" << i << ", " << j << "), ";
            unvisited[Coordinates {i, j} ] = new Node{ nullptr, (float)INT32_MAX, Coordinates {i, j}, grid[j][i]};
        }
        std::cout << '\n';
    }

    if(unvisited.find(Coordinates{1,0}) == unvisited.end())
        std::cout << "fail" << std::endl;
    else
        std::cout << "pass" << std::endl;

    // Temp variable for searching
    Node* curr_node;
    Coordinates temp_coor;
    // Temp variable 
    Node* next = unvisited.at( Coordinates {hunter[0] , hunter[1]} );
    next->distance = 0;
    // make sure unordered_map hash is working
    next_search.push( next );

    int node_expansions = 0;
    while(searching)
    {
        node_expansions++;
        //get next node to search 
        curr_node = next_search.top();
        visited[curr_node->coordinates] = curr_node;
        next_search.pop();

        std::cout << "pop (" << curr_node->coordinates.X<< ", " << curr_node->coordinates.Y<< ")\n";

        //search unvisited adjacent nodes
        for(int x=-1; x<=1; x++)
        {
            for(int y=-1; y<= 1; y++)
            {
                //make sure coordinates are in bounds
                // std::cout << "(" << curr_node->coordinates.X + x << ", " << curr_node->coordinates.Y + y << ")\n";
                if((curr_node->coordinates.X + x >= 0 && curr_node->coordinates.X + x < width) && (curr_node->coordinates.Y + y >= 0 && curr_node->coordinates.Y + y < height) && (grid[curr_node->coordinates.Y + y][curr_node->coordinates.X + x] > 0))
                {
                    temp_coor = Coordinates{curr_node->coordinates.X + x, curr_node->coordinates.Y + y};
                    //checks if node has been visited
                    // std::cout << "check visited\n";
                    if(visited.find(temp_coor) == visited.end())
                    {
                        // if curr_node has more efficient pathway then set new prev_node
                        // std::cout << "is it efficient\n";
                        if(unvisited.at(temp_coor)->distance > curr_node->distance + getAdjDistance(curr_node, temp_coor, grid[temp_coor.Y][temp_coor.X]))
                        {   
                            unvisited.at(temp_coor)->distance = curr_node->distance + getAdjDistance(curr_node, temp_coor, grid[temp_coor.Y][temp_coor.X]);
                            unvisited.at(temp_coor)->prev = curr_node;
                            next_search.push(unvisited.at(temp_coor)); //repush so PQ will adjust
                            // checks for goal node
                            // std::cout << "is it the goal\n";
                            if(temp_coor.X == target[0] && temp_coor.Y == target[1])
                            {
                                std::cout << "it's the goal node!\n";
                                searching = false;
                                found = true;
                                break; // stop
                            }
                        }
                    }
                }

            }
        }

        // if no nodes are available for searching then stop the algorithm
        if(next_search.empty())
        {
            searching = false;
        }
    }
    std::cout << "node expansions: " << node_expansions << std::endl;

    if(found)
    {
        curr_node = unvisited.at( Coordinates {target[0], target[1]} );
        while(curr_node != nullptr)
        {
            path.push_back(curr_node->coordinates);
            curr_node = curr_node->prev;
        }
    }

    // deallocate every node
    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            temp_coor = Coordinates { i, j };
            if(unvisited.find(temp_coor) != unvisited.end())
            {
                delete unvisited.at(temp_coor);
            }
            else
            {
                delete visited.at(temp_coor);
            }
        }
    }

    return found;
}