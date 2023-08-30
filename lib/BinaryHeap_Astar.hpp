#pragma once
#include <vector>
#include <iostream>
#include <unordered_map>
#include "../src/A_star.h"

using std::vector;

template <typename T>
class DefaultCompare
{
public:
	bool operator()(T obj1, T obj2)
	{
		return obj1 > obj2;
	} 	
};

class Node_Hash
{
public:
	size_t operator()(const Node* key ) const
	{
        //unsigned int
		size_t hashVal = 0;
        if(key->coordinates.X >= 0 && key->coordinates.Y >= 0)
		    hashVal = 0.5 * (key->coordinates.X + key->coordinates.Y) * (key->coordinates.X + key->coordinates.Y + 1) + key->coordinates.X; //will always result in integer

		return hashVal;
	}
};

template <typename T, class Compare = DefaultCompare<T> >
class BinaryHeap
{
private:
	vector<T> container;
	Compare firstBigger;	
	std::unordered_map<Node*, unsigned int, Node_Hash> elementToIndex;

public:
	BinaryHeap();
	void insert(T data); // inserts data at end of container then reorders

	void pop(); // remove top, simply calls deleteElemtn(0) 
	void replaceTop(T newData); // faster than pop then insert
	void replaceElement(T newData, unsigned int index);
	void modifyKey(unsigned int index); // important for A*
	T getDataAtIndex(unsigned int index); // returns data at an index
	void deleteElement(unsigned int index); // deletes element at an index

	T getTop(); // calls getDataAtIndex(0)
	bool isEmpty(); // checks for container size

	const std::unordered_map<Node*, unsigned int, Node_Hash>& getHash();
private:
	void heapSort(unsigned int index);
	void siftUp(unsigned int index);
	void siftDown(unsigned int index);
};

template<typename T, class Compare>
BinaryHeap<T, Compare>::BinaryHeap()
{
    vector<T> container;
}


template<typename T, class Compare>
void BinaryHeap<T, Compare>::insert(T data)
{
	container.push_back(data); // add to end
	elementToIndex[data] = container.size()-1;
	// std::cout << "pushed: " << container[container.size()-1] << std::endl;
	heapSort(container.size()-1);
}


template<typename T, class Compare>
void BinaryHeap<T, Compare>::pop()
{
	if(container.size() <= 0) // bad pop
		return;
	deleteElement(0);
}


template<typename T, class Compare>
void BinaryHeap<T, Compare>::replaceTop(T newData)
{
	return replaceElement(newData, 0);
}


template<typename T, class Compare>
void BinaryHeap<T, Compare>::replaceElement(T newData, unsigned int index)
{
	if(container.size() <= 0)
		return;
	container[index] = newData;
	heapSort(index);
}


template<typename T, class Compare>
void BinaryHeap<T, Compare>::modifyKey(unsigned int index)
{
	if(container.size() <= index)
		return;
	heapSort(index);
}


template<typename T, class Compare>
T BinaryHeap<T, Compare>::getDataAtIndex(unsigned int index)
{
	if(container.size() <= index)
		throw std::out_of_range("index out of range");
	return container[index];
}


template<typename T, class Compare>
void BinaryHeap<T, Compare>::deleteElement(unsigned index)
{
	if(container.size() <= index)
		return;
	
	T temp = container.back();
	container[container.size()-1] = container[index];
	container[index] = temp;
	// A* specific implementation
	elementToIndex.erase(container.back());
	container.pop_back();
	heapSort(index);
}


template<typename T, class Compare>
T BinaryHeap<T, Compare>::getTop()
{
	return container.front();
}

template<typename T, class Compare>
bool BinaryHeap<T, Compare>::isEmpty()
{
	return container.empty();
}

template<typename T, class Compare>
const std::unordered_map<Node*, unsigned int, Node_Hash>& BinaryHeap<T, Compare>::getHash()
{
	return elementToIndex; // const reference
}


// no two elements next to each other will be greater than its parent?
template<typename T, class Compare>
void BinaryHeap<T, Compare>::heapSort(unsigned int indexCheck)
{
	// std::cout << "index: " << indexCheck << '\n';
	if(indexCheck > 0 && firstBigger(container[(indexCheck-1)/2], container[indexCheck]))
		siftUp(indexCheck);
	else if((2*indexCheck+1 < container.size() && firstBigger(container[indexCheck], container[(2*indexCheck)+1])) || (2*indexCheck+2 < container.size() && firstBigger(container[indexCheck], container[2*indexCheck+2])))
		siftDown(indexCheck);
}

template<typename T, class Compare>
void BinaryHeap<T, Compare>::siftUp(unsigned int indexCheck)
{
	int tempIndex;
	T temp;
	// std::cout << "sift-up\n";
	while(indexCheck > 0 && firstBigger(container[(indexCheck-1)/2], container[indexCheck]))
	{
		// swap
		tempIndex = (indexCheck-1)/2;
		temp = container[tempIndex];
		container[tempIndex] = container[indexCheck];
		container[indexCheck] = temp;
		indexCheck = tempIndex;

		// swap pos in map
		elementToIndex[container[indexCheck]] = indexCheck;
		elementToIndex[container[tempIndex]] = tempIndex;
	}	
}

template<typename T, class Compare>
void BinaryHeap<T, Compare>::siftDown(unsigned int indexCheck)
{
	T temp;
	int tempIndex;
	bool swap = true;
	// std::cout << "sift-down\n";
	while(indexCheck < container.size() && swap)
	{
		if((2*indexCheck+1 < container.size() && firstBigger(container[indexCheck], container[(2*indexCheck)+1])) || (2*indexCheck+2 < container.size() && firstBigger(container[indexCheck], container[2*indexCheck+2])))
		{
			swap = true;	
		}
		else
			swap = false;
		if(swap)
		{
			if(firstBigger(container[2*indexCheck+1], container[2*indexCheck+2]))
				tempIndex = 2*indexCheck+2;
			else
				tempIndex = 2*indexCheck+1;

			// swap
			temp = container[tempIndex];
			container[tempIndex] = container[indexCheck];
			container[indexCheck] = temp;
			indexCheck = tempIndex;
		}
	}	
}
