#pragma once

#define _CRT_SECURE_NO_WARNINGS

#ifndef Header_h
#define Header_h
#include "Entry.h"
#include <fstream>
#include <deque>
#define VOLUME_SIZE 128
#define CLUSTER_SIZE 2048 // 4 sector
const int BUFFER_SIZE = 512;

void initVolume(string name, string passWord, int volumeSize);

void PassAction(string volName, int option);

BootSector* readBootSector(FILE* f);

FatTable readFatTable(string volName);

vector<Entry*> listEntry(FILE* f, BootSector* boot, FatTable fatTable);

void deleteListEntry(vector<Entry*>& entry);

int FindEmptyCluster(FatTable& fatTable, int dataSize);

bool changePassVolumn(BootSector* bootSector, string curPass, string passWord);

bool checkPassVolumn(BootSector* bootSector, string passWord);

bool importFile(string volName, BootSector* boot, FatTable fatTable, string filePath, string password);
int FindEmptyCluster(FatTable& fatTable, int size);
BYTE* readBlock(uint32_t offset, LPCWSTR fileName, uint32_t num = 512);

bool writeBlock(uint32_t offset, BYTE res[], LPCWSTR fileName, uint32_t num = 512);

LPCWSTR convertStringToLPCWSTR(string val);

void writeEntry(string volname, BootSector* boot, FatTable fat, Entry entry);
vector<Entry> readEntry(string volname, BootSector* boot);

int changePassFile(vector<Entry>& list, string fileName, string curPass, string newPass);

void changePassWordFile(string volname, BootSector* boot, vector<Entry>& list, string fileName, string curPass, string newPass);

int deleteNomal(vector<Entry>& list, string fileName, string passWord);

void deleteFileNormal(string volName, string fileName, string passWord);

int deleteForever(vector<Entry>& list, string fileName, string passWord);

void deleteFileForever(string volName, string fileName, string passWord);
#endif