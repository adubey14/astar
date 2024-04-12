#ifndef __SEARCH_H
#define __SEARCH_H

#define N 400
// making sure we dont run forever
#define MaxSteps 100000
typedef struct
{
    int x, y;            // Coordinates of the cell
    int f, g, h;         // Cost parameters for A* algorithm
    struct Cell *parent; // Parent cell
} Cell;

typedef struct
{
    int x;
    int y;
    /* data */
} search_result;

// A structure to hold the relevant data for the A* algorithm
typedef struct
{
    Cell *current;
    int length;
} Path;

typedef struct
{
    Cell *current;
    struct Allocated_cells *next;
} Allocated_cells;


search_result *find_path(int matrix[N][N],int start_x, int start_y, int dest_x, int dest_y, int *path_length,int RETURN_FORWARD_ONLY);
void load_matrix();
#endif