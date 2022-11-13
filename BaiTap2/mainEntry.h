#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <iostream>
#include <Windows.h>
using namespace std;

class mainEntry
{
private:
	BYTE _MainName[6];
	bool _checkDel; // is deleted 0: deleted
	bool _status; // folder or file 1: file 0:folder
	BYTE _UpdateTime[2];
	BYTE _UpdateDate[2];
	uint32_t _fileSize;
	uint16_t _startCluster;
	uint16_t _nameLength;
	uint16_t _passWordLength;
	uint16_t _numEntryChild;
	uint16_t _numExtraEntry;
	BYTE _Null[6];

public:
	mainEntry();
	mainEntry(string fileName);
	mainEntry craete_mainEntry(string fileName, string pass, 
		BYTE* tg, uint32_t _fileSize, uint16_t _startCluster, uint16_t _numEntryChild, bool _status);
	bool getCheckDelete();
	bool getStatus();
	BYTE* getTime() { return this->_UpdateTime; }
	BYTE* getDate() { return this->_UpdateDate; }
	uint32_t getFileSize();
	uint16_t getStartCluster();
	uint16_t getNameLength();
	uint16_t getPassWordLength();
	uint16_t getNumEntryChild();
	uint16_t getNumExtraEntry();
	BYTE* getMainName() { return this->_MainName; }
	string getName();
	int get2Byte();
	void writeEntry(FILE* f);
	void setStartCluster(int pos);
	void setPassWordLength(string pass);
	BYTE* mainEntryToBYTE();
};

