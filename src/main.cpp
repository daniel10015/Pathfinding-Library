#include <vector>
#include "Dijkstra.h"
//#include "A_star.h"
#include <iostream>
#include <cstdlib>
#include <time.h>

using std::cout;
using std::endl;
using std::vector;

void randomizeGrid(vector<vector<float>>& grid)
{
    srand(time(0));
    grid.resize(20);
    for(int i=0; i<20; i++)
    {
        grid[i].resize(30);
        for(int j=0; j<30; j++)
        {
            float value = float( rand()%10 );
            if(value > 8)
            {
                value = -1;
                cout << "/ ";
            }
            else
                cout << value << " ";
            grid[i][j] = value;
        }
        cout << endl;
    }
    cout << "---------------\n\n";
}

int main(void)
{
    //something, maybe get a random grid going?
    //vector<vector<int>> grid(20, vector<int> (30));
    vector<vector<float>> grid;

    randomizeGrid(grid);
    // grid.resize(20);
    // for(int i=0; i<20; i++)
    // {
    //     grid[i].resize(30);
    //     for(int j=0; j<30; j++)
    //     {
    //         grid[i][j] = 1.0f;
    //     }
    // }
    vector<int> hunter = { 0, 0 };
    vector<int> target = { 29, 19 };

    //print grid
    for(int i=0; i<(int)grid.size(); i++)
    {
        for(int j=0; j<(int)(grid[i].size()); j++)
        {
            if(i == hunter[0] && j == hunter[1])
            {
                cout << "@ ";
            }
            else if(j == target[0] && i == target[1])
            {
                cout << "$ ";
            }
            else
            {
                cout << grid[i][j] << " ";
            }
        }
        cout << endl;
    }

    vector<Coordinates> path;

    if(!dijkstra(grid, path, hunter, target))
    {
        cout << "Path not found" << endl;
    }
    else
    {
        cout << "Path found, coordinates: " << endl;
        for(int i=path.size()-1; i>=0; i--)
        {
            cout << "(" << path[i].X << "," << path[i].Y << "), ";
            grid[path[i].Y][path[i].X] = -2.0f;
        }
        cout << endl;

        for(int i=0; i<(int)grid.size(); i++)
        {
            for(int j=0; j<(int)(grid[i].size()); j++)
            {
                if(j == hunter[0] && i == hunter[1])
                {
                    cout << "@ ";
                }
                else if(j == target[0] && i == target[1])
                {
                    cout << "$ ";
                }
                else if(grid[i][j] == -2.0f)
                {
                    cout << "* ";
                }
                else
                    cout << "0 ";
            }
            cout << endl;
        }

    }

    return 0;
}