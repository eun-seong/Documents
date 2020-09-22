#pragma once

class CMyString{
    public:
        CMyString();
        
        CMyString(const CMyString& rhs);            // ���� ������
        explicit CMyString(const char* nParam);     // ��ȯ ������
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