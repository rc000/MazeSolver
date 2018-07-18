#pragma once
#include<stack>
#include<thread>
#include<vector>

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

		float colors[3];
		int pointSize;
		int x;
		int y;
		Node() : pointSize(10), colors{ 1.0f,0.0f,0.0f },x(0),y(0), visitedDuring(VisitedDuring::UNVISITED)
		{
			for (int i = 0; i < 4; i++)
				wall[i] = true;
		}


	};
	int start[2];
	int end[2];
	int widthOfGrid,heightOfGrid;
	Direction direction;
	std::vector<std::vector<Node> >grid;
	std::stack<Node> recentlyVisitedNodes;



	


};
