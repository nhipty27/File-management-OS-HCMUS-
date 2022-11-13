#define _CRT_SECURE_NO_WARNINGS

#ifndef Header_h
#define Header_h
#include <iostream>
#include <stdio.h>
#include"Entry.h"
#include<string>
#include<vector>
#include "BootSector.h"
#include "FatTable.h"

#define VOLUME_SIZE 128
#define CLUSTER_SIZE 2048 // 4 sector
const int BUFFER_SIZE = 512;

void initVolume(string name, int volumeSize);

BootSector* readBootSector(FILE* f);

void readFatTable(FILE* fp, BootSector* bootSector, FatTable& fatTable);

vector<Entry*> listEntry(FILE* f, BootSector* boot, FatTable fatTable);

void deleteListEntry(vector<Entry*>& entry);

int FindEmptyCluster(FatTable& fatTable, int dataSize);

bool changePassVolumn(BootSector* bootSector, string curPass, string passWord);

bool checkPassVolumn(BootSector* bootSector, string passWord);

bool importFile(FatTable*fat, string fileSource, string fileName, string password);

BYTE* readBlock(uint32_t sector, LPCWSTR fileName, uint32_t num = 512);

bool writeBlock(uint32_t sector, BYTE* res, LPCWSTR fileName, uint32_t num = 512);

LPCWSTR convertStringToLPCWSTR(string val);
#endif