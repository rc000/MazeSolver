#include "Maze.h"
#include<iostream>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include<thread>
#include<Windows.h>
#include<vector>

Maze::Maze() :widthOfGrid(40), heightOfGrid(40), start{ 0,0 }, end{ 0,0 },direction(Direction::NONE)
{

	grid.reserve(heightOfGrid);
	for (size_t i = 0; i < heightOfGrid; ++i) {
		grid.emplace_back(std::vector<Node>(widthOfGrid));
	}
}

Maze::~Maze()
{

}


bool Maze::NodeHasUnvisitedNeighbours(int height,int width)
{
	if ((width-1 > 0)&& (grid[height][width - 1].visitedDuring != Node::VisitedDuring::CREATING))
	{	
		return true;
	}
	else if((width + 1 <widthOfGrid)&& (grid[height][width + 1].visitedDuring != Node::VisitedDuring::CREATING))
	{
		return true;		
	}
	else if( (height + 1 <heightOfGrid)&& (grid[height + 1][width].visitedDuring != Node::VisitedDuring::CREATING))
	{
		return true;		
	}
	else if ((height - 1 > 0) && (grid[height - 1][width].visitedDuring != Node::VisitedDuring::CREATING))
	{	
		return true;		
	}
	else return false;
}
void Maze::CreatePassage(int height, int width)
{

	Sleep(3);//little delay to see creating
	grid[height][width].colors[0] = 1.0f;
	grid[height][width].colors[1] = 1.0f;
	grid[height][width].colors[2] = 1.0f;

	if(NodeHasUnvisitedNeighbours(height,width)==false)
	{
		if (recentlyVisitedNodes.empty() == false)
		{
			Node temp = recentlyVisitedNodes.top();
			recentlyVisitedNodes.pop();
			CreatePassage(temp.x, temp.y);
		}
	}
	else {
		
		grid[height][width].visitedDuring = Node::VisitedDuring::CREATING;

		bool foundUnvisited = false;
		while (!foundUnvisited)
		{
			int dir = (std::rand() % 4);
			switch (dir) {
			case 0:
				if (height + 1 < heightOfGrid)
				{
					if (grid[height + 1][width].visitedDuring!= Node::VisitedDuring::CREATING)
					{
						grid[height][width].wall[dir] = false;
						grid[height][width].visitedDuring = Node::VisitedDuring::CREATING;
						height++;
						direction = Direction::DOWN;
						foundUnvisited = true;
					}				
				}
				break;
			case 1:
				if (width - 1 >= 0)
				{
					if (grid[height][width - 1].visitedDuring != Node::VisitedDuring::CREATING)
					{
						grid[height][width].wall[dir] = false;
						grid[height][width].visitedDuring = Node::VisitedDuring::CREATING;
						width--;
						direction = Direction::RIGHT;
						foundUnvisited = true;
					}			
				}
				break;
			case 2:
				if (width + 1 < widthOfGrid)
				{
					if (grid[height][width + 1].visitedDuring != Node::VisitedDuring::CREATING)
					{
						grid[height][width].wall[dir] = false;
						grid[height][width].visitedDuring = Node::VisitedDuring::CREATING;
						width++;
						direction = Direction::LEFT;
						foundUnvisited = true;
					}				
				}

				break;
			case 3:
				if (height - 1 >= 0)
				{
					if (grid[height - 1][width].visitedDuring != Node::VisitedDuring::CREATING)
					{
						grid[height][width].wall[dir] = false;
						grid[height][width].visitedDuring = Node::VisitedDuring::CREATING;
						height--;
						direction = Direction::UP;
						foundUnvisited = true;
					}		
				}
				break;
			}
		}
		


			grid[height][width].wall[direction] = false;//implicit conversion, enum to int
			grid[height][width].visitedDuring = Node::VisitedDuring::CREATING;

			grid[height][width].x = height;
			grid[height][width].y = width;
			

			recentlyVisitedNodes.push(grid[height][width]);
			CreatePassage(height, width);
		
	}
	


}

void Maze::CreateMaze()
{



	srand(time(NULL));
	
	int x = (std::rand() % heightOfGrid);
	int y = (std::rand() % widthOfGrid);

	CreatePassage(0, 0);

	start[0] = std::rand() % heightOfGrid;
	start[1] = 0;
	grid[start[0]][start[1]].wall[1] = false;//removing left wall from start cell

	end[0] = std::rand() % heightOfGrid;
	end[1] = widthOfGrid - 1;

	grid[end[0]][end[1]].wall[2] = false;//removing right wall from end cell
	Sleep(1000);

	ResolveMaze();//start resolving

}


Maze::Direction Maze::FindWay(int x,int y) {

	Sleep(1);
	bool xLessThanZero = false;
	bool yLessThanZero = false;
	bool xGreaterThanRange = false;
	bool yGreaterThanRange = false;

	if (x + 1 >= heightOfGrid)
		xGreaterThanRange = true;
	else if (x - 1 < 0)
		xLessThanZero = true;

	if (y + 1 >= widthOfGrid)
	{
		yGreaterThanRange = true;
	}
	else if (y - 1 < 0)
		yLessThanZero = true;


	if ((!grid[x][y].wall[2]) && (y + 1 < widthOfGrid) && (grid[x][y + 1].visitedDuring!=Node::VisitedDuring::RESOLVING))
	{

		return Direction::RIGHT;
	
	}
	 else if ((!grid[x][y].wall[0]) && (x + 1 < heightOfGrid) && (grid[x + 1][y].visitedDuring != Node::VisitedDuring::RESOLVING))
	{

		 return Direction::UP;
		
	}
	 else if ((!grid[x][y].wall[1]) && (y>=0) && (grid[x][y - 1].visitedDuring != Node::VisitedDuring::RESOLVING))
	{

		 return Direction::LEFT;
		
	}
	 else if ((!grid[x][y].wall[3]) && (x>=0) && (grid[x - 1][y].visitedDuring != Node::VisitedDuring::RESOLVING))
	{

		 return Direction::DOWN;
		
	}
	
	 else
	 {

		 return Direction::NONE;
	 }

}
void Maze::ResolveMaze()
{

	int x = start[0];
	int y = start[1];

	while (grid[end[0]][end[1]].visitedDuring!= Node::VisitedDuring::RESOLVING)// until end cell is not reached
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		grid[x][y].visitedDuring = Node::VisitedDuring::RESOLVING;
		grid[x][y].colors[0] = 1.0f;
		grid[x][y].colors[1] = 0.0f;
		grid[x][y].colors[2] = 1.0f;
		switch (FindWay(x, y))
		{
		case 0:
			recentlyVisitedNodes.push(grid[x][y]);	
			x++;//up
			break;
		case 1:		
			recentlyVisitedNodes.push(grid[x][y]);
			y--;//left
			break;
		case 2:	
			recentlyVisitedNodes.push(grid[x][y]);
			y++;//right
			break;
		case 3:
			recentlyVisitedNodes.push(grid[x][y]);
			x--;//down
			break;
		case 4:
			grid[x][y].colors[0] = 0.0f;
			grid[x][y].colors[1] = 0.0f;
			grid[x][y].colors[2] = 0.0f;
			grid[x][y].pointSize = 4;
			x = recentlyVisitedNodes.top().x;
			y = recentlyVisitedNodes.top().y;
			recentlyVisitedNodes.pop();
			break;
		}
	}
}
void Maze::DrawMaze()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(2.5);
	
	//variables used to draw grid properly 
	int drawHeight = 10;
	int drawWidth = 10;
	int move = 15;

		for (int i = 0; i < widthOfGrid; ++i)
		{
			for (int j = 0; j < heightOfGrid; ++j)
			{
					glPointSize(grid[i][j].pointSize);
					glColor3f(grid[i][j].colors[0], grid[i][j].colors[1], grid[i][j].colors[2]);
					glBegin(GL_POINTS);
					glVertex3f(drawWidth + move / 2, drawHeight + move / 2, 0.0);
					glEnd();
					glColor3f(0.0, 0.0, 0.0);

				if (grid[i][j].wall[0] == true)
				{
					glBegin(GL_LINES);

					glVertex3f(drawWidth + move, drawHeight + move, 0.0);
					glVertex3f(drawWidth, drawHeight + move, 0.0);

					glEnd();

				}
				if (grid[i][j].wall[1] == true)
				{
					glBegin(GL_LINES);
					glVertex3f(drawWidth, drawHeight + move, 0.0);
					glVertex3f(drawWidth, drawHeight, 0.0);
					glEnd();
				}
				if (grid[i][j].wall[3] == true)
				{
					glBegin(GL_LINES);
					glVertex3f(drawWidth, drawHeight, 0.0);
					glVertex3f(drawWidth + move, drawHeight, 0.0);
					glEnd();
				}
				if (grid[i][j].wall[2] == true)
				{
					glBegin(GL_LINES);
					glVertex3f(drawWidth + move, drawHeight, 0.0);
					glVertex3f(drawWidth + move, drawHeight + move, 0.0);
					glEnd();
				}
				drawWidth += move;
			}
			drawWidth = 10;
			drawHeight += move;
		}


}
