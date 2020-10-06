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
this->SetString(rhs.GetString());

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

    // 책 코드
    if (this != &rhs) this->SetString(rhs.GetString());

    return *this;
}


CMyString::operator const char*(void) const {
    return m_pszData;
}

CMyString CMyString::operator+(const CMyString& rhs) {
    CMyString temp(m_pszData);
    temp.Append(rhs.GetString());

    return temp;
}

CMyString& CMyString::operator+=(const CMyString& rhs) {
    Append(rhs.GetString());
    return *this;
}

int CMyString::operator==(const CMyString& rhs) {
    if (m_pszData != NULL && rhs.m_pszData != NULL) {
        if (strcmp(m_pszData, rhs.m_pszData) == 0) return 1;
    }

    return 0;
}

int CMyString::operator!=(const CMyString& rhs) {
    if (m_pszData != NULL && rhs.m_pszData != NULL) {
        if (strcmp(m_pszData, rhs.m_pszData) == 0) return 0;
    }

    return 1;
}

char CMyString::operator[] (int index) const{
    // cout << "operator[]\n";
    return this->m_pszData[index];
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

int CMyString::GetLength() const {
    return m_nLength;
}

void CMyString::Release(){
    if(m_pszData != NULL) delete[] m_pszData;

    m_pszData = NULL;
    m_nLength = 0;
}

int CMyString::Append(const char* pszParam) {
    if (pszParam == NULL) return 0;

    int nLenParam = strlen(pszParam);

    if (nLenParam == 0) return 0;
    
    if (m_pszData == NULL) {
        SetString(pszParam);

        return m_nLength;
    }

    int nLenCur = m_nLength;

    char* pszResult = new char[nLenCur + nLenParam + 1];

    strcpy_s(pszResult, sizeof(char) * (nLenCur + 1), m_pszData);
    strcpy_s(pszResult + (sizeof(char) * nLenCur), sizeof(char) * (nLenParam + 1), pszParam);

    Release();
    m_pszData = pszResult;
    m_nLength = nLenCur + nLenParam;

    return m_nLength;
}
