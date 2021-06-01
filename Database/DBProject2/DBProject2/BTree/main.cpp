#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <vector>
#define HEADER_SIZE 12
#define ENTRY_SIZE 8

using namespace std;


struct BTREE_HEADER_FORMAT {
	int blockSize;
	int rootBID;
	int depth;
};

class Entry {
public:
	Entry(int key = INT_MAX, int value = INT_MAX) :key(key), value(value) {}
	int key;
	int value;		// leaf: value, non-leaf: nextLevelBID(left child)
};

bool compare(const Entry a, const Entry b) {
	return a.key < b.key;
}

class Node {
public:
	Node(int myBID) {
		this->myBID = myBID;
	}

	int* nodeToStream(int blockSize) {
		int i = 0, B = (blockSize - 4) / ENTRY_SIZE + 1;
		int* stream = new int[blockSize / sizeof(int)];

		for (i = 0; i < entries.size(); i++) {
			stream[i * 2] = entries[i].key;
			stream[i * 2 + 1] = entries[i].value;
		}
		for (; i < B - 1; i++) {
			stream[i * 2] = 0;
			stream[i * 2 + 1] = 0;
		}
		stream[i * 2] = this->nextBID;

		return stream;
	}

	deque<Entry> entries;
	int nextBID = -1;	// leaf: nextBID, non-leaf: nextLevelBID(last pointer)
	int myBID;
};

class BTree {
public:
	BTree(char* fileName);
	void init();
	bool createFile(int blockSize);
	bool insertFile(char* fileName);
	void insert(int key, int rid);
	void writeTreeToFile();
	deque<int> searchRoom(int key);
	Node* split(Node* curNode, int key);
	int* search(int key); // point search
	int* search(int startRange, int endRange); // range search
	void print(char* fileName);
	Node* streamToNode(int* stream, int BID);
	void makeEmptyTree(int rootBID);
	void makeTree();
	bool checkRangeForNode(bool isLeafNode, int curEntryNum, int curBID);

	Node* getNode(int BID);
	int getBlockOffset(int BID);

private:
	char* fileName;
	int B;
	vector<Node*> nodes;
	fstream f;
	BTREE_HEADER_FORMAT headerInfo;
};

BTree::BTree(char* fileName) : fileName(fileName) {
	makeEmptyTree(1);	// 빈 트리 생성
}

void BTree::init() {
	ifstream is(this->fileName, ios::binary | ios::in);
	if (is.fail()) return;

	is.read((char*)&(this->headerInfo), sizeof(headerInfo));
	this->B = (this->headerInfo.blockSize - 4) / ENTRY_SIZE + 1;

	is.close();
}

void BTree::makeEmptyTree(int rootBID) {
	nodes.push_back(new Node(-1));
	nodes.push_back(new Node(-1));
	nodes[rootBID]->myBID = rootBID;
}

void BTree::makeTree() {
	// TODO
}

bool BTree::checkRangeForNode(bool isLeafNode, int curEntryNum, int curBID) {
	if (isLeafNode) {
		return curEntryNum <= this->B - 1 && (curEntryNum >= ceil((B - 1) / 2) || curBID == this->headerInfo.rootBID);
	}
	else {	// for non-leaf node
		return curEntryNum <= this->B - 1 && curEntryNum >= ceil(B / 2) - 1;
	}
}

bool BTree::createFile(int blockSize) {
	ofstream binaryFileOut(fileName, ios::binary | ios::out);

	if (binaryFileOut.fail()) return false;

	BTREE_HEADER_FORMAT header = { blockSize, 1, 0 };
	binaryFileOut.write((char*)(&header), sizeof(header));

	this->B = (blockSize - 4) / ENTRY_SIZE + 1;

	binaryFileOut.close();
	return true;
}

bool BTree::insertFile(char* fileName) {
	ifstream is(fileName, ios::in);
	if (is.fail()) return false;

	string key, value;

	while (getline(is, key, ',')) {
		is >> value;
		is.seekg(2, ios::cur);	// 개행 문자 : \r\n 2byte
		this->insert(stoi(key), stoi(value));
	}

	is.close();
	
	writeTreeToFile();

	return true;
}


void BTree::insert(int key, int rid) {
	deque<int> ancestor = searchRoom(key);
	Node* curNode = this->nodes[ancestor[0]];
	deque<Entry>::iterator it = curNode->entries.begin();
	ancestor.pop_front();

	// 1. leafNode에 Entry 추가
	Entry newEntry(key, rid);
	it = lower_bound(curNode->entries.begin(), curNode->entries.end(), newEntry, compare);
	it = curNode->entries.insert(it, newEntry);

	// 엔트리 개수가 정상 범위인지 체크
	int curEntryNum = curNode->entries.size();
	if (checkRangeForNode(true, curEntryNum, curNode->myBID)) return;


	// 2. 정상 범위가 아닐 경우 leafNode split 진행
	Node* newNode = split(curNode, key);
	Entry newParentEntry(newNode->entries[0].key, newNode->myBID);
	newNode->nextBID = curNode->nextBID;
	curNode->nextBID = newNode->myBID;

	// 현재 레벨이 0. 즉, 부모노드가 없음, rootNode에서 split된 경우
	if (ancestor.empty()) {
		this->headerInfo.rootBID = this->nodes.size();
		this->headerInfo.depth++;
		Node* newRootNode = new Node(this->headerInfo.rootBID);
		newRootNode->nextBID = curNode->myBID;
		newRootNode->entries.push_back(newParentEntry);
		this->nodes.push_back(newRootNode);
		return;
	}

	// 부모 노드가 있음
	curNode = nodes[ancestor[0]];
	it = lower_bound(curNode->entries.begin(), curNode->entries.end(), newParentEntry, compare);
	it = curNode->entries.insert(it, newParentEntry);
	curEntryNum = curNode->entries.size();


	// 3. bottom-up
	while (!checkRangeForNode(false, curEntryNum, curNode->myBID)) {
		ancestor.pop_front();
		newNode = split(curNode, key);
		newNode->nextBID = newNode->entries[0].value;
		Entry newParentEntry(newNode->entries[0].key, newNode->myBID);
		newNode->entries.pop_front();

		// 현재 레벨이 0. 즉, 부모노드가 없음, rootNode에서 split된 경우
		if (ancestor.empty()) {
			this->headerInfo.rootBID = this->nodes.size();
			this->headerInfo.depth++;
			Node* newRootNode = new Node(this->headerInfo.rootBID);
			newRootNode->nextBID = curNode->myBID;
			newRootNode->entries.push_back(newParentEntry);
			this->nodes.push_back(newRootNode);
			break;
		}

		// 부모 노드가 있음
		curNode = nodes[ancestor[0]];
		it = lower_bound(curNode->entries.begin(), curNode->entries.end(), newParentEntry, compare);
		it = curNode->entries.insert(it, newParentEntry);
		curEntryNum = curNode->entries.size();
	}
}

void BTree::writeTreeToFile() {
	ofstream of(this->fileName, ios::binary | ios::trunc);
	if (of.fail()) {
		cerr << "Error on writeTreeToFile()\n";
		return;
	}

	of.write((char*)&(this->headerInfo), HEADER_SIZE);

	for (int i = 1; i < this->nodes.size(); i++) {
		int* stream = nodes[i]->nodeToStream(headerInfo.blockSize);
		of.write((char*)stream, headerInfo.blockSize);
	}

	of.close();
}

deque<int> BTree::searchRoom(int key) {
	deque<int> ancestor;
	int curBID = this->headerInfo.rootBID;
	int leafLevel = this->headerInfo.depth;
	int curLevel = 0;

	do {
		Node* curNode = this->nodes[curBID];
		int nextBID = curNode->nextBID;
	
		int i = 0;
		for (i = 0; i < curNode->entries.size(); i++) {
			if (key < curNode->entries[i].key) break;
		}
		if (i != 0) nextBID = curNode->entries[i - 1].value;

 		ancestor.push_front(curBID);
		curBID = nextBID;

	} while (curLevel++ < leafLevel);
	
	return ancestor;
}

Node* BTree::split(Node* curNode, int key) {
	int newBID = this->nodes.size();
	nodes.push_back(new Node(newBID));

	Node* newNode = nodes[newBID];
	for (int i = curNode->entries.size() - 1; i >= (B / 2); i--) {
		newNode->entries.push_front(curNode->entries[i]);
		curNode->entries.pop_back();
	}

	return newNode;
}


Node* BTree::getNode(int BID) {
	ifstream is(this->fileName, ios::binary | ios::in);
	int blockSize = this->headerInfo.blockSize;

	int* stream = new int[blockSize /sizeof(int)];
	is.seekg(getBlockOffset(BID), ios::beg);
	is.read((char*)stream, blockSize);

	is.close();
	return streamToNode(stream, BID);
}

Node* BTree::streamToNode(int* stream, int BID) {
	Node* node = new Node(BID);

	for (int i = 0; i < (this->B) - 1; i++) {
		if (stream[i * 2] == 0) break;

		node->entries.push_back(Entry(stream[i * 2], stream[i * 2 + 1]));
	}
	node->nextBID = stream[(this->B-1) * 2];

	return node;
}

int BTree::getBlockOffset(int BID) {
	return HEADER_SIZE + ((BID - 1) * this->headerInfo.blockSize);
}

void printToFile(Node* node, ofstream& outputFile) {
	for (int i = 0; i < node->entries.size(); i++) {
		outputFile << node->entries[i].key;
		if (i != node->entries.size() - 1) outputFile << ",";
	}
}

void BTree::print(char* fileName) {
	ifstream binaryFile(this->fileName, ios::binary | ios::in);
	ofstream outputFile(fileName, ios::out | ios::trunc);

	outputFile << "<0> \n";
	binaryFile.seekg(HEADER_SIZE, ios::beg);
	Node* rootNode = getNode(this->headerInfo.rootBID);
	printToFile(rootNode, outputFile);

	outputFile << "\n\n<1> \n";
	Node* child = getNode(rootNode->nextBID);
	printToFile(child, outputFile);
	outputFile << ",";
	for (int i = 0; i < rootNode->entries.size(); i++) {
		child = getNode(rootNode->entries[i].value);
		printToFile(child, outputFile);
		if (i != rootNode->entries.size() - 1) outputFile << ',';
	}
	outputFile << '\n';

	outputFile.close();
	binaryFile.close();
}



void test() {
	ofstream f("test.bin", ios::binary | ios::out);


	BTREE_HEADER_FORMAT header = { 1, 2, 3 };

	f.write((char*)&header, sizeof(header));

	f.close();
}


int main(int argc, char* argv[]) {
	char command = argv[1][0];
	BTree btree(argv[2]);

	//test();

	switch (command) {
	case 'c':
		if (!btree.createFile(atoi(argv[3]))) {
			cerr << "Error on createBTreeFile()\n";
			return 0;
		}
		break;
	case 'i':
		// insertion
		// btree.bin insert.txt
		btree.init();
		btree.insertFile(argv[3]);
		break;
	case 's':
		// point search
		// btree.bin search.txt ouput.txt
		break;
	case 'r':
		// range search
		// btree.bin rangesearch.txt output.txt
		break;
	case 'p':
		// print level[0], level[1]
		btree.init();
		btree.print(argv[3]);
		break;
	}
}
