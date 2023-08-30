#include "A_star.h"
#include "../lib/BinaryHeap_Astar.hpp"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>

#include <iostream>
using std::cout;
using std::endl;

using std::vector;

class Coordinate_Hash
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


class Astar_Node_Compare
{
public:
	bool operator() (const Node* node0, const Node* node1) const
	{
        if (node0->f > node1->f)
			return true;
        else
		    return false;
	}
};

float square(float num)
{
    return num*num;
}

float getHeuristic(const Coordinates& current_node, const Coordinates& end)
{
    return sqrt(square(end.X - current_node.X) + square(end.Y - current_node.Y));
}

float getAdjDistance(const Node* current_node, const Coordinates& next_node, const float weight)
{
    float distance;
    // diagonal distance has same x/y abs(differential)  
    if(abs(current_node->coordinates.X - next_node.X) == abs(current_node->coordinates.Y - next_node.Y))
        distance = current_node->g + (1.4142857 * weight);
    else
        distance = current_node->g + weight;
    return distance;
}

bool A_star(const vector<vector<float>>& grid, vector<Coordinates>& path, const vector<int>& hunter, const vector<int>& target, int edges)
{
    const int height = grid.size();
    const int width = grid[0].size();

    BinaryHeap<Node*, Astar_Node_Compare> open_list;
    std::unordered_map<Coordinates, Node*, Coordinate_Hash> closed_list;
    std::unordered_map<Coordinates, Node*, Coordinate_Hash> known_nodes;

    // preallocate children
    vector<Coordinates> children_list;
    int child = 0;
    for(int i=0; i<edges; i++)
    {
        children_list.push_back(Coordinates {});
    }
    Coordinates temp_coor { hunter[0], hunter[1] };
    Node* curr_node = new Node{ nullptr, temp_coor, 0, 0, 0, 1 };
    open_list.insert(curr_node);
    known_nodes[temp_coor] = curr_node;

    bool searching = true;
    bool found = false;
    int node_expansions = 0;
    while(searching)
    {
        node_expansions++;
        curr_node = open_list.getTop();

        // cout << "(" << curr_node->coordinates.X << ", " << curr_node->coordinates.Y << ")\n";

        temp_coor = curr_node->coordinates;
        child = 0;
        open_list.pop();
        closed_list[curr_node->coordinates] = curr_node;

        if(curr_node->coordinates.X == target[0] && curr_node->coordinates.Y == target[1])
        {
            // cout << "path found!\n";
            searching = false;
            found = true;
            //backtrack then construct path
            while(curr_node != nullptr)
            {
                path.push_back(curr_node->coordinates);
                curr_node = curr_node->parent;
            }
            break;
        }
        // keep searching surrounding nodes
        else
        {
            //search surrounding nodes then grab any that are not walls (grid>0) or not in closed list
            for(int x=-1; x<=1; x++)
            {
                for(int y=-1; y<= 1; y++)
                {
                    //make sure coordinates are in bounds, not a wall, and not in closed list
                    if((closed_list.find(Coordinates { curr_node->coordinates.X + x, curr_node->coordinates.Y + y }) == closed_list.end()) && (curr_node->coordinates.X + x >= 0 && curr_node->coordinates.X + x < width) && (curr_node->coordinates.Y + y >= 0 && curr_node->coordinates.Y + y < height) && grid[curr_node->coordinates.Y + y][curr_node->coordinates.X + x] > 0.0f)
                    {
                        children_list[child] = Coordinates { temp_coor.X + x, temp_coor.Y + y };
                        // cout << "added child (" << children_list[child].X << ", " << children_list[child].Y << ")\n";
                        child++;
                    }
                }
            }
            // check the childs, if any
            if(child > 0)
            {
                //check if known, if so is it more efficient
                for(int i=0; i<child; i++)
                {
                    // HERE
                    if((known_nodes.find(children_list[i]) != known_nodes.end()) && (known_nodes[children_list[i]]->g < curr_node->g + getAdjDistance(curr_node, children_list[i], grid[children_list[i].Y][children_list[i].X])))
                    {
                        // error state (inefficient)
                        children_list[i] = Coordinates {-1, -1};
                    }
                }
                // if more efficient, modify node value in visited_node then delete and allocate with new data
                for(int i=0; i<child; i++)
                {
                    if(children_list[i].X == -1) // child is more inefficient than what's previously been found, so skip
                        continue;
                    else if((known_nodes.find(children_list[i]) == known_nodes.end()))
                    {
                        known_nodes[children_list[i]] = new Node { 
                            curr_node, // parent
                            children_list[i], // coordinates
                            // HERE
                            curr_node->g + getAdjDistance(curr_node, children_list[i], grid[children_list[i].Y][children_list[i].X]), // start->pos
                            getHeuristic(children_list[i], Coordinates{target[0], target[1]}), // heuristic
                            0, // func ( start->pos + heuristic(pos, goal) )
                            grid[children_list[i].Y][children_list[i].X]}; // weight

                        // add to priority queue for future searches
                        open_list.insert( known_nodes.at(children_list[i]) );
                        cout << "inserted node: (" << children_list[i].X << ", " << children_list[i].Y << ")\n";
                    }
                    else
                    {
                        // reset values in memory then tell PQ to change order if necessary
                        known_nodes[children_list[i]]->parent = curr_node;
                        known_nodes[children_list[i]]->g = curr_node->g + grid[children_list[i].Y][children_list[i].X];
                        known_nodes[children_list[i]]->f = known_nodes[children_list[i]]->g + known_nodes[children_list[i]]->h;
                        open_list.modifyKey( open_list.getHash().at(known_nodes[children_list[i]]) ); // makes necessary updates in priority queue, getting index from internal mapper (source changed specifically for A*)
                    }
                }
            }
        }
        if(open_list.isEmpty())
        {
            searching = false;
        }
    }
    cout << "node expansions: " << node_expansions << endl;

    // deallocates every node
    for(auto node=known_nodes.begin(); node!=known_nodes.end(); node++)
    {
        delete node->second;
    } 

    return found;
}