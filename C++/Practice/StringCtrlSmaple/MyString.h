#pragma once

class CMyString{
    public:
        CMyString();
        ~CMyString();
        int SetString(const char* pszParam);
        const char* GetString() const;
        void Release();

    private:
        char* m_pszData;
        int m_nLength;
};