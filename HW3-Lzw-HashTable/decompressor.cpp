#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
 // 2017 Emir Alaattin Yılmaz
using namespace std;

const int TABLE_SIZE = 4096; // Assigning table size

int main()
{
    clock_t tStart = clock();

	string * dictionary = new string [TABLE_SIZE];
	string sch, cStr, P, C;
	int num_of_elements = 0;	
	int cW, pW;

		for (int i = 0; i < 256; i++) // Assigning ascii characters 
		{
			sch = (char) i;
			dictionary[i] = sch;
			num_of_elements++;
		}

	ifstream input("compout");

	ofstream output("decompout");

	input >> cW; // The first codeword

	if (cW < 0) { cout << "Input failed." << endl; return 0; }

	output << dictionary[cW]; // Outputing the codeword
	pW = cW;
	
	
	while(input >> cW) // cW is next codeword
	{
	
		if (dictionary[cW] != "") // Does it contain codeword?
		{
			output << dictionary[cW]; // Printing out into output file
			P = dictionary[pW];
			C = dictionary[cW][0];
		}
		else 
		{
			P = dictionary[pW];
			C = P[0];
			output << (P + C); // Printing out into output file
		}
		dictionary[num_of_elements] = P + C;
		num_of_elements++;
		pW = cW;
	}

	input.close();
	output.close();
	
	cout << "The decompression has been completed successfully." << endl;

	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	return 0;
}