#include "LPA_star.h"

int main(void)
{
    LPA_star pathFinder;
    vector<vector<float>> grid;
    vector<Coordinates> path;
    grid.resize(6);
    for(int i=0; i<6; i++)
    {
        grid[i].resize(4);
        for(int j=0; j<4; j++)
        {
            grid[i][j] = 1.0f;
        }
    }
    grid[1][0] = (float)INT32_MAX;
    grid[2][0] = (float)INT32_MAX;
    grid[3][0] = (float)INT32_MAX;
    grid[4][0] = (float)INT32_MAX;
    
    grid[1][2] = (float)INT32_MAX;
    grid[2][2] = (float)INT32_MAX;
    grid[3][2] = (float)INT32_MAX;
    grid[4][2] = (float)INT32_MAX;




    vector<int> hunter = { 3, 0 };
    vector<int> target = { 0, 5 };
    pathFinder.run(grid, path, hunter, target);
    return 0;
}