#include <iostream>
using namespace std;

class mClass{
    public:
    mClass(int param){
        this->mData = param;
    }
    
    int mData;
};

void testFunc(mClass &&nParam){
    cout<<"testFunc(): "<<nParam.mData<<'\n';
}

int main(){
    testFunc(mClass(10));

    return 0;
}