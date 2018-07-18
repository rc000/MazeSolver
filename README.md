# MazeSolver

Program creates maze by using Backtracking algorithm. Function CreatePassage is launched with 0,0 as indexes and
randomly looks for unvisited adjacent cell. If it finds, removes wall between those cells. Found cell becomes new current cell and
is marked as visited during creating and pushed on a stack. Function CreatePassage is called recursively with coordinates of current cell.
If current cell does not have unvisited neighbours, last visited cell is popped out of the stack and function is called with indexes 
of this cell. Maze is created when stack is empty.

![](https://github.com/rc000/MazeSolver/blob/master/Screenshots/DuringCreating.png)

![](https://github.com/rc000/MazeSolver/blob/master/Screenshots/Created.png)



