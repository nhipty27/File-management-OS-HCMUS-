#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;
class FatTable {
private:
    vector<uint16_t> status;
    uint32_t _size;
public:
    FatTable() {
        _size = 256;
        for (uint16_t i = 0; i < _size; i++)
            status.push_back(0);
    }
    FatTable(uint32_t size)
    {
        _size = size;
        for (uint16_t i = 0; i < size; i++)
            status.push_back(0);

    }

    uint32_t size() { return  _size; }
    void setFatTableSize(uint32_t size) {
        this->_size = size;
    }

    void set_value_at_index(uint16_t value, uint32_t index)
    {
        if (index >= 0 && index < _size)
            status[index] = value;
    }

    uint32_t get_value_at_index(uint32_t index) { return  status[index]; }

    bool writeFatTable(string volName, int posStart, int posEnd) {
        DWORD bytesRead;
        HANDLE hFile;
        bool flag;
        wstring temp = wstring(volName.begin(), volName.end());
        LPCWSTR res = temp.c_str();
        hFile = CreateFile(res,
            GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);
        int j = 0;
        BYTE* arr = new BYTE[posEnd-posStart+1];
        for (int i = posStart; i <= posEnd; i++)
            arr[j++] = this->status[ i];
        SetFilePointer(hFile,posStart +512, NULL, FILE_BEGIN);
        flag = WriteFile(hFile, arr, posEnd - posStart + 1, &bytesRead, NULL);
        CloseHandle(hFile);
        delete[]arr;
        return flag;
    }
};