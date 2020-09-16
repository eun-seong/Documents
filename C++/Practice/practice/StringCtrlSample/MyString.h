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

        int SetString(const char* pszParam);
        const char* GetString() const;
        void Release();

    private:
        char* m_pszData;
        int m_nLength;
};