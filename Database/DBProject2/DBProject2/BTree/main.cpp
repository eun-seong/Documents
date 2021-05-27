#include <iostream>
using namespace std;

class Entry {
};

class Node {
};

class BTree {
public:
	BTree(const char* fileName, int blockSize);
	bool insert(int key, int rid);
	void print();
	int* search(int key); // point search
	int* search(int startRange, int endRange); // range search
};

BTree::BTree(const char* fileName, int blockSize) {

}


int main(int argc, char* argv[]) {
	char command = argv[1][0];

	switch (command) {
	case 'c':
		// index creation
		break;
	case 'i':
		// insertion
		// btree.bin insert.txt
		break;
	case 's':
		// point search
		// btree.bin search.txt ouput.txt
		break;
	case 'r':
		// range search
		// btree.bin rangesearch.txt output.txt
		break;
	}
}
