#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <class HashedObj>
class HashTable {
public:
	explicit HashTable(const HashedObj & notFound, int size);

	HashTable(const HashTable & rhs)
		: ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
		dictionary(rhs.dictionary), currentSize(rhs.currentSize) {
	}

	bool find(const HashedObj & x);
	void makeEmpty();
	bool insert(const HashedObj & x);
	bool remove(const HashedObj & x);
	int getCurrElemSize(); // Returns the current number of elements
	int getProbes(); // Returns the number of probes so far
	bool isFull(); // Checks if the hash table is full
	
	bool HashTable::operator ==(const HashTable &b) const;

	enum EntryType { ACTIVE, EMPTY, DELETED };

	int hashfunc(const HashedObj & x, int size);

	struct HashEntry {
		HashedObj element;
		int code;
		
		EntryType info;

		HashEntry(const HashedObj & e = HashedObj(), EntryType i = EMPTY, int c = 0)
			: element(e), info(i), code(c){
		}
	};

	vector <HashEntry> dictionary;
	
	int findPos(const HashedObj & x);

	int getCode(const HashedObj & x);

private:
	int currentSize;
	const HashedObj ITEM_NOT_FOUND;
	int curr_elems; // Stores the current number of elements
	int numOfProbes; // Stores the number of probes

	bool isActive(int currentPos);
};


template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj & notFound, int size)
	: ITEM_NOT_FOUND(notFound), dictionary(size) {
		curr_elems = 0;
		currentSize = size;
		numOfProbes = 0;
}

template <class HashedObj>
bool HashTable<HashedObj>::operator ==(const HashTable & rhs) const {
	
	// First checking their size if they are not equal they are not the same
	if (rhs.dictionary.size() != currentSize)
	{
		return false;
	}

	// Secondly it is comparing all the elements 
	for (int i = 0; i < currentSize; i++)
	{
		if(dictionary[i].element != rhs.dictionary[i].element)
		{
			return false;
		}
	}

	return true;
}


template <class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj & x) {
	int currentPos = hashfunc(x, dictionary.size());
	int init = 0;
	int i = 0;
	while (dictionary[ currentPos ].info != EMPTY && dictionary[ currentPos ].element != x && dictionary[ currentPos ].info != DELETED) {
		currentPos += 1; // add the difference
		init++;
		if (currentPos >= currentSize) { // perform the mod
			currentPos -= currentSize; // if necessary
		}
	}
	numOfProbes += init; // storing the counted probes

	return currentPos;
}

template <class HashedObj>
int HashTable<HashedObj>::getCurrElemSize() {
	return curr_elems;
}


template <class HashedObj>
int HashTable<HashedObj>::getProbes() {
	return numOfProbes;
}

// Checking of hashtable full or not
template <class HashedObj>
bool HashTable<HashedObj>::isFull() {
	return curr_elems == currentSize;
}

// Makes hashtable empty
template <class HashedObj>
void HashTable<HashedObj>::makeEmpty() {
	for (int i = 0; i < currentSize; i++) {
		dictionary[i].info = EMPTY;
	}
	curr_elems = 0;
	numOfProbes = 0;
}

// Checker function is a place is active
template <class HashedObj>
bool HashTable<HashedObj>::isActive(int currentPos) {
	return dictionary[ currentPos ].info == ACTIVE;
}

// Remover function from dictionary
template <class HashedObj>
bool HashTable<HashedObj>::remove(const HashedObj & x) {
	int currentPos = findPos(x);
	if (isActive(currentPos)) {
		dictionary[ currentPos ].info = DELETED;
		curr_elems--;
		return true;
	}
	return false;
}

// This function 
template <class HashedObj>
bool HashTable<HashedObj>::find(const HashedObj & x) {
	int currentPos = findPos(x);
	if (isActive(currentPos))
		return true;

	return false;
}

// Insert x as active
template <class HashedObj>
bool HashTable<HashedObj>::insert(const HashedObj & x) {
	int currentPos = findPos(x);

	if (isActive(currentPos))
	{ return false; }

	dictionary[ currentPos ] = HashEntry(x, ACTIVE, curr_elems);
	curr_elems++;

	return true;
}

// This function returns code of the key
template <class HashedObj>
int HashTable<HashedObj>::getCode(const HashedObj & x) {
	int currentPos = findPos(x);
	return dictionary[currentPos].code; 
}

template <class HashedObj>
int HashTable<HashedObj>::hashfunc(const HashedObj & key, int size) {
// Hash function
	unsigned int h = 0;

	for (unsigned int i = 0; i < key.size(); i++)
	{
		h = h * 61 +  key[i]; // EN BÜYÜK TRABZON
	}

	return (h % currentSize);

}

#endif