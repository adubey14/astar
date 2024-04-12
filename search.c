#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "search.h"

// Structure to represent a cell in the matrix

Cell *allocate_cell(Allocated_cells **allocated_cell_head_ptr, Allocated_cells **allocated_cell_current_ptr)
{
    Allocated_cells *allocated_cell_head = *allocated_cell_head_ptr;
    Allocated_cells *allocated_cell_current = *allocated_cell_current_ptr;
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

void free_allocated_cells(Allocated_cells *allocated_cell_head)
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
Path *biDirectionalAstar(int matrix[N][N], Cell *start, Cell *dest, Allocated_cells **allocated_cell_head_ptr, Allocated_cells **allocated_cell_current_ptr, int RETURN_FORWARD_ONLY)
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

    int numberOfSteps = 0;

    while (!isDestination(start->x, start->y, dest) && !isDestination(dest->x, dest->y, start))
    {
        // Forward A* search
        // Start with the start cell
        Cell *currentForward = start;
        if (numberOfSteps > MaxSteps)
        {
            return NULL;
        }

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

            if (numberOfSteps > MaxSteps)
            {
                return NULL;
            }

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
                Path *backwardPath = reconstructPath(dest, currentBackward);
                Cell *prev = backwardPath->current;
                Cell *current = forwardPath->current;
                prev = NULL;
                // reverse the path for forward
                while (current != NULL)
                {
                    Cell *next = current->parent;
                    current->parent = prev;
                    prev = current;
                    current = next;
                }
                forwardPath->current = prev;
                prev = forwardPath->current;
                // if both paths are same because they both found the full answer then return one.
                if ((prev->x == backwardPath->current->x) && (prev->y == backwardPath->current->y) || RETURN_FORWARD_ONLY)
                {
                    free(backwardPath);
                    //printf("found origin on both forward and backward");
                    return forwardPath;
                }

                // we have to merge them.
                // bool trigger = true;
                //printf("merging forward and backward");
                while (prev != NULL)
                {

                    if (prev->parent == NULL)
                    {
                        Cell *temp = backwardPath->current;
                        while (temp != NULL && (temp->x == prev->x && temp->y == prev->y))
                        {
                            temp = temp->parent;
                            backwardPath->length = backwardPath->length - 1;
                        }

                        prev->parent = temp;
                        break;
                    }
                    prev = prev->parent;
                }
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
                            Cell *neighbor = allocate_cell(allocated_cell_head_ptr, allocated_cell_current_ptr);
                            neighbor->x = row;
                            neighbor->y = col;
                            neighbor->f = fNew;
                            neighbor->g = gNew;
                            neighbor->h = hNew;
                            neighbor->parent = currentForward;

                            // Add the neighbor to the open list
                            openListForward[openListForwardSize++] = neighbor;
                            numberOfSteps = numberOfSteps + 1;
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
                            Cell *neighbor = allocate_cell(allocated_cell_head_ptr, allocated_cell_current_ptr);
                            neighbor->x = row;
                            neighbor->y = col;
                            neighbor->f = fNew;
                            neighbor->g = gNew;
                            neighbor->h = hNew;
                            neighbor->parent = currentBackward;

                            // Add the neighbor to the open list
                            openListBackward[openListBackwardSize++] = neighbor;
                            numberOfSteps = numberOfSteps + 1;
                        }
                    }
                }
            }
        }
    }

    return NULL; // No path found
}

// Function to print the path
search_result *convertPathToResultArray(Path *path)
{
    search_result *results = NULL;
    if (path == NULL)
        return results;
    Cell *current = path->current;
    Cell *prev = NULL;
    //printf("Path: ");
    results = (search_result *)malloc(sizeof(search_result) * (path->length));
    int count = 0;
    while (current != NULL)
    {
        //printf("(%d,%d) ", current->x, current->y);
        results[count].x = current->x;
        results[count].y = current->y;
        count = count + 1;
        prev = current;
        current = current->parent;
    }
    printf("\nLength: %d\n", path->length);
    return results;
}

search_result *find_path(int matrix[N][N], int start_x, int start_y, int dest_x, int dest_y, int *path_length,int RETURN_FORWARD_ONLY)
{
    
    if (start_x >= 0 && start_y < N && dest_x >= 0 && dest_y < N)
    {
        Cell start;
        Cell dest;
        start.x=start_x;
        start.y=start_y;
        dest.x=dest_x;
        dest.y=dest_y;
        Allocated_cells *allocated_cell_head = NULL;
        Allocated_cells *allocated_cell_current = NULL;
        if (path_length != NULL)
            *path_length = 0;

        Path *path = biDirectionalAstar(matrix, &start, &dest, &allocated_cell_head, &allocated_cell_current,RETURN_FORWARD_ONLY);
        if (path != NULL)
        {
            search_result *results = convertPathToResultArray(path);
            *path_length = path->length;

            free_allocated_cells(allocated_cell_head);
            free(path);
            return results;
        }
        else
        {
            free_allocated_cells(allocated_cell_head);
            printf("No path found!\n");
            return NULL;
        }
    }
    else
    {
        // invalid configuration
        printf("invalid start or dest. Check ranges please. \n");
        return NULL;
    }
}


