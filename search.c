#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define N 400

// Structure to represent a cell in the matrix
typedef struct
{
    int x, y;            // Coordinates of the cell
    int f, g, h;         // Cost parameters for A* algorithm
    struct Cell *parent; // Parent cell
} Cell;

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

Allocated_cells *allocated_cell_head = NULL;
Allocated_cells *allocated_cell_current = NULL;

Cell *allocate_cell()
{
    Cell *mycell = (Cell *)malloc(sizeof(Cell));
    Allocated_cells *alloccells = (Allocated_cells *)malloc(sizeof(Allocated_cells));
    alloccells->current = mycell;
    alloccells->next = NULL;
    if (allocated_cell_head == NULL)
    {
        allocated_cell_head = alloccells;
        allocated_cell_current = allocated_cell_head;
    }
    else
    {
        allocated_cell_current->next = alloccells;
        allocated_cell_current = alloccells;
    }
    return mycell;
}

void free_allocated_cells()
{
    if (allocated_cell_head == NULL)
        return;

    while (allocated_cell_head != NULL)
    {
        Allocated_cells *temp = allocated_cell_head;
        allocated_cell_head = allocated_cell_head->next;
        if (temp->current != NULL)
        {
            free(temp->current);
        }
        free(temp);
    }
}

// Function to check if a cell is valid or not (within the matrix boundaries and not an obstacle)
bool isValid(int row, int col, int matrix[N][N], bool visited[N][N])
{
    return (row >= 0) && (row < N) && (col >= 0) && (col < N) && (matrix[row][col] == 0) && (!visited[row][col]);
}

// Function to check if a cell is the destination
bool isDestination(int row, int col, Cell *dest)
{
    return (row == dest->x) && (col == dest->y);
}

// Function to calculate the heuristic value (Euclidean distance) between two cells
double calculateHValue(int row, int col, Cell *dest)
{
    return sqrt((row - dest->x) * (row - dest->x) + (col - dest->y) * (col - dest->y));
}

// Function to reconstruct the path from origin to destination
Path *reconstructPath(Cell *start, Cell *end)
{
    Path *path = (Path *)malloc(sizeof(Path));
    path->length = 0;
    path->current = end;

    Cell *current = end;
    while (current != start)
    {
        path->length++;
        current = current->parent;
    }

    path->length++; // Add the start cell to the length
    path->current = end;
    return path;
}

// Function to perform A* search algorithm
Path *aStarSearch(int matrix[N][N], Cell *start, Cell *dest)
{
    bool visitedForward[N][N] = {false};
    bool visitedBackward[N][N] = {false};

    start->f = 0;
    start->g = 0;
    start->h = 0;
    start->parent = NULL;

    dest->f = 0;
    dest->g = 0;
    dest->h = 0;
    dest->parent = NULL;

    while (!isDestination(start->x, start->y, dest) && !isDestination(dest->x, dest->y, start))
    {
        // Forward A* search
        // Start with the start cell
        Cell *currentForward = start;

        // Initialize the forward open list with the start cell
        Cell *openListForward[N * N];
        int openListForwardSize = 1;
        openListForward[0] = start;

        // Backward A* search
        // Start with the destination cell
        Cell *currentBackward = dest;

        // Initialize the backward open list with the destination cell
        Cell *openListBackward[N * N];
        int openListBackwardSize = 1;
        openListBackward[0] = dest;

        // Loop until a path is found or both searches have nowhere else to go
        while (openListForwardSize > 0 && openListBackwardSize > 0)
        {
            // Forward A* search
            // Choose the cell with the lowest f value from the forward open list
            currentForward = openListForward[0];
            for (int i = 1; i < openListForwardSize; ++i)
            {
                if (openListForward[i]->f < currentForward->f)
                    currentForward = openListForward[i];
            }

            // Remove the current cell from the open list
            for (int i = 0; i < openListForwardSize; ++i)
            {
                if (openListForward[i] == currentForward)
                {
                    openListForward[i] = openListForward[openListForwardSize - 1];
                    openListForwardSize--;
                    break;
                }
            }

            // Mark the current cell as visited in the forward direction
            visitedForward[currentForward->x][currentForward->y] = true;

            // Backward A* search
            // Choose the cell with the lowest f value from the backward open list
            currentBackward = openListBackward[0];
            for (int i = 1; i < openListBackwardSize; ++i)
            {
                if (openListBackward[i]->f < currentBackward->f)
                    currentBackward = openListBackward[i];
            }

            // Remove the current cell from the open list
            for (int i = 0; i < openListBackwardSize; ++i)
            {
                if (openListBackward[i] == currentBackward)
                {
                    openListBackward[i] = openListBackward[openListBackwardSize - 1];
                    openListBackwardSize--;
                    break;
                }
            }

            // Mark the current cell as visited in the backward direction
            visitedBackward[currentBackward->x][currentBackward->y] = true;

            // Check if we've reached the destination from the forward or backward search
            if (visitedForward[currentBackward->x][currentBackward->y] || visitedBackward[currentForward->x][currentForward->y])
            {
                // If so, reconstruct and return the path
                Path *forwardPath = reconstructPath(start, currentForward);
                // printf("%d;%d\n",forwardPath->current->x,forwardPath->current->y);
                Path *backwardPath = reconstructPath(dest, currentBackward);
                // printf("%d;%d\n",backwardPath->current->x,backwardPath->current->y);

                Cell *prev = backwardPath->current;

                Cell *current = forwardPath->current;
                prev = NULL;
                while (current != NULL)
                {
                    Cell *next = current->parent;
                    current->parent = prev;
                    prev = current;
                    current = next;
                }
                forwardPath->current = prev;
                prev = forwardPath->current;
                bool trigger = true;
                while (prev != NULL)
                {
                    if (prev->parent == NULL && trigger)
                    {
                        Cell *temp = backwardPath->current;
                        while (temp->x == prev->x && temp->y == prev->y)
                        {
                            temp = temp->parent;
                            backwardPath->length = backwardPath->length - 1;
                        }

                        prev->parent = temp;
                        trigger = false;
                    }
                    prev = prev->parent;
                }

                // prev = forwardPath->current;
                // if (prev->x==backwardPath->current->x && prev->y==backwardPath->current->y){
                //     prev=prev->parent;
                // }

                // Cell* current = prev->parent;
                // while (current != NULL) {
                //     current->parent = prev;
                //     prev = current;
                //     current = current->parent;
                // }
                // forwardPath->current->parent = prev;
                forwardPath->length = forwardPath->length + backwardPath->length;
                free(backwardPath);
                return forwardPath;
            }

            // Explore neighbors in the forward direction
            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    if (i == 0 && j == 0)
                        continue;

                    int row = currentForward->x + i;
                    int col = currentForward->y + j;

                    if (isValid(row, col, matrix, visitedForward))
                    {
                        double gNew = currentForward->g + 1.0; // Cost to move to the neighbor is 1
                        double hNew = calculateHValue(row, col, dest);
                        double fNew = gNew + hNew;

                        if (matrix[row][col] != 2)
                        { // Check for obstacle
                            Cell *neighbor = allocate_cell();
                            neighbor->x = row;
                            neighbor->y = col;
                            neighbor->f = fNew;
                            neighbor->g = gNew;
                            neighbor->h = hNew;
                            neighbor->parent = currentForward;

                            // Add the neighbor to the open list
                            openListForward[openListForwardSize++] = neighbor;
                        }
                    }
                }
            }

            // Explore neighbors in the backward direction
            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    if (i == 0 && j == 0)
                        continue;

                    int row = currentBackward->x + i;
                    int col = currentBackward->y + j;

                    if (isValid(row, col, matrix, visitedBackward))
                    {
                        double gNew = currentBackward->g + 1.0; // Cost to move to the neighbor is 1
                        double hNew = calculateHValue(row, col, start);
                        double fNew = gNew + hNew;

                        if (matrix[row][col] != 2)
                        { // Check for obstacle
                            Cell *neighbor = allocate_cell();
                            neighbor->x = row;
                            neighbor->y = col;
                            neighbor->f = fNew;
                            neighbor->g = gNew;
                            neighbor->h = hNew;
                            neighbor->parent = currentBackward;

                            // Add the neighbor to the open list
                            openListBackward[openListBackwardSize++] = neighbor;
                        }
                    }
                }
            }
        }
    }

    return NULL; // No path found
}

// Function to print the path
void printPath(Path *path)
{
    Cell *current = path->current;
    Cell *prev = NULL;
    printf("Path: ");
    while (current != NULL)
    {
        printf("(%d,%d) ", current->x, current->y);
        prev = current;
        current = current->parent;
    }
    printf("\nLength: %d\n", path->length);
}

int main()
{
    int matrix[N][N] = {0}; // Initialize matrix with all zeroes (no obstacles)
    matrix[3][1] = 2;
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
    Cell start = {0, 0};
    Cell dest = {4, 4}; // Set the destination to the bottom-right corner

    Path *path = aStarSearch(matrix, &start, &dest);
     if (path != NULL)
    {
        printPath(path);
        free_allocated_cells();
        free(path);

        //  freePath(path);
    }
    else
    {
        printf("No path found!\n");
    }
    path = aStarSearch(matrix, &start, &dest);
    if (path != NULL)
    {
        printPath(path);
        free_allocated_cells();
        free(path);
    }
    else
    {
        printf("No path found!\n");
    }

    return 0;
}
