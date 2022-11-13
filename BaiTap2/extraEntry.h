#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <iostream>
#include <Windows.h>
using namespace std;

class extraEntry
{
private:
	uint16_t _pos;
	BYTE _Content[30];
public:
	extraEntry();
	extraEntry(string val, int numEntry);
	extraEntry create_extraEntry(string val, int numEntry);
	void readExtraEntry(string& val, int length);
	uint16_t getPos();
	string getContent(int start = 0, int end = 30);
	void writeEntry(FILE* f);
	void setContent(string val,  int nameLength);
};

