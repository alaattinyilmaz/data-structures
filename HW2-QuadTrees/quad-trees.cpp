/* 
Emir Alaattin Yilmaz - CS300 - Data Structures - HW2 - Sabanci University
This program reads given input coordinates and examines which and how many rectangles it intersects.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// This is a coordinate struct that takes query points
struct Coordinate 
{
	Coordinate(int x, int y):x(x),y(y){}
	int x;
	int y;
};

class Rectangle	{
// This Class has all the features of rectangles
public:
	Rectangle (int top, int left, int bottom, int right):Top(top),Left(left),Bottom(bottom),Right(right){}
	int getTop() const { return Top; }
	int getLeft() const { return Left; }
	int getBottom() const { return Bottom; }
	int getRight() const { return Right; }
	int getWidth() const { return (Right-Left); }
	int getHeight() const { return (Bottom-Top); }

private:
	int Top; // y coordinate of the upper edge
	int Left; // x coordinate of the left edge
	int Bottom; // y coordinate of the bottom edge
	int Right; // x coordinate of the right edge

};

struct node
{
	node(Rectangle * rect):rect(rect),next(nullptr){}
	Rectangle * rect;
	node * next;
};

vector <Rectangle*> contain_rects;

class linkedlist
{

public:
	linkedlist():head(nullptr){}

	void insert(Rectangle & rect)
	{
		node * ptr = head;
		node * newrect = new node(&rect);

		if(!isContain(rect)) // Checking to add unique elements
		{
			if(!head)
			{
				newrect->next = head;
				head = newrect;
			}
			else 
			{
				while (ptr->next)
				{
					ptr = ptr->next;
				}

				ptr->next = newrect;
				newrect->next = nullptr;
			}
		}
	}

	bool isContain(Rectangle & checkrect)
	{
		node * ptr = head;
		while (ptr)
		{
			if (ptr->rect->getTop() == checkrect.getTop() && ptr->rect->getLeft() == checkrect.getLeft() &&
				ptr->rect->getBottom() == checkrect.getBottom() && ptr->rect->getRight() == checkrect.getRight()) //  If it is seen before it will not be added!
			{ return true; }			
			ptr = ptr->next;
		}
		return false;
	}

	void searchRects(int x, int y)
	{
		node * ptr = head;
		while (ptr)
		{
			if (x >= ptr->rect->getLeft() && x < ptr->rect->getRight() && y >= ptr->rect->getTop() && y < ptr->rect->getBottom()) // Checking of this point is inside of rectangle
			{
				contain_rects.push_back(ptr->rect);
			}			
			ptr = ptr->next;
		}
	}

private:
	node * head;

};

class TwoDimTreeNode {
// This is a two dimensional tree node that has a extent rectangle which is the area of that node occupy also it has 4 childs as topleft, topright, bottomleft, bottomright
public:
	TwoDimTreeNode(Rectangle * root):Extent(root->getTop(), root->getLeft(), root->getBottom(), root->getRight()), 
		TopLeft(nullptr), TopRight(nullptr), BottomLeft(nullptr), BottomRight(nullptr) {}

	Rectangle * getExtent() { return &Extent; }
	int getCenterX () {	return (Extent.getLeft()+Extent.getRight())/2; }
	int getCenterY () {	return (Extent.getTop()+Extent.getBottom())/2; }
	linkedlist * getVertical() { return &Vertical;	}
	linkedlist * getHorizontal() { return &Horizontal; }
	void setTopLeft(Rectangle & rect) { TopLeft = new TwoDimTreeNode(&rect); }
	void setTopRight(Rectangle & rect) { TopRight = new TwoDimTreeNode(&rect); }
	void setBottomLeft(Rectangle & rect) { BottomLeft = new TwoDimTreeNode(&rect); }
	void setBottomRight(Rectangle & rect) { BottomRight = new TwoDimTreeNode(&rect); }

	TwoDimTreeNode * getTopLeft() { return TopLeft; }
	TwoDimTreeNode * getTopRight() { return TopRight; }
	TwoDimTreeNode * getBottomLeft() { return BottomLeft; }
	TwoDimTreeNode * getBottomRight() { return BottomRight; }

private:
	Rectangle Extent;
	linkedlist Vertical;
	linkedlist Horizontal;
	TwoDimTreeNode *TopLeft;
	TwoDimTreeNode *TopRight;
	TwoDimTreeNode *BottomLeft;
	TwoDimTreeNode *BottomRight;

};

class TwoDimTree {

public:
	TwoDimTree(Rectangle * rootrect):root(new TwoDimTreeNode(rootrect))	{}
	TwoDimTreeNode * getRoot()	{ return root; } 

	void insert (Rectangle & rect, TwoDimTreeNode * current)
	{

		if (rect.getLeft() <= current->getCenterX() && rect.getRight() > current->getCenterX()) // Vertical Intersection
		{ current->getVertical()->insert(rect); }

		else if (rect.getBottom() > current->getCenterY() && rect.getTop() <= current->getCenterY()) // Horizontal Intersection
		{ current->getHorizontal()->insert(rect); }

		else 
		{

			if (!(current->getExtent()->getWidth() == 1 && current->getExtent()->getHeight() == 1)) // END Condition
			{ 
				if (rect.getRight() <= current->getCenterX() && rect.getBottom() <= current->getCenterY()) // TopLeft
				{
					Rectangle smallrect (current->getExtent()->getTop(), current->getExtent()->getLeft(), (current->getExtent()->getTop()+current->getExtent()->getBottom())/2, (current->getExtent()->getRight()+current->getExtent()->getLeft())/2);

					if(!current->getTopLeft())
					{ current->setTopLeft(smallrect); } // Adding new node to topleft child

					insert(rect, current->getTopLeft());
				}

				else if (rect.getLeft() >= current->getCenterX() && rect.getBottom() <= current->getCenterY()) // TopRight
				{
					Rectangle smallrect (current->getExtent()->getTop(), (current->getExtent()->getLeft()+current->getExtent()->getRight())/2, (current->getExtent()->getTop()+current->getExtent()->getBottom())/2, current->getExtent()->getRight());

					if(!current->getTopRight())
					{ current->setTopRight(smallrect); }

					insert(rect, current->getTopRight());
				}

				else if (rect.getRight() <= current->getCenterX() && rect.getTop() >= current->getCenterY()) // BottomLeft
				{
					Rectangle smallrect ((current->getExtent()->getTop()+current->getExtent()->getBottom())/2, current->getExtent()->getLeft(), current->getExtent()->getBottom(), (current->getExtent()->getRight()+current->getExtent()->getLeft())/2);

					if(!current->getBottomLeft())
					{ current->setBottomLeft(smallrect); }

					insert(rect, current->getBottomLeft());
				}

				else if (rect.getLeft() >= current->getCenterX() && rect.getTop() >= current->getCenterY()) // BottomRight
				{
					Rectangle smallrect ((current->getExtent()->getTop()+current->getExtent()->getBottom())/2 + 1, (current->getExtent()->getRight()+current->getExtent()->getLeft())/2 + 1,  current->getExtent()->getBottom(), current->getExtent()->getRight());

					if(!current->getBottomRight())
					{ current->setBottomRight(smallrect); }

					insert(rect, current->getBottomRight());
				}
			}
		}

	}

	void search (int x, int y, TwoDimTreeNode * current)
	{
		if (current)
		{
			if (x < current->getCenterX() && y < current->getCenterY()) // Top left quadrant
			{ search(x,y, current->getTopLeft()); }

			if (x < current->getCenterX() && y > current->getCenterY()) // Bottom left
			{ search(x,y, current->getBottomLeft()); }

			if (x > current->getCenterX() && y < current->getCenterY()) // Top right
			{ search(x,y, current->getTopRight()); }

			if (x > current->getCenterX() && y > current->getCenterY()) // Bottom right
			{ search(x,y, current->getBottomRight()); }

			current->getHorizontal()->searchRects(x,y);
			current->getVertical()->searchRects(x,y);
		}

	}

private:
	TwoDimTreeNode * root;

};

int main ()
{
	cout << "This program reads given input coordinates and examines which and how many rectangles it intersects." << endl;
	cout << "Please entry the query points: " << endl;
	ifstream input;
	input.open("rectdb.txt");

	TwoDimTree * tree;

	if (input.is_open()){

		int extop, exleft, exbot, exright;
		int top, left, bottom, right;

		input >> extop;
		input >> exleft;
		input >> exbot;
		input >> exright;

		Rectangle extent(extop, exleft, exbot, exright); // Initializer rectangle for tree
		tree = new TwoDimTree(&extent); // Creating the tree with an extent

		TwoDimTreeNode * root = tree->getRoot();
		Rectangle * rect;

		while (input >> top >> left >> bottom >> right)
		{
			if (top < 0 || left < 0 || bottom < 0 || right < 0)
			{ break; }

			else 
			{ 
				rect = new Rectangle (top, left, bottom, right); // Creating a new rectangle
				tree->insert(*rect, root); // Inserting into tree
			}
		}

		int x, y;
		
		vector <Coordinate> coordinates;

		while (cin >> x >> y) // Taking inputs
		{
			if (x < 0 || y < 0) { break; }
			else 
			{ coordinates.push_back(Coordinate(x,y)); }
		}

		cout << endl;

		for (unsigned int i = 0; i < coordinates.size(); i++)
		{
			cout << coordinates[i].x << " " << coordinates[i].y << " (the query point)" << endl;
			tree->search(coordinates[i].x,coordinates[i].y,root);
			cout << contain_rects.size() << " (number of rectangles)" << endl;

			for (int j = contain_rects.size()-1; j >= 0; j--)
			{
				cout << contain_rects[j]->getTop() << " " << contain_rects[j]->getLeft() << " "
					<< contain_rects[j]->getBottom() << " " << contain_rects[j]->getRight() << endl;
			}
			contain_rects.clear();
			cout << endl;
		}

	}

}