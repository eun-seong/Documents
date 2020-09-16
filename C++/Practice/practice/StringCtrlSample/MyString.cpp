#include "stddef.h"
#include "MyString.h"
#include <iostream>
#include <cstring>
using namespace std;

CMyString::CMyString()
    : m_pszData(NULL), m_nLength(0)
{

}

CMyString::CMyString(const CMyString& rhs)
    : m_pszData(NULL), m_nLength(0) {
    this->m_pszData = new char[rhs.m_nLength + 1];
    strcpy_s(m_pszData, rhs.m_nLength + 1, rhs.m_pszData);

    // 책 코드
    // this->SetString(rhs.GetString());

    this->m_nLength = rhs.m_nLength;
}

CMyString::CMyString(const char* rhs)
    : m_pszData(NULL), m_nLength(0) {
    SetString(rhs);
}

CMyString::CMyString(CMyString&& rhs)
    : m_pszData(NULL), m_nLength(0) {
    cout << "CMyString 이동 생성자 호출\n";
    
    m_pszData = rhs.m_pszData;
    m_nLength = rhs.m_nLength;

    rhs.m_nLength = 0;
    rhs.m_pszData = NULL;
}

CMyString::~CMyString(){
    Release();
}

CMyString& CMyString::operator=(const CMyString& rhs) {
    strcpy_s(m_pszData, rhs.m_nLength + 1, rhs.m_pszData);
    this->m_nLength = rhs.m_nLength;

    // 책 코드
    if (this != &rhs) this->SetString(rhs.GetString());

    return *this;
}

CMyString::operator const char*(void) const {
    return m_pszData;
}

int CMyString::SetString(const char* pszParam){
    Release();

    if(pszParam != NULL){
        int length = strlen(pszParam) + 1;
        m_pszData = new char[length];
        strcpy_s(m_pszData, length, pszParam);

        m_nLength = strlen(m_pszData);
    }

    return 0;
}

const char* CMyString::GetString() const{
    return m_pszData;
}

void CMyString::Release(){
    if(m_pszData != NULL) delete[] m_pszData;

    m_pszData = NULL;
    m_nLength = 0;
}
