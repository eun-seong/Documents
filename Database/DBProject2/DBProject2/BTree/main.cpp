#include <iostream>
#include <fstream>
#define HEADER_SIZE 12

using namespace std;

struct BTREE_HEADER_FORMAT {
	int blockSize;
	int rootBID;
	int depth;
};

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


bool createBTreeFile(char* binaryFileName, char* blockSize) {
	ofstream binaryFileOut(binaryFileName, ios::binary);

	if (binaryFileOut.fail()) {
		return false;
	}

	BTREE_HEADER_FORMAT header = { atoi(blockSize), HEADER_SIZE, 0 };
	binaryFileOut.write((char*)&header, sizeof(header));

	binaryFileOut.close();
	return true;
}


int main(int argc, char* argv[]) {
	char command = argv[1][0];

	switch (command) {
	case 'c':
		if (!createBTreeFile(argv[2], argv[3])) {
			cerr << "Error on createBTreeFile()\n";
			return 0;
		}
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
