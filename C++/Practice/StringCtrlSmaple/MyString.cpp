#include "stddef.h"
#include "MyString.h"
#include <iostream>
#include <cstring>
using namespace std;

CMyString::CMyString()
    : m_pszData(NULL), m_nLength(0)
{

}

CMyString::~CMyString(){
    Release();
}

int CMyString::SetString(const char* pszParam){
    if(pszParam != NULL){
        m_pszData = new char[strlen(pszParam)+1];
        strcpy(m_pszData, pszParam);

        m_nLength = strlen(m_pszData);
    }

    return 0;
}

const char* CMyString::GetString(){
    return m_pszData;
}

void CMyString::Release(){
    if(m_pszData != NULL) delete[] m_pszData;

    m_pszData = NULL;
    m_nLength = 0;
}