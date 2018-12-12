#pragma once
// C++ program for Huffman Coding 

// This constant can be avoided by explicitly 
// calculating height of Huffman Tree 

// A Huffman tree node 
struct MinHeapNode {

	// One of the input characters 
	unsigned char data;

	// Frequency of the character 
	unsigned freq;

	// Left and right child of this node 
	struct MinHeapNode *left, *right;
};

// A Min Heap: Collection of 
// min heap (or Hufmman tree) nodes 
struct MinHeap {

	// Current size of min heap 
	unsigned size;

	// capacity of min heap 
	unsigned capacity;

	// Attay of minheap node pointers 
	struct MinHeapNode** array;
};

// A utility function allocate a new 
// min heap node with given character 
// and frequency of the character 
struct MinHeapNode* newNode(unsigned char data, unsigned freq);


// A utility function to create 
// a min heap of given capacity 
struct MinHeap* createMinHeap(unsigned capacity);


// A utility function to 
// swap two min heap nodes 
void swapMinHeapNode(struct MinHeapNode** a,
	struct MinHeapNode** b);


// The standard minHeapify function. 
void minHeapify(struct MinHeap* minHeap, int idx);

// A utility function to check 
// if size of heap is 1 or not 
int isSizeOne(struct MinHeap* minHeap);

// A standard function to extract 
// minimum value node from heap 
struct MinHeapNode* extractMin(struct MinHeap* minHeap);

// A utility function to insert 
// a new node to Min Heap 
void insertMinHeap(struct MinHeap* minHeap,
	struct MinHeapNode* minHeapNode);


// A standard funvtion to build min heap 
void buildMinHeap(struct MinHeap* minHeap);


// A utility function to print an array of size n 
void printArr(int arr[], int n, std::vector<int> myvector);

// Utility function to check if this node is leaf 
int isLeaf(struct MinHeapNode* root);


// Creates a min heap of capacity 
// equal to size and inserts all character of 
// data[] in min heap. Initially size of 
// min heap is equal to capacity 
struct MinHeap* createAndBuildMinHeap(unsigned char data[], int freq[], int size);


// The main function that builds Huffman tree 
struct MinHeapNode* buildHuffmanTree(unsigned char data[], int freq[], int size);

// Prints huffman codes from the root of Huffman Tree. 
// It uses arr[] to store codes 
void printCodes(struct MinHeapNode* root, int arr[], int top, std::vector<int> myvector[]);

// The main function that builds a 
// Huffman Tree and print codes by traversing 
// the built Huffman Tree 
void HuffmanCodes(unsigned char data[], int freq[], int size, std::vector<int> myvector[]);

// Driver program to test above functions 
//int main()
//{
//
//	unsigned char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
//	int freq[] = { 5, 9, 12, 13, 16, 45 };
//
//	int size = sizeof(arr) / sizeof(arr[0]);
//
//	HuffmanCodes(arr, freq, size);
//
//	return 0;
//}
