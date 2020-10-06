#pragma once

class CMyString{
    public:
        CMyString();
        
        CMyString(const CMyString& rhs);            // 복사 생성자
        explicit CMyString(const char* nParam);     // 변환 생성자
        CMyString(CMyString&& rhs);
        ~CMyString();

        CMyString& operator=(const CMyString& rhs);
        operator const char* (void) const;
        CMyString operator+(const CMyString& rhs);
        CMyString& operator+=(const CMyString& rhs);
        int operator==(const CMyString& rhs);
        int operator!=(const CMyString& rhs);
        char operator[] (int index) const;

        int SetString(const char* pszParam);
        const char* GetString() const;
        int GetLength() const;
        void Release();
        int Append(const char* pszParam);

    private:
        char* m_pszData;
        int m_nLength;
};