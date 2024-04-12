
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "search.h"

int matrix_main_test[N][N]={0};

//replace this code with something custom to read data once from a file.
void load_matrix()
{
    matrix_main_test[3][1] = 2;       // use 2 to denote obstacle
    matrix_main_test[3][0] = 2;
    matrix_main_test[4][0] = 2;
    matrix_main_test[4][1] = 2;
    matrix_main_test[9][0] = 2;
    matrix_main_test[5][4] = 2;
    matrix_main_test[5][3] = 2;
    matrix_main_test[4][4] = 2;
    matrix_main_test[4][5] = 2;
    matrix_main_test[4][3] = 2;
    matrix_main_test[4][6] = 2;
    matrix_main_test[4][7] = 2;
    matrix_main_test[6][1] = 2;
    matrix_main_test[6][4] = 2;
    matrix_main_test[6][5] = 2;
    matrix_main_test[10][1] = 2;
    matrix_main_test[0][98] = 2;
    matrix_main_test[1][98] = 2;
    matrix_main_test[1][99] = 2;
    matrix_main_test[1][100] = 2;
   // matrix_main_test[0][100] = 2; // uncomment this to check that code reacts to max step bound
}
    
int main()
{

    load_matrix();

    Cell start = {0, 0};
    int start_x=0; int start_y=0;
    int dest_x=0;int dest_y= 99;
    int path_length = 0;
    search_result *results = find_path(matrix_main_test,start_x,start_y, dest_x,dest_y, &path_length,1);//1 is return_forward only
    if (results != NULL && path_length > 0)
    {
        for (int i = 0; i < path_length; i++)
        {
            printf("%d:(%d,%d)\n", i, results[i].x, results[i].y);
        }
        free(results); // we can just return this results to the python side.
    }
}
