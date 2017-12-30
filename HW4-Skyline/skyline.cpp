#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

struct buildingPoint {
	buildingPoint(){}
	buildingPoint (int x, int y, bool isStart, int label):x(x),height(y),isStart(isStart),label(label){}
	int x;
	int height;
	int label;
	bool isStart; // Is it a start or end point
};

class MPQ // Modified Priority Queue Class
{
public:
	MPQ(const int pqSize):heap(new MPQobj[pqSize]), Location(new int[pqSize]),PQ_SIZE(pqSize), currentSize(0){}

	void insert(int x, int label)
	{
		currentSize++;
		int hole = currentSize;

		while (hole > 1 && x > heap[hole/2].value) // Max heap
		{ 
			heap[hole] = heap[hole/2]; // Parents goes to down in the tree
			Location[heap[hole/2].label] = hole; // Location information of the ex parent is now downside of the tree
			hole = hole / 2;
		}

		heap[hole].value = x;
		heap[hole].label = label;	
		Location[label] = hole;

	}

	int GetMax ()
	{
		if (!IsEmpty()){ return heap[1].value; }
		else { return -1; }
	}

	struct MPQobj
	{
		MPQobj (){}
		MPQobj (int value, int label):value(value),label(label){}
		int value;
		int label;
	};

	void percolateDown(int hole)
	{
		int child;
		int tmp = heap[hole].value;
		int tmplabel = heap[hole].label;

		while (hole*2 <= currentSize)
		{
			child = hole*2;

			if (child != currentSize && heap[child+1].value > heap[child].value) // Finding biggest child
			{ child++; }	
			
			if (heap[child].value > tmp) // If it is bigger than hole's value
			{ 
				heap[hole] = heap[child]; // Getting child to up because it is bigger 
				Location[heap[child].label] = hole; // Updating child's location
			
			}
			else
			{ break; }

			hole = child; // Lowering the hole (ex place of child)
		}

		heap[hole].value = tmp; 
		heap[hole].label =  tmplabel;
		Location[tmplabel] = hole; // Updating location information
	}

	int Remove(int label)
	{
		int label_location = Location[label]; // Getting location of the requested height value in the heap that is called by its unique label -> O(1) time!
		int tmp = heap[label_location].value;

		heap[label_location] = heap[currentSize--]; // Getting the last child to the removed position
		
		Location[heap[currentSize+1].label] = label_location; // Updating the child's location

        percolateDown(label_location);

		max_value = heap[1].value;
		return tmp;
	}

	bool IsEmpty() { return (currentSize == 0);  }

	~MPQ ()
	{
		delete [] Location;
	}

private:

	const int PQ_SIZE;
	int currentSize;
	MPQobj * heap;
	int max_value;
	int * Location;

};

bool operator<(const buildingPoint & lhs, const buildingPoint &rhs) {
    
	if (lhs.x < rhs.x)
	{ return true; }

	if (lhs.x == rhs.x)
	{		
		if (lhs.isStart && rhs.isStart) // if two STARTs are compared then higher height building should be the first
		{ return (lhs.height > rhs.height); }

		if (!lhs.isStart && !rhs.isStart) // if two ENDs are compared then lower height building should be the first
		{ return (lhs.height < rhs.height); }
		
		return lhs.isStart;  // if one STARTS and END compared, then START should be the first
	}

		return false;
}

int main ()
{
	cout << "This program prints out the skyline of a city." << endl;

	ifstream input;
	input.open("input.txt");

	int buildingNumber;
	int x1, y, x2;
	
	input >> buildingNumber;

	int pointSize = buildingNumber*2+1;

	buildingPoint * buildingPoints = new buildingPoint [pointSize]; // It will be mulptiplied by 2 because there are 2 times of start and end points
	buildingPoint * mBuildingPoint;
	int label, height, prevmax;

	mBuildingPoint = new buildingPoint(0, 0, true, 0); // Starting point
	buildingPoints[0] = *mBuildingPoint;

	int bpIndex = 1; // It started from 1 because we put 0 point as 0 label into the MPQ

	// This for loops takes inputs from user and putting them into buildingPoints array
	for (int i = 0; i < buildingNumber; i++)
	{
		input >> x1 >> y >> x2; // Taking x1, height and x2 values
		label = bpIndex; // Identifying number for that building

		mBuildingPoint = new buildingPoint(x1, y, true, label); // Starting point
		buildingPoints[bpIndex] = *mBuildingPoint;

		mBuildingPoint = new buildingPoint(x2, y, false, label); // End point
		buildingPoints[bpIndex+1] = *mBuildingPoint;

		bpIndex = bpIndex + 2;
	}

	cout << endl << "SKYLINE: " << endl;

	std::sort(buildingPoints, buildingPoints+pointSize); // Sorting of the array

	MPQ mpq(pointSize);

	for (int i = 0; i < pointSize; i++)
	{

		if (buildingPoints[i].isStart)
		{
			height = buildingPoints[i].height;
			prevmax = mpq.GetMax();
			mpq.insert(height, buildingPoints[i].label);		

			if(prevmax != mpq.GetMax()) // If max of the priority queue changes output that building 
			{ cout << buildingPoints[i].x << " " << height << endl;	}
		}

		else 
		{
		
			height = buildingPoints[i].height;
			prevmax = mpq.GetMax();
			mpq.Remove(buildingPoints[i].label);

			if(mpq.GetMax() != prevmax) // If max of the priority queue changes output that building 
			{ cout << buildingPoints[i].x << " " << mpq.GetMax() << endl; }
			
		}

	}
	
	input.close();
	return 0;
}