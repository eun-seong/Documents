#include <iostream>
#include <fstream>
#include <string>
#include <stack>
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

struct Ancestor {
	int BID;
	int index;
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
	bool createFile(int blockSize);
	bool insertFile(char* fileName);
	void insert(int key, int rid);
	void writeTreeToFile();
	stack<Ancestor> searchRoom(int key);
	Node* split(Node* curNode, int key);
	int* search(int key); // point search
	int* search(int startRange, int endRange); // range search
	void print();
	Node streamToNode(int* stream);
	void makeEmptyTree(int rootBID);
	void makeTree();
	bool checkRangeForNode(bool isLeafNode, int curEntryNum, int curBID);

	Node getNode(int BID);
	int getBlockOffset(int BID);

private:
	char* fileName;
	int B;
	vector<Node*> nodes;
	fstream f;
	BTREE_HEADER_FORMAT headerInfo;
};

BTree::BTree(char* fileName) :fileName(fileName) {
	makeEmptyTree(1);	// 빈 트리 생성

	ifstream is(this->fileName, ios::binary | ios::in);
	if (is.fail()) return;

	is.read((char*)&(this->headerInfo), sizeof(headerInfo));
	this->B = (this->headerInfo.blockSize - 4) / ENTRY_SIZE + 1;

	if (!is.get() == -1) makeTree();	// 입력된 값이 있을 경우

	is.close();
}

void BTree::makeEmptyTree(int rootBID) {
	nodes.assign(2, new Node(-1));
	nodes[rootBID]->myBID = rootBID;
}

void BTree::makeTree() {
	// TODO
}

bool BTree::checkRangeForNode(bool isLeafNode, int curEntryNum, int curBID) {
	if (isLeafNode) {
		return curEntryNum + 1 <= this->B - 1 && (curEntryNum + 1 >= ceil((B - 1) / 2) || curBID == this->headerInfo.rootBID);
	}
	else {	// for non-leaf node
		return curEntryNum + 1 <= this->B - 1 && curEntryNum + 1 >= ceil((B / 2) - 1);
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
	stack<Ancestor> ancestor = searchRoom(key);
	Node* curNode = this->nodes[ancestor.top().BID];
	int curEntryNum = curNode->entries.size();

	curNode->entries.insert(curNode->entries.begin() + ancestor.top().index, Entry(key, rid));
	// entry 추가 후 정상 범위일 경우 insert 완료
	if (checkRangeForNode(true, curEntryNum, curNode->myBID)) return;

	// 정상 범위가 아닐 경우 split 진행
	ancestor.pop();
	do {
		Node* newNode = split(curNode, key);

		// 현재 레벨이 0. 즉, 부모노드가 없음, rootNode에서 split된 경우
		if (ancestor.empty()) {
			this->headerInfo.rootBID = this->nodes.size();
			this->headerInfo.depth++;
			Node* newRootNode = new Node(this->headerInfo.rootBID);
			newRootNode->nextBID = newNode->myBID;
			
			newRootNode->entries.push_back(Entry(newNode->entries[0].key, curNode->myBID));
			break;
		}

		// 부모 노드가 있음
		curNode = nodes[ancestor.top().BID];
		curNode->entries.insert(curNode->entries.begin() + ancestor.top().index, newNode->entries[0]);
		curEntryNum = curNode->entries.size();

	} while (!checkRangeForNode(false, curEntryNum, curNode->myBID));
}

void BTree::writeTreeToFile() {
	ofstream of(this->fileName, ios::binary | ios::trunc);
	if (of.fail()) {
		cerr << "Error on writeTreeToFile()\n";
		return;
	}

	of.write((char*)&(this->headerInfo), HEADER_SIZE);

	for (int i = 0; i < this->nodes.size(); i++) {
		int* stream = nodes[i]->nodeToStream(headerInfo.blockSize);
		of.write((char*)stream, headerInfo.blockSize);
	}

	of.close();
}

stack<Ancestor> BTree::searchRoom(int key) {
	stack<Ancestor> ancestor;
	int curBID = this->headerInfo.rootBID;
	int nextBID = -1;
	int leafLevel = this->headerInfo.depth;
	int curLevel = 0;

	do {
		Node* curNode = this->nodes[curBID];
	
		int index = 0;
		for (index = 0; index < curNode->entries.size(); index++) {
			if (curNode->entries[index].key > key) {
				nextBID = curNode->entries[index].value;
				break;
			}
		}

		ancestor.push({ curBID,  index }); // 새 엔트리가 들어갈 자리 바로 다음 엔트리의 index
		curBID = nextBID;

	} while (++curLevel < leafLevel);
	
	return ancestor;
}

Node* BTree::split(Node* curNode, int key) {
	int newBID = this->nodes.size();
	nodes.push_back(new Node(newBID));

	Node* newNode = nodes[newBID];
	for (int i = nodes[curNode->myBID]->entries.size() - 1; i >= ceil(B / 2); i--) {
		newNode->entries.push_front(nodes[curNode->myBID]->entries[i]);
		nodes[curNode->myBID]->entries.pop_back();
	}

	return newNode;
}


Node BTree::getNode(int BID) {
	int blockSize = this->headerInfo.blockSize;

	int* stream = new int[blockSize /sizeof(int)];
	f.seekg(getBlockOffset(BID), ios::beg);
	this->f.read((char*)stream, blockSize);	

	return streamToNode(stream);
}

Node BTree::streamToNode(int* stream) {
	Node node(this->B);

	return node;
}

int BTree::getBlockOffset(int BID) {
	return HEADER_SIZE + ((BID - 1) * this->headerInfo.blockSize);
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
		btree.insertFile(argv[3]);
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
