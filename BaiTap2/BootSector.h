#define _CRT_SECURE_NO_WARNINGS

#ifndef BootSector_h
#define BootSector_h

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <vector>

using namespace std;
class BootSector
{
private:
    uint16_t _numBytePerSector;
    uint16_t _numSectorPerCluster;
    uint16_t _numSectorOfBoot;
    uint16_t _numFatTable;
    uint64_t _volumeSize;
    uint64_t _fatSize;
    uint32_t _numberOfEntry;
    uint16_t _passSize;
    string _passWord;
public:
    BootSector()
    {
        _numBytePerSector = 512;
        _numSectorOfBoot = 1;
        _numSectorPerCluster = 4;
        _numFatTable = 1;
        _volumeSize = 128 * 1024 * 1024;
        _fatSize = (_volumeSize - 512) / (2 + 4 * 512) + 1;
        _numberOfEntry = 0;
        _passWord = "@";
        _passSize = _passWord.length();
    }

    BootSector(uint32_t volSize)
    {
        _numBytePerSector = 512;
        _numSectorOfBoot = 1;
        _numSectorPerCluster = 4;
        _numFatTable = 1;
        _volumeSize = volSize * 1024 * 1024;
        _fatSize = (_volumeSize - 512) / (2 + 4 * 512) + 1;
        _numberOfEntry = 0;
        _passWord = "#";
        _passSize = _passWord.length();
    }

    uint16_t numBytePerSector() { return _numBytePerSector; }
    uint16_t numSectorOfCluster() { return _numSectorPerCluster; }
    uint16_t numSectorOfBoot() { return _numSectorOfBoot; }
    uint16_t numFatTable() { return _numFatTable; }

    uint64_t volumeSize() { return _volumeSize; }
    void setVolumeSize(uint32_t value) { _volumeSize = value; }


    uint64_t fatSize() { return _fatSize; }

    uint32_t numberOfEntry() { return _numberOfEntry; }
    void setNumberOfEntry(uint32_t value) { _numberOfEntry = value; }

    void setPassSize(uint16_t value) {
        _passSize = value;
    }
    uint16_t passSize() { return _passSize; }

    void setPassWord(string value) { _passWord = value; }
    string passWord() { return _passWord; }

    bool checkPassword(string value);
    void write(FILE* f);
    void read(FILE* f);
    void update(FILE* f, int num);
    void writeBootsector(LPCWSTR fileName);
    BYTE* readBootsector(LPCWSTR fileName);
    
};
#endif