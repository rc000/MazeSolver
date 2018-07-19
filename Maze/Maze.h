#pragma once
#include<stack>
#include<vector>
#include<utility>

class Maze {
public:
	enum  Direction {// I dont declare this enum as enum class because I want to allow an implicit conversion to int
		UP,
		LEFT,
		RIGHT,
		DOWN,
		NONE
	};
	Maze();
	~Maze();
	void DrawMaze();
	void CreatePassage(int, int);
	bool NodeHasUnvisitedNeighbours(int, int);
	void CreateMaze();
	void ResolveMaze();
	Direction FindWay(int, int);
private:
	struct Node{
		enum class VisitedDuring {
			UNVISITED,
			CREATING,
			RESOLVING
		};
		bool wall[4];//up,left,right,down

		VisitedDuring visitedDuring;

		enum class Color {
			RED,
			BLUE,
			BLACK,
			NONE
		};
		int distanceToExit;
		Color color;
		int pointSize;
		int x;
		int y;
		Node() : pointSize(10),x(0),y(0), visitedDuring(VisitedDuring::UNVISITED),color(Color::RED),wall{true,true,true,true},distanceToExit(0)
		{
	
		}


	};
	std::pair<int, int> start;
	std::pair<int, int> end;


	int widthOfGrid,heightOfGrid;
	Direction direction;
	std::vector<std::vector<Node> >grid;
	std::stack<Node> recentlyVisitedNodes;


};
