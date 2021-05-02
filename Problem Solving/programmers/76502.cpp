#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

bool isRight(queue<char> s) {
    stack<char> open;
    int sz = s.size();
    
    for(int i=0;i<sz;i++){
        char c = s.front();
        s.pop();
        
        if(c == '[' || c == '(' || c == '{'){
            open.push(c);
        }
        else {
            if(open.empty()) return false;
            char t = open.top();
            open.pop();
            
            if((t != '[' && c == ']') ||
               (t != '(' && c == ')') ||
               (t != '{' && c == '}')) return false;
        }
    }
    
    if(!open.empty()) return false;
    return true;
}

int solution(string s) {
    int answer = 0;
    queue<char> strQueue;
    
    for(int i=0;i<s.length();i++){
        strQueue.push(s[i]);
    }
    
    for(int i=0;i<s.length();i++){
        if(isRight(strQueue)) answer++;
        
        char c = strQueue.front();
        strQueue.pop();
        strQueue.push(c);
    }
    
    return answer;
}