# MazeSolver

Program creates maze by using backtracking algorithm. Function CreatePassage is launched with 0,0 as indexes and
randomly looks for unvisited adjacent cell. If it finds, removes wall between those cells. Found cell becomes new current cell and
is marked as visited during creating and pushed on a stack. Function CreatePassage is called recursively with indexes of current cell.
If current cell does not have unvisited neighbours, last visited cell is popped out of the stack and function is called with indexes 
of this cell. Maze is created when stack is empty.

![](https://github.com/rc000/MazeSolver/blob/master/Screenshots/DuringCreating.png)

![](https://github.com/rc000/MazeSolver/blob/master/Screenshots/Created.png)


Function ResolveMaze starts from entrance  to the maze. Main action takes place in while loop which is working until exit cell is reached.
Algorithm which I used in this solution is based on graph traversal algorithms. It always looks for unvisited cell in this order:RIGHT,UP,DOWN,LEFT. Visited cell is pushed on the stack like as during creating and indexes of current cell change properly. Color of cell which belongs to passage changes to blue. If current cell does not have unvisited neighbours, cells are popped out of stack until one of them satisfies condition. Visited cell which does not belong to passage is marked with black, small point.


![](https://github.com/rc000/MazeSolver/blob/master/Screenshots/DuringResolving.png)

![](https://github.com/rc000/MazeSolver/blob/master/Screenshots/Resolved.png)

