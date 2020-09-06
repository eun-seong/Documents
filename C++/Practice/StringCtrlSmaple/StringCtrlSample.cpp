#include <iostream>
#include "MyString.h"
using namespace std;

void TestFunc(const CMyString &param){
    cout<<param.GetString()<<'\n';
}

int main(){
    CMyString strData;

    strData.SetString("Hello");
    cout << strData.GetString() << '\n';
    strData.Release();

    return 0;
}

