#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// This node struct can be understood as Maze Coordinate
struct node
{
	node (int x, int y):xpos(x),ypos(y){}
	int xpos;
	int ypos;
	node * next;
};
// This is a basic stack class that has push and pop member functions
class stack
{

public:
	stack():head(nullptr) {}

	void push(int x, int y)
	{
		node * nwnode = new node(x,y);
		nwnode->next = head;
		head = nwnode;
	}

	// This is a search function that checks this coordinates is visited or not
	bool isVisited(int x, int y)
	{
		node * ptr = head;
		while (ptr)
		{
			if(ptr->xpos == x && ptr->ypos == y)
			{
				return true;
			}
			ptr = ptr->next;
		}
		return false;
	}

	void pop()
	{
		node * temp = head;
		if(temp)
		{
			head = head->next;
		}
	}

	void recursivePrinter(node * current)
	{
		if(current)
		{
			recursivePrinter(current->next);
			cout << current->xpos << " " << current->ypos << endl;
		}
	}

	node * gethead()
	{
		return head;
	}

private:
	node * head;

};

static int xstart;
static int ystart;
static stack pathStack; 
static stack visitedPath;
static bool isPathFound = false;

void traverse_maze (unsigned int x, unsigned int y, vector <vector<int>> & vec)
{
	// This end condition checks whether the coordinate at the edges of the maze
	if(!((x == 0 || y == 0 || x == vec.size()-1 || y == vec[0].size()-1) && (x != xstart || y != ystart)))
	{
		visitedPath.push(x,y); // Marking as visited

		// These if conditions below checks whether right-left-up-down element visited or not also it considers vector boundaries
		if((y+1) < vec[0].size() && vec[x][y+1] == 0 && !visitedPath.isVisited(x, y+1))
		{	
			pathStack.push(x, y+1);
			traverse_maze(x, y+1, vec);		
		}

		if ((y-1) < vec[0].size() && (y-1) >= 0 && vec[x][y-1] == 0  && !visitedPath.isVisited(x, y-1)) 
		{
			pathStack.push(x, y-1);	
			traverse_maze(x, y-1, vec);			
		}	

		if ((x+1) < vec.size() && vec[x+1][y] == 0 && !visitedPath.isVisited(x+1, y)) 
		{
			pathStack.push(x+1, y);
			traverse_maze(x+1, y, vec);
		}	

		if ((x-1) < vec.size() && (x-1) >= 0 && vec[x-1][y] == 0 && !visitedPath.isVisited(x-1, y)) 
		{
			pathStack.push(x-1, y);
			traverse_maze(x-1, y, vec);
		}

	}

	else
	{
		// One path is enough
		if(!isPathFound){
			cout << "The solution to the puzzle is:" << endl;
			pathStack.recursivePrinter(pathStack.gethead()); 
			isPathFound = true;
		}
	}

	pathStack.pop();
}


bool inputCheck(int row, int col, int xstart, int ystart)
{
	if(row <= 0 || col <= 0)
	{
		cout << "Row and column number must be positive." << endl;
		return false;
	}

	else if(xstart < 0 || ystart <0)
	{
		cout << "You cannot start from a negative index." << endl;
		return false;
	}

	else if (xstart >= row || ystart >= col)
	{
		cout << "Starting point is out of bound." << endl;	
		return false;
	}

	return true;

}


int main ()
{

	int row, col, component;

	cout << "This program solves a two dimensional maze." << endl
		 << "Please enter the row and column numbers, starting points and input matrix: " << endl;

	cin >> row;
	cin >> col;

	cin >> xstart;
	cin >> ystart;
	
	if(!inputCheck(row, col, xstart,ystart))
	{
		return 0;
	}

	vector <vector<int>> vec;

	// Assigning elements to a matrix vector
	for (int i = 0; i < row; i++)
	{
		vector <int> rowvec;
		for (int j = 0; j < col; j++)
		{
			cin >> component;
			rowvec.push_back(component);
		}
		vec.push_back(rowvec);
	}

	cout << endl;


	// Printing out the input data
	for(unsigned int i = 0; i < vec.size(); i++)
	{
		for (unsigned int j = 0; j < vec[0].size(); j++)
		{
			cout << vec[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;

	pathStack.push(xstart, ystart); // Kicking off the process
	traverse_maze(xstart,ystart,vec); // Recursive call

}