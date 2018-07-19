#include "Maze.h"
#include<iostream>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include<thread>
#include<Windows.h>
#include<math.h>
#include<vector>

Maze::Maze() :widthOfGrid(40), heightOfGrid(40),direction(Direction::NONE),start(std::make_pair(0,0)), end(std::make_pair(heightOfGrid-1, widthOfGrid-1))
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

	//Sleep(3);//little delay to see creating
	grid[height][width].color = Node::Color::NONE;
	grid[height][width].distanceToExit = std::abs(end.first - height) + std::abs(end.second- width);

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
		


			grid[height][width].wall[direction] = false;//implicit conversion, enum direction to int
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



	start.first = std::rand() % heightOfGrid;
	start.second = 0;
	grid[start.first][start.second].wall[1] = false;//removing left wall from start cell

	end.first = std::rand() % heightOfGrid;
	end.second = widthOfGrid - 1;

	grid[end.first][end.second].wall[2] = false;//removing right wall from end cell
	CreatePassage(0, 0);
	std::cout << "obok " << grid[end.first - 1][end.second].distanceToExit << std::endl;
	std::cout << "na samej gorze " << grid[39][39].distanceToExit << std::endl;
	std::cout << "na samym dole " << grid[0][39].distanceToExit << std::endl;

	Sleep(1000);

	ResolveMaze();//start resolving

}


Maze::Direction Maze::FindWay(int x,int y) {

	Sleep(100);

	std::pair<int, Direction> distance = std::make_pair(heightOfGrid+widthOfGrid+1, Direction::NONE);

	if ((!grid[x][y].wall[2]) && (y + 1 < widthOfGrid) && (grid[x][y + 1].visitedDuring!=Node::VisitedDuring::RESOLVING))
	{

		if (grid[x][y+1].distanceToExit < distance.first)
		{
			distance.first = grid[x][y+1].distanceToExit;
			distance.second= Direction::RIGHT;
		}
	

	}
	  if ((!grid[x][y].wall[0]) && (x + 1 < heightOfGrid) && (grid[x + 1][y].visitedDuring != Node::VisitedDuring::RESOLVING))
	{

		 if (grid[x+1][y].distanceToExit < distance.first)
		 {

			 distance.first = grid[x+1][y].distanceToExit;
			 distance.second = Direction::UP;
		 }
	}
	 if ((!grid[x][y].wall[3]) && (x > 0) && (grid[x - 1][y].visitedDuring != Node::VisitedDuring::RESOLVING))
	 {

		 if (grid[x-1][y].distanceToExit < distance.first)
		 {

			 distance.first = grid[x-1][y].distanceToExit;
			 distance.second = Direction::DOWN;
		 }


	 }
	  if ((!grid[x][y].wall[1]) && (y>0) && (grid[x][y - 1].visitedDuring != Node::VisitedDuring::RESOLVING))
	{


		 if (grid[x][y-1].distanceToExit < distance.first)
		 {

			 distance.first = grid[x][y-1].distanceToExit;
			 distance.second = Direction::LEFT;
		 }

	}


	return distance.second;


}
void Maze::ResolveMaze()
{

	int x = start.first;
	int y = start.second;

	while (grid[end.first][end.second].visitedDuring!= Node::VisitedDuring::RESOLVING)// until end cell is not reached
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		grid[x][y].visitedDuring = Node::VisitedDuring::RESOLVING;
		grid[x][y].color = Node::Color::BLUE;


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
			grid[x][y].color = Node::Color::BLACK;

	
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
	
	int drawHeight = 10;
	int drawWidth = 10;
	int move = 15;

		for (int i = 0; i < widthOfGrid; ++i)
		{
			for (int j = 0; j < heightOfGrid; ++j)
			{
					glPointSize(grid[i][j].pointSize);
					switch (grid[i][j].color)
					{
					case Node::Color::RED:
						glColor3f(1.0f, 0.0f, 0.0f);
							break;
					case Node::Color::BLACK:
						glColor3f(0.0f, 0.0f, 0.0f);
						break;
					case Node::Color::BLUE:
						glColor3f(1.0f, 0.0f, 1.0f);
						break;
					case Node::Color::NONE:
						glColor3f(1.0f, 1.0f, 1.0f);
						break;
					}
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
