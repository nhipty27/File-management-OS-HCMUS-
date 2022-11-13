#define _CRT_SECURE_NO_WARNINGS
#ifndef entry_h
#define entry_h
#include <iostream>
#include <vector>
#include <fstream>
#include <Windows.h>
#include "extraEntry.h"
#include "mainEntry.h"
#include "dateTime.h"
#include "Sha256.h"
#include "Header.h"
using namespace std;

struct _Entry
{
	mainEntry _mEntry;
	extraEntry _eEntry;
};


class Entry
{
private:
	vector<_Entry> listEntry;

public:
	Entry create_Entry(string name, string passWord,
		uint32_t _fileSize, uint16_t _startCluster, uint16_t _numEntryChild, bool _status);
	string getFileName();
	string getFilePassword();
	bool add(string fileName, BootSector* boot, FatTable& fatTable);
	void read(FILE* f);
	bool checkPassword(string value);
	void setNewPass(string pass);
};
#endif