#include "MyString.h"

#pragma once
class CMyStringEx :
    public CMyString
{
public:
    using CMyString::CMyString;
    ~CMyStringEx();
    void SetString(const char*);
    int Find(const char*);
};