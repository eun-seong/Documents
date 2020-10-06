#include <iostream>
#include "MyStringEx.h"
using namespace std;

int main(){
    // Practice9
    cout << "------------- Practice 9 -------------\n";
    CMyStringEx strTest;
    strTest.SetString("I am a boy");
    cout << strTest << '\n';

    int nIndex = strTest.Find("am");
    cout << "Index: " << nIndex << '\n';

    //Practice10
    cout << "------------- Practice 10 -------------\n";
    strTest.SetString("¸Û¸ÛÀÌ¾Æµé");
    cout << strTest << '\n';

    strTest.SetString("Hello");
    cout << strTest << '\n';

    // Practice11
    cout << "------------- Practice 11 -------------\n";
    CMyStringEx strLeft("Hello"), strRight("World");
    cout << strLeft + strRight << '\n';

    return 0;
}

