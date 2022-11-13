#define _CRT_SECURE_NO_WARNINGS

#ifndef BootSector_h
#define BootSector_h

#include <string>
#include "sha256.h"
#include "convertDataType.h"
using namespace std;

class BootSector
{
private:
    uint16_t _numBytePerSector;
    uint16_t _numSectorOfBoot;
    uint16_t _numSectorPerCluster;
    uint16_t _numFatTable;
    uint64_t _volumeSize;
    uint64_t _fatSize;
    uint64_t _numberOfEntry;
    uint16_t _passSize;
    BYTE _passWord[64];
public:
    void passTest(string tmp) {
        string a = sha256(tmp);
        for (int i = 0; i < a.length(); i++) {
            this->_passWord[i] = BYTE(a[i]);
        }
    }

    BootSector()
    {
        _numBytePerSector = 512;
        _numSectorOfBoot = 1;
        _numSectorPerCluster = 4;
        _numFatTable = 1;
        _volumeSize = 128 * 1024 * 1024;
        _fatSize = (this->_volumeSize - 512) / (2 + 4 * 512) + 1;
        _numberOfEntry = 0;
        _passSize = 32;
        passTest("@");
    }

    BootSector(uint32_t volSize, string pass)
    {
        _numBytePerSector = 512;
        _numSectorOfBoot = 1;
        _numSectorPerCluster = 4;
        _numFatTable = 1;
        _volumeSize = volSize * 1024 * 1024;
        _fatSize = (this->_volumeSize - 512) / (2 + 4 * 512) +1;
        _numberOfEntry = 0;
        _passSize = 64;
        passTest(pass);
    }

    uint16_t numBytePerSector() { return _numBytePerSector; }
    uint16_t numSectorOfCluster() { return _numSectorPerCluster; }
    uint16_t numSectorOfBoot() { return _numSectorOfBoot; }
    uint16_t numFatTable() { return _numFatTable; }

    uint64_t volumeSize() { return _volumeSize; }
    void setVolumeSize(uint32_t value) { _volumeSize = value; }


    uint64_t fatSize() { return _fatSize; }

    uint64_t numberOfEntry() { return _numberOfEntry; }
    void setNumberOfEntry(uint64_t value) { _numberOfEntry = value; }

    void setPassSize(uint16_t value) {
        _passSize = uint16_t(32);
    }
    uint16_t passSize() { return uint16_t(32); }

    void setPassWord(string value) { passTest(value); }
    BYTE* passWord() { return this->_passWord; }

    bool checkPassword(string value) {
        //return _passWord == sha256(value);
        return true;
    }

    void update(FILE* f, int num)
    {
        fseek(f, 28, SEEK_SET);
        fwrite(&this->_numberOfEntry + num, 1, sizeof(this->_numberOfEntry), f);
    }
    bool writeBootsector(string fileName) {
        DWORD bytesRead;
        HANDLE hFile;
        bool flag;
        wstring temp = wstring(fileName.begin(), fileName.end());
        LPCWSTR res = temp.c_str();
        hFile = CreateFile(res,
            GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);
        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
        flag = WriteFile(hFile, (this), 512, &bytesRead, NULL);
        CloseHandle(hFile);
        return flag;
    }

    BYTE* readBootsector(string fileName) {
        DWORD bytesRead;
        HANDLE device = NULL;
        bool flag;
        wstring temp = wstring(fileName.begin(), fileName.end());
        LPCWSTR fileN = temp.c_str();
        BYTE* res = new BYTE[512];
        device = CreateFile(fileN,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);
        SetFilePointer(device, 0, NULL, FILE_BEGIN);
        flag = ReadFile(device, res, 512, &bytesRead, NULL);
        CloseHandle(device);
        if (flag) {
            return res;
        }
        return NULL;
    }

    void convertBootsector(string fileName) {
        BYTE* boot = new BYTE[512];
        convertDataType tmp;
        boot = this->readBootsector(fileName);

        this->_numBytePerSector = tmp.convertByteToInt16(boot, 0);
        this->_numSectorOfBoot = tmp.convertByteToInt16(boot, 2);
        this->_numSectorPerCluster = tmp.convertByteToInt16(boot, 4);
        this->_numFatTable = tmp.convertByteToInt16(boot, 6);
        this->_volumeSize = tmp.convertByteToInt64(boot, 8);
        this->_fatSize = tmp.convertByteToInt64(boot, 16);
        this->_numberOfEntry = tmp.convertByteToInt64(boot, 24);
        this->_passSize = tmp.convertByteToInt16(boot, 32);
        //this->_passWord = tmp.convertByteToString(boot, 34);
        for (int i = 0; i < 64; i++) {
            this->_passWord[i] = boot[34 + i];
        }
        delete[] boot;
    }
};
#endif