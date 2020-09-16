#include <iostream>
#include "MyString.h"
using namespace std;

CMyString TestFunc() {
    CMyString strTest("TestFunc() return");
    cout << strTest << '\n';

    return strTest;
}

int main(){
    TestFunc();

    return 0;
}

