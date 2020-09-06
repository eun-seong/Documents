#include <iostream>
#include "MyString.h"
using namespace std;

int main(){
    CMyString strData;

    strData.SetString("Hello");
    cout << strData.GetString() << '\n';
    strData.Release();

    return 0;
}

