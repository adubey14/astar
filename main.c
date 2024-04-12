
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "search.h"

int main()
{

    int matrix[N][N] = {0}; // Initialize matrix with all zeroes (no obstacles)
    matrix[3][1] = 2;       // use 2 to denote obstacle
    matrix[3][0] = 2;
    matrix[4][0] = 2;
    matrix[4][1] = 2;
    matrix[9][0] = 2;
    matrix[5][4] = 2;
    matrix[5][3] = 2;
    matrix[4][4] = 2;
    matrix[4][5] = 2;
    matrix[4][3] = 2;
    matrix[4][6] = 2;
    matrix[4][7] = 2;
    matrix[6][1] = 2;
    matrix[6][4] = 2;
    matrix[6][5] = 2;
    matrix[10][1] = 2;
    matrix[0][98] = 2;
    matrix[1][98] = 2;
    matrix[1][99] = 2;
    matrix[1][100] = 2;
    //matrix[0][100] = 2; // uncomment this to check that code reacts to max step bound

    Cell start = {0, 0};
    Cell dest = {0, 99};
    int path_length = 0;
    search_result *results = find_path(matrix, start, dest, &path_length);
    if (results != NULL && path_length > 0)
    {
        for (int i = 0; i < path_length; i++)
        {
            printf("%d:(%d,%d)\n", i, results[i].x, results[i].y);
        }
        free(results); // we can just return this results to the python side.
    }
}
