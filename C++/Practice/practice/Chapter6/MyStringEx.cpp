#include "MyStringEx.h"
#include <iostream>
#include <string>
using namespace std;

CMyStringEx::~CMyStringEx() {

}

void CMyStringEx::SetString(const char* pszParam) {
	if (strcmp(pszParam, "�۸��̾Ƶ�") == 0) CMyString::SetString("���ѻ��");
	else CMyString::SetString(pszParam);
}

int		CMyStringEx::Find(const char* pszParam) {
	if (pszParam == NULL) return -1;

	const char* ind = strstr(this->GetString(), pszParam);
	if (ind != NULL) return ind - this->GetString();
	return -1;
}