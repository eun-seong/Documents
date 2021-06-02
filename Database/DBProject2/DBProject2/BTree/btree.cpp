#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <string>
using namespace std;

#define headerSize 12
//Entry nullEntry(INT_MAX, INT_MAX);

class Entry {
public:
	int key;
	int v; //NextLevelBID(in non-leaf node) or Value(in leaf node)
	Entry(int k = INT_MAX, int v = INT_MAX) : key(k), v(v) {}
};
class Node {
public:
	vector<Entry> entries; //indexEntry(in non-leaf node) or dataEntry(in leaf node) 
	int BID; //nextlevelBID(in non-leaf node) or nextBID(in leaf node)
	int B; //이 노드의 최대 entry개수 + 1

	Node(int bid = -1, int B = -1) {
		Entry nullEntry;
		this->BID = bid;
		entries.assign(B + 1, nullEntry);
		this->B = B;

	}
	bool is_null(int idx) {
		if (entries[idx].key == INT_MAX)
			return true;
		else
			return false;
	}
	void insert_at(int idx, Entry e) { //entries의 idx번째 위치에 삽입. 
		if (is_null(idx))
			entries[idx] = e;
		else {
			for (int i = B; i >= idx; i--) {
				if (!is_null(i)) //값이 들어있는 곳부터 idx까지 한칸씩 땡겨
					entries[i + 1] = entries[i];
			}
			entries[idx] = e;
		}
	}
	int* node_to_stream() {
		int blocksize = (B - 1) * 8 + 4;
		int* stream = new int[blocksize / sizeof(int)];

		stream[0] = this->BID;
		for (int i = 1; i < B; i++) {
			stream[i * 2 - 1] = entries[i].key;
			stream[i * 2] = entries[i].v;
		}
		
		return stream;
	}
};
Node* stream_to_node(int blocksize, int* stream) {
	int B = (blocksize - 4) / 8 + 1;
	Node* node = new Node(stream[0], B);

	for (int i = 1; i < B; i++) {
		node->entries[i].key = stream[i * 2 - 1];
		node->entries[i].v = stream[i * 2];
	}
	return node;
}

class BTree {
public:
	fstream f;
	char fileName[100];
	int blockSize;
	int rootBID;
	int depth;
	int B; //한 블록의 최대 entry갯수 + 1
	int lastBID;
	
	BTree(char *fileName);
	void insert(int key, int rid);
	void print(char* filename);
	int search(int key); 
	int* search(int startRange, int endRange); 
	
	Node* get_block(int bid);
	void write_block(int bid, Node* node);
	void print_block(int bid);

	int bid_to_location(int bid) {
		return (bid - 1)*blockSize + headerSize;
	}

	void write_rootBID(int bid) {
		this->f.open(fileName, ios::in | ios::out | ios::binary);
		f.seekp(sizeof(int) * 1);
		f.write((char*)&bid, sizeof(int));
		this->rootBID = bid;
		
		f.close();
	}
	void write_depth(int dep) {
		this->f.open(fileName, ios::in | ios::out | ios::binary);
		f.seekp(sizeof(int) * 2);
		f.write((char*)&dep, sizeof(int));
		this->depth = dep;

		f.close();
	}
};
BTree::BTree(char* fileName) {
	this->f.open(fileName, ios::in | ios::out | ios::binary);

	f.read(reinterpret_cast<char*>(&blockSize), sizeof(int));
	f.read(reinterpret_cast<char*>(&rootBID), sizeof(int));
	f.read(reinterpret_cast<char*>(&depth), sizeof(int));
	
	this->B = (blockSize - 4) / 8 + 1;
	this->lastBID = ((int)(f.tellg()) - headerSize) / blockSize + 1;
	strcpy(this->fileName, fileName);
	f.close();
}
void BTree::insert(int key, int rid) {
	stack<int> ancestor; //계보
	stack<int> indexes; //자식블록id를 가리키는 인덱스의 위치
	ancestor.push(rootBID);

	Entry entry(key, rid);
	Node* nowNode;
	Node* newNode;
	Node* parNode;
	int nowBID;
	int newBID;
	int parBID;
	Entry nullEntry(INT_MAX, INT_MAX);

	//1. search
	for (int i = 1; i <= this->depth; i++) {
		if (i == depth)	//leaf
			break; 
		else {	//nonleaf
			nowBID = ancestor.top();
			nowNode = get_block(nowBID);

			if (key < nowNode->entries[1].key) {
				ancestor.push(nowNode->BID);
				indexes.push(1);
			}else {
				for (int j = 1; j <= this->B; j++)
					if (key < nowNode->entries[j + 1].key) {
						ancestor.push(nowNode->entries[j].v);
						indexes.push(j + 1);
						break;
					}
			}
		}
	}



	//2. leaf에 삽입
	nowBID = ancestor.top(); ancestor.pop();
	nowNode = get_block(nowBID);
	for (int i = 1; i <= this->B; i++)
		if (key < nowNode->entries[i].key) {
			nowNode->insert_at(i, entry);
			break;
		}
	write_block(nowBID, nowNode);



	if (!nowNode->is_null(B)) { //leaf가 꽉차있으면
		//3. leaf_split & add it to parNode
		newNode = new Node(nowNode->BID, B);
		newBID = lastBID + 1; this->lastBID += 1;

		int before = B / 2;
		for (int i = before + 1; i <= B; i++) {
			newNode->entries[i - before] = nowNode->entries[i];
			nowNode->entries[i] = nullEntry;
		}
		nowNode->BID = newBID; //nextblockid를 설정
		write_block(nowBID, nowNode); //nownode를(leaf) 디스크에 기록
		write_block(newBID, newNode); //newnode를 디스크에 기록

		Entry upentry(newNode->entries[1].key, newBID);
		if (ancestor.empty()) { //부모가 없으면 루트 생성
			Node* rootNode = new Node(nowBID, B);
			rootNode->insert_at(1, upentry);
			write_rootBID(lastBID + 1); this->lastBID += 1;
			write_depth(this->depth + 1);

			write_block(rootBID, rootNode);
			parNode = get_block(rootBID);
		}
		else {
			parBID = ancestor.top(); ancestor.pop();
			parNode = get_block(parBID);
			parNode->insert_at(indexes.top(), upentry);
			indexes.pop();
			write_block(parBID, parNode); //parnode를 디스크에 기록
		}
		

		//4. going up
		while (!parNode->is_null(B)) { //B개까지 꽉차있으면
			nowBID = parBID;
			nowNode = parNode;
			int start = B / 2 + 1; //번째 entry부터 newnode로 이동

			newBID = lastBID + 1; this->lastBID += 1;
			newNode = new Node(nowNode->entries[start].v, B); //newnode의 nextlevelBID, 블록갯수 설정
			Entry upentry(nowNode->entries[start].key, newBID);
			
			int j = 1;
			nowNode->entries[start] = nullEntry;
			for (int i = start + 1; i <= B; i++) {
				newNode->entries[j] = nowNode->entries[i];
				nowNode->entries[i] = nullEntry;
				j++;
			}
			write_block(nowBID, nowNode); //수정한 nowNode 기록
			write_block(newBID, newNode); //newNode를 디스크에 기록.
			

			if (nowBID == rootBID) { //새로운 루트에 upentry 삽입
				Node* rootNode = new Node(nowBID, B);
				rootNode->insert_at(1, upentry);
				write_rootBID(lastBID + 1); this->lastBID += 1;
				write_depth(this->depth + 1);

				write_block(rootBID, rootNode);
				break;
			}
			else { //parNode에 upentry 삽입.
				parBID = ancestor.top(); ancestor.pop();
				parNode = get_block(parBID);
				parNode->insert_at(indexes.top(), upentry);
				indexes.pop();
				write_block(parBID, parNode);
			}
		}
	}


	/*
	//바뀐 btree 프린트
	for (int i = 1; i <= lastBID; i++) {
		print_block(i);
	}
	cout << endl;
	*/
	
}
int BTree::search(int key) {
	Node* nowNode;
	int nextBID = rootBID;

	for (int i = 1; i <= this->depth; i++) {
		if (i == depth) { //leaf
			nowNode = get_block(nextBID);
			for (int j = 1; j < this->B; j++) {
				if (key == nowNode->entries[j].key)
					return nowNode->entries[j].v;
			}
		}
		else {	//nonleaf
			nowNode = get_block(nextBID);

			if (key < nowNode->entries[1].key) {
				nextBID = nowNode->BID;
			}
			else {
				for (int j = 1; j <= this->B; j++)
					if (key < nowNode->entries[j + 1].key) {
						nextBID = nowNode->entries[j].v;
						break;
					}
			}
		}
	}
}
int* BTree::search(int start, int end) {
	vector<int> vec;
	int* stream;

	Node* nowNode;
	int nextBID = rootBID;
	for (int i = 1; i <= this->depth; i++) {
		if (i == depth) //leaf
			break;
		else {	//nonleaf
			nowNode = get_block(nextBID);
			if (start < nowNode->entries[1].key) 
				nextBID = nowNode->BID;
			else {
				for (int j = 1; j <= this->B; j++)
					if (start < nowNode->entries[j + 1].key) {
						nextBID = nowNode->entries[j].v;
						break;
					}
			}
		}
	}


	while (true) {
		nowNode = get_block(nextBID);
		nextBID = nowNode->BID;

		for (int j = 1; j < this->B; j++) {
			int k = nowNode->entries[j].key;
			if ( start <= k && k <= end) {
				vec.push_back(nowNode->entries[j].key);
				vec.push_back(nowNode->entries[j].v);
			}
			else if (k != INT_MAX && k > end) {
				int size = vec.size();
				stream = new int[size + 2];
				for (int i = 0; i < size; i++)
					stream[i] = vec[i];

				stream[size] = INT_MAX;
				stream[size + 1] = INT_MAX;
				return stream;
			}
		}
	}
}

void BTree::print(char* filename) {
	ofstream f(filename, ios::out | ios::trunc);
	vector<int> level1_BID;

	f << "<0>" << endl << endl;
	Node* rootNode = get_block(rootBID);
	

	level1_BID.push_back(rootNode->BID);
	f << rootNode->entries[1].key;
	for (int i = 2; i < B; i++) {
		level1_BID.push_back(rootNode->entries[i - 1].v);

		if (rootNode->entries[i].key == INT_MAX) break; //nullentry이면 break
		f << "," << rootNode->entries[i].key;
	}
	f << endl << endl;


	f << "<1>" << endl << endl;

	for (int i = 0; i < level1_BID.size(); i++) {
		Node* node = get_block(level1_BID[i]);
		for (int j = 1; j < B; j++) {
			if (node->entries[j].key == INT_MAX) break;
			f << node->entries[j].key << ",";
		}
	}
	
	f.close();
}
Node* BTree::get_block(int bid) {
	this->f.open(fileName, ios::in | ios::out | ios::binary);
	/*
	seekg을 통해 bid에 해당하는 블록을 int* stream으로 읽어온다.
	stream을 Node형으로 바꿔서 return Node*형.
	*/
	f.seekg(bid_to_location(bid));
	int* stream = new int[blockSize / sizeof(int)];
	f.read(reinterpret_cast<char*>(stream), blockSize);
	f.close();

	return stream_to_node(blockSize, stream);
}
void BTree::write_block(int bid, Node* node) {
	this->f.open(fileName, ios::in | ios::out | ios::binary);
	/*
	node를 stream형으로 바꾸고
	seekfp 설정
	write
	*/
	int* stream = node->node_to_stream();
	f.seekp(bid_to_location(bid));
	f.write(reinterpret_cast<char*>(stream), blockSize);
	f.close();
}
void BTree::print_block(int bid) {
	this->f.open(fileName, ios::in | ios::out | ios::binary);
	f.seekg(bid_to_location(bid));
	int* stream = new int[blockSize / sizeof(int)];
	f.read(reinterpret_cast<char*>(stream), blockSize);

	cout << "BID: " << bid;
	cout << ", 다음블록: " << stream[0];
	for (int i = 1; i < blockSize / sizeof(int); i += 2) {
		cout << ", (" << stream[i] << " " << stream[i + 1] << ")";
	}
	cout << endl;

	f.close();
}

int main(int argc, char* argv[])
{
	char command = argv[1][0];
	switch (command)
	{
	case 'c': // create index file
	{
		//open f
		fstream f(argv[2], ios::in | ios::out | ios::binary | ios::trunc);

		//헤더정보(blocksize, rootBID, Depth) 적기.
		int blocksize = atoi(argv[3]);
		int B = (blocksize - 4) / 8 + 1;
		int rootBID = 1;
		int depth = 1;
		f.write((char*)&blocksize, sizeof(int));
		f.write((char*)&rootBID, sizeof(int));
		f.write((char*)&depth, sizeof(int));

		//f에 빈 leaf노드 출력
		Node* node = new Node(-1, B);
		int* stream = node->node_to_stream();
		f.seekp(headerSize);
		f.write(reinterpret_cast<char*>(stream), blocksize);

	
		break;
	}
	case 'i': // insert records from [records data file], ex) records.txt
	{
		BTree* myBtree = new BTree(argv[2]);
		ifstream input(argv[3], ios::in);

		string line; 
		char buf[100];
		while (getline(input, line)) {
			strcpy(buf, line.c_str());
			int k = atoi(strtok(buf, ","));
			int v = atoi(strtok(NULL, ","));
			myBtree->insert(k ,v);
		}
		break;
	}
	case 's': // search keys in [input file] and print results to [output file]
	{
		BTree* myBtree = new BTree(argv[2]);
		ifstream input(argv[3], ios::in);
		ofstream output(argv[4], ios::out | ios::trunc);
		
		int key;
		while (input >> key) {
			output << key << "," << myBtree->search(key) << endl;
		}
		break;
	}
		
	case 'r': // search keys in [input file] and print results to [output file]
	{
		BTree* myBtree = new BTree(argv[2]);
		ifstream input(argv[3], ios::in);
		ofstream output(argv[4], ios::out | ios::trunc);

		int start, end;
		char c;
		int* stream;
		while (input >> start >> c >>  end) {
			stream = myBtree->search(start, end);
			for (int i = 0; stream[i] != INT_MAX; i += 2) {
				output << stream[i] << "," << stream[i + 1] << " ";
			}
			output << endl;
		}

		break;
	}
		
	case 'p': // print B+-Tree structure to [output file]
	{
		BTree* myBtree = new BTree(argv[2]);
		char* filename = argv[3];
		myBtree->print(filename);

		break;
	}
	}
}