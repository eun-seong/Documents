#include <vector>
#include <cmath>
#include <queue>
using namespace std;

bool visited[300001];
vector<int> nodeList[300001];
int nodes[300001];

long long solution(vector<int> a, vector<vector<int> > edges) {
    long long answer = 0;
    int nodeNum = a.size();
    vector<long long> weight(nodeNum);
    queue<int> q;
    
    int checkSum = 0;
    for(int i=0;i<nodeNum;i++){
        checkSum += a[i];
        weight[i] = a[i];
    }
    if(checkSum != 0) return -1;
    
    
    for(int i=0;i<nodeNum-1;i++){
        nodeList[edges[i][0]].push_back(edges[i][1]);
        nodeList[edges[i][1]].push_back(edges[i][0]);
    }
    
    for(int i=0;i<nodeNum;i++){
        int curSize = nodeList[i].size();
        nodes[i] = curSize;
        if(curSize == 1) q.push(i);
    }
    
    while(!q.empty()){
        int currentNode = q.front();
        q.pop();
        visited[currentNode] = true;
        
        for(int i=0;i<nodeList[currentNode].size();i++){
            int nextNode = nodeList[currentNode][i];
            if(!visited[nextNode]){
                answer += abs(weight[currentNode]);
                weight[nextNode] += weight[currentNode];
                weight[currentNode] = 0;
                nodes[nextNode]--;
                if(nodes[nextNode]==1) q.push(nextNode);
            }
        }
    }

    return answer;
}


