#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "HashTable.h"
#include <chrono>

using namespace std;

void insertChars(HashTable<string> & hTable)
{
	string sch;
	for (int ch = 0; ch < 256; ch++)
	{
		sch = (char) ch;
		hTable.insert(sch);
	}
}

int main()
{
	 
    clock_t tStart = clock();

	ifstream input("compin");
	ofstream output("compout");

	int table_size = 4096;
	HashTable <string> hTable("not found", table_size);
	insertChars(hTable); // Insertion of chars into hashtable

	string p = "";
	char ch; // Next char
	string c;

	bool lastone = false; 

	while(input.peek() != EOF) // c is next char
	{
		input.get(ch);
		c = ch;
		if (hTable.find(p + c)) // Does it contain p+c?
		{
			p = p + c;
		}
		else 
		{
			output << hTable.getCode(p) << " "; // Printing of code of p
			hTable.insert(p+c); // add p+c to the dictionary
			p = c;
		}
	}

	output << hTable.getCode(p) << " "; // Printing out the code of the last character

	input.close();
	output.close();
	cout << "Compression has been completed sucessfully." << endl << endl;
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	return 0;
}