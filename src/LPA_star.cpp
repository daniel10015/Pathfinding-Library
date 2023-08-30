#include "LPA_star.h"
#include "../lib/BinaryHeap_LPAstar.hpp"
#include <unordered_map>
#include <vector>
#include <cmath>
#include <iomanip>
// shortcuts
using std::unordered_map;
using std::vector;

#include <iostream>

/*
  * Organized as follows:
  * 1. Initialize everything
  * 2. Run core function
  * 3. assist core functions
  * 4. Update pathGrid
*/

bool isFloatEqual(float a, float b)
{
  float errorUpper = 1e-3f;
  float errorLower = -1e-3f;
  if((a - b) < errorUpper && (a - b) > errorLower)
    return true;
  return false;
}

bool isFloatNotEqual(float a, float b)
{
  return !isFloatEqual(a, b);
}

// --- Initialize ---

LPA_star::LPA_star()
{
  // initialize private variables
  // init goal and start nodes
  Node* start = nullptr;
  Node* goal = nullptr;
  vector<vector<float>> fieldGrid;
  vector<vector<float>> pathGrid;
  unordered_map<Coordinates, Node*, Coordinate_Hash> nodes;
  BinaryHeap<Node*, CalculateKey> U;
}

void LPA_star::initializeFieldGrid(const vector<vector<float>>& grid)
{
  fieldGrid.resize(grid.size());
  for(int i=0; i<grid.size(); i++)
  {
    fieldGrid[i].resize(grid[i].size());
    for(int j=0; j<grid[i].size(); j++)
    {
      fieldGrid[i][j] = grid[i][j];
    }
  }
}

void LPA_star::initializePathGrid(const vector<vector<float>>& grid)
{
  pathGrid.resize(grid.size());
  for(int i=0; i<grid.size(); i++)
  {
    pathGrid[i].resize(grid[i].size(), 0.0f);
    for(int j=0; j<grid[i].size(); j++)
    {
      if(isFloatEqual(grid[i][j], float(INT32_MAX))) // wall
        pathGrid[i][j] = -1.0f;
    }
  }
}

// --- Run Core ---
bool LPA_star::run(const vector<vector<float>>& grid, vector<Coordinates>& path, const vector<int>& hunter, const vector<int>& target)
{
  bool running = true;
  bool change = true;
  int numberOfRuns = 0;
  vector<Node*> changedNodes;
  vector<Node*> adjNodes(8);
  int numberOfAdjNodes = 0;
  // init fieldGrid
  initializeFieldGrid(grid);
  initializePathGrid(grid);

  // init goal and start nodes
  goal = new Node;
  goal->coordinates = Coordinates{target[0], target[1]};
  goal->weight = fieldGrid[goal->coordinates.Y][goal->coordinates.X];

  start = new Node;
  start->rhs = 0.0f;
  start->coordinates = Coordinates{hunter[0], hunter[1]};
  start->h = heuristic(start->coordinates);
  start->weight = fieldGrid[start->coordinates.Y][start->coordinates.X];
  // put into relavent nodes 
  nodes[goal->coordinates] = goal;
  nodes[start->coordinates] = start;
  U.insert(start);
  while(running)
  {
    numberOfRuns++;
    if(numberOfRuns == 2)
      running = false;
    computeShortestPath();
    updateGrid();
    printGrid();
    // wait for change
    changeField(changedNodes);
    
    if(change)
    {
      change = false;
      for(int i=0; i<changedNodes.size(); i++)
      {
        // updates weight (for cost function)
        //changedNodes[i]->weight = fieldGrid[changedNodes[i]->coordinates.Y][changedNodes[i]->coordinates.X];
        // updates the vertex (g and rhs values)
        // recursively update nodes
        updateNodeValues(changedNodes[i]);

        // updateVertex(changedNodes[i]);
        // // update vertexes of adjacent nodes
        // numberOfAdjNodes = getSuccessors(changedNodes[i], adjNodes);
        // for(int j=0; j<numberOfAdjNodes; j++)
        //   updateVertex(adjNodes[j]);
      }
      changedNodes.clear();
    }
  }

}

void LPA_star::updateNodeValues(Node* node)
{
  updateVertex(node);
  vector<Node*> adjNodes;
  // update vertexes of adjacent nodes
  int numberOfAdjNodes = getActiveSuccessors(node, adjNodes);
  // remove node, allow for new value to populate its coordinate
  nodes.erase(node->coordinates); // erase from known nodes
  U.remove(node); // erase from priority queue (if applicable)
  delete node; // delete node pointer

  // else update nodes if existing adjacent nodes exist 
  for(int j=0; j<numberOfAdjNodes; j++)
  {
    updateVertex(adjNodes[j]);
    updateNodeValues(adjNodes[j]);
  }
}

void LPA_star::computeShortestPath()
{
  int count_node_expansions = 0;
  CalculateKey firstBigger;
  Node* node;
  vector<Node*> successors(8);
  int succSize = 0;
  std::cout << "Compute shortest path\n"; // DEBUG
  while(!U.isEmpty() && (firstBigger(goal, U.getTop()) || isFloatNotEqual(goal->rhs, goal->g)))
  {
    count_node_expansions++;
    successors.clear();
    succSize = 0;
    node = U.getTop();
    std::cout << "Loop coordinates/g/rhs: {" << node->coordinates.X << ", " << node->coordinates.Y << "}" << node->g << "/" << node->rhs << "\n"; // DEBUG
    U.pop();
    if(node->g > node->rhs) // overconsistent
    {
      std::cout << "Overconsistent\n"; // DEBUG
      node->g = node->rhs; // update g(node)
      // update successors
      succSize = getSuccessors(node, successors);
      for(int i=0; i<succSize; i++)
      {
        updateVertex(successors[i]);
      }
    }
    else // underconsistent (since node is for sure inconsistent)
    {
      std::cout << "Underconsistent\n"; // DEBUG
      node->g = (float)INT32_MAX;
      // update successors
      succSize = getSuccessors(node, successors);
      for(int i=0; i<succSize; i++)
      {
        updateVertex(successors[i]);
      }
    }
  }
  std::cout << "node expansions: " << count_node_expansions << std::endl;
}

// update vertex in accoradance to the paper
// update rhs-value 
// remove from priority queue, and put in priority queue if inconsistent
void LPA_star::updateVertex(Node* node)
{
  if(node != start)
    node->rhs = calculateMinRHS(node);
  if(U.exists(node))
  {
    std::cout << "-- REMOVED -- " << node->coordinates.X << ", " << node->coordinates.Y << std::endl; // DEBUG
    U.remove(node);
  }
  if(isFloatNotEqual(node->g, node->rhs))
  {
    U.insert(node);
    std::cout << "-- INSERTED -- " << node->coordinates.X << ", " << node->coordinates.Y << " at g/rhs" << node->g << "/" << node->rhs << std::endl; // DEBUG
  }
  else
    std::cout << "-- NOT INSERTED -- " << node->coordinates.X << ", " << node->coordinates.Y << " at g/rhs" << node->g << "/" << node->rhs << std::endl; // DEBUG
}

// --- Assist core functions ---

// - returns an estimated distance traveled
// from a (coordinates) to goal (coordinates)
float LPA_star::heuristic(const Coordinates& a)
{
  float dx = abs(goal->coordinates.X - a.X);
  float dy = abs(goal->coordinates.Y - a.Y);
  return (SQRT2 * (std::min(dx, dy)) + abs(dx - dy));
}

// - returns the actual cost from a to b
// using weights from fieldGrid and a function
// to determine actual distance traveled
float LPA_star::cost(const Node* a, const Node* b)
{
  // get cost from moving a to b using weights
  // assuming linear and adjacent relationship between weights across nodes
  float totalCost = b->weight + a->weight - 1.0f; // (b-a) gets the weight, -1 adjusts for offset
  if(a->coordinates.X - b->coordinates.X == a->coordinates.Y - b->coordinates.Y)
    totalCost *= SQRT2; // scale extra cost if diagonal movement
  return totalCost;
}

// - returns the rhs value defined in the LPA* paper
// - checks 8-adj cells (not specifically the pred)
// because it's still an O(n) time complexity
// and cost to implement feature may not
// result in faster rhs calculation
float LPA_star::calculateMinRHS(const Node* node)
{
  // go through neighbors and get what minimizes s->g + cost(s, node)
  auto tempNode = nodes.begin();
  float rhs = (float)INT32_MAX;
  for(int x=-1; x<=1; x++)
  {
      for(int y=-1; y<=1; y++)
      {
        if(x==0 && y== 0) // don't check current node
          continue;
        tempNode = nodes.find(Coordinates{ node->coordinates.X+x, node->coordinates.Y+y });
        if(tempNode != nodes.end() && (tempNode->second->g + cost(tempNode->second, node) < rhs))
          rhs = tempNode->second->g + cost(tempNode->second, node);
      }
  }
  std::cout << "final rhs: " << rhs << std::endl; // DEBUG
  return rhs;
}

// returns number of successors
// - In ideal scenario this function always returns 8
// however in case of walls this function will return
// the number of non-wall adjacent cells
// - Used as a helper for calculateMinRHS(Node*)
int LPA_star::getSuccessors(const Node* node, vector<Node*>& successors)
{
  int size = 0;
  Coordinates coor = node->coordinates;
  if(successors.size() != 8)
    successors.resize(8);
  for(int y=-1; y<=1; y++)
  {
    for(int x=-1; x<=1; x++)
    {
      // don't repeat coordinate
      if(y==0 && x==0)
        continue;
      if(coor.Y + y >= 0 && coor.Y + y < fieldGrid.size() && coor.X + x >= 0 && coor.X + x < fieldGrid[0].size() && fieldGrid[coor.Y + y][coor.X + x] != (float)INT32_MAX)
      {
        // Store new node if not previously relavent
        if(nodes.find(Coordinates{coor.X + x, coor.Y + y}) == nodes.end())
        {
          nodes[ Coordinates{coor.X + x, coor.Y + y} ] = new Node{
            (float)INT32_MAX, // g approx
            (float)INT32_MAX, // rhs informed approx (calculated on next line)
            heuristic(Coordinates{coor.X + x, coor.Y + y}), // heuristic
            Coordinates{coor.X + x, coor.Y + y}, // coordinates
            fieldGrid[coor.Y + y][coor.X + x]}; // weight
            nodes[ Coordinates{coor.X + x, coor.Y + y} ]->rhs = calculateMinRHS(nodes[ Coordinates{coor.X + x, coor.Y + y} ]);
        }
        successors[size] = nodes.at(Coordinates{coor.X + x, coor.Y + y});
        std::cout << "coordinate/heuristic/rhs of node: (" << coor.X + x << ", " << coor.Y + y << ") / " << nodes.at(Coordinates{coor.X + x, coor.Y + y})->h << "/" << nodes.at(Coordinates{coor.X + x, coor.Y + y})->rhs << std::endl; // DEBUG
        size++;
      }
    }
  }
  return size;
}

int LPA_star::getActiveSuccessors(const Node* node, vector<Node*>& successors)
{
  int size = 0;
  Coordinates coor = node->coordinates;
  if(successors.size() != 8)
    successors.resize(8);
  for(int y=-1; y<=1; y++)
  {
    for(int x=-1; x<=1; x++)
    {
      // don't repeat coordinate
      if(y==0 && x==0)
        continue;
      // checks for valid entry (not a wall and in-bounds)
      if(coor.Y + y >= 0 && coor.Y + y < fieldGrid.size() && coor.X + x >= 0 && coor.X + x < fieldGrid[0].size() && fieldGrid[coor.Y + y][coor.X + x] != (float)INT32_MAX)
      {
        // checks if node has been logged and is going towards goal to determine that it's a successor
        if(nodes.find(Coordinates{coor.X + x, coor.Y + y}) != nodes.end() && nodes.at(Coordinates{coor.X + x, coor.Y + y})->g > node->g)
        {
          successors[size] = nodes.at(Coordinates{coor.X + x, coor.Y + y});
          std::cout << "coordinate/heuristic/rhs of node: (" << coor.X + x << ", " << coor.Y + y << ") / " << nodes.at(Coordinates{coor.X + x, coor.Y + y})->h << "/" << nodes.at(Coordinates{coor.X + x, coor.Y + y})->rhs << std::endl; // DEBUG
          size++;
        }
      }
    }
  }
  return size;
}

// --- Updaters ---
void LPA_star::updateGrid()
{
  for(int i=0; i<pathGrid.size(); i++)
  {
    for(int j=0; j<pathGrid[i].size(); j++)
    {
      if(isFloatEqual(fieldGrid[i][j], INT32_MAX) || nodes.find(Coordinates{j, i}) == nodes.end())
        continue;
      pathGrid[i][j] = nodes.at(Coordinates{j, i})->g;
    }
  }
}

void LPA_star::printGrid()
{
  for(int i=0; i<pathGrid.size(); i++)
    {
      std::cout << std::setw(8);
      for(int j=0; j<pathGrid[i].size(); j++)
      {
        // DEBUG
        // if(Coordinates{j, i} == Coordinates{1, 5})
        // {
        //   std::cout << "(1, 5) g-val: " << nodes.at(Coordinates{1, 5})->g << std::endl;
        //   std::cout << pathGrid[i][j] << std::endl;
        // }

        if(isFloatEqual(fieldGrid[i][j], INT32_MAX))
          std::cout << -2 << " " << std::setw(8); // DEBUG wall
        else if(isFloatEqual(pathGrid[i][j], (float)INT32_MAX))
          std::cout << -1 << " " << std::setw(8); // DEBUG not expanded
        else
          std::cout << pathGrid[i][j] << " " << std::setw(8); // DEBUG g-val
      }
      std::cout << std::endl;
    }
}

void LPA_star::changeField(vector<Node*>& changedNodes)
{
  // make a predictable change
  Coordinates modded = Coordinates{1, 3}; // grid[3][1] = (float)INT32_MAX;
  fieldGrid[modded.Y][modded.X] = (float)INT32_MAX;
  if(nodes.find(modded) == nodes.end()) // make new node
  {
    nodes[modded] = new Node {
      (float)INT32_MAX, // g approx
      (float)INT32_MAX, // rhs informed approx (calculated on next line)
      heuristic(modded), // heuristic
      modded, // coordinates
      fieldGrid[modded.Y][modded.X] // weight
      };
  }
  else
  {
    nodes[modded]->weight = fieldGrid[modded.Y][modded.X];
  }
  changedNodes.push_back(nodes.at(modded));
}

// destructor
LPA_star::~LPA_star()
{
  for(auto iter = nodes.begin(); iter!=nodes.end(); iter++)
    delete iter->second;
}