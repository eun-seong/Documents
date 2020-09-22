#include <iostream>
#include "MyString.h"
using namespace std;

void TestFunc(const CMyString& strParam) {
    cout << strParam[0] << '\n';
    cout << strParam[strParam.GetLength() - 1] << '\n';
}

int main(){
    // practice6
    cout << "------------- Practice 6 -------------\n";
    CMyString strLeft("Hello"), strRight("World"), strResult;

    strResult = strLeft + strRight;
    cout << strResult << '\n';

    cout << strLeft << '\n';
    strLeft += CMyString("World");

    cout << strLeft << '\n';

    // practice7
    cout << "------------- Practice 7 -------------\n";
    CMyString strParam("HelloWorld");
    cout << strParam << '\n';
    TestFunc(strParam);

    // practice8
    cout << "------------- Practice 8 -------------\n";
    CMyString strLeft8("Test"), strRight8("String");

    if (strLeft8 == strRight8) cout << "Same\n";
    else cout << "Different\n";

    strLeft8 = CMyString("String");

    if (strLeft8 != strRight8) cout << "Different\n";
    else cout << "Same\n";

    return 0;
}

