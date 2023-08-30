#include <vector>
#include "A_star.h"
//#include "A_star.h"
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

int main(void)
{
    //something, maybe get a random grid going?
    //vector<vector<int>> grid(20, vector<int> (30));
    vector<vector<float>> grid;
    grid.resize(10);
    for(int i=0; i<10; i++)
    {
        grid[i].resize(10);
        for(int j=0; j<10; j++)
        {
            grid[i][j] = 1.0f;
        }
    }
    vector<int> hunter = { 0, 0 };
    vector<int> target = { 9, 9 };

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

    if(!A_star(grid, path, hunter, target))
    {
        cout << "Path not found" << endl;
    }
    else
    {
        cout << "Path found, coordinates: " << endl;
        for(int i=path.size()-1; i>=0; i--)
        {
            cout << "(" << path[i].X << "," << path[i].Y << "), ";
            grid[path[i].Y][path[i].X] = -1.0f;
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
                else if(grid[i][j] == -1.0f)
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