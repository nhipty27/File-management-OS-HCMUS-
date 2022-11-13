#define _CRT_SECURE_NO_WARNINGS
#include <queue>
#include"Header.h"
#include"Entry.h"
#include <stdint.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

//Ham doc 512bytes/ 1 sector
BYTE* readBlock(uint32_t sector, LPCWSTR fileName, uint32_t num ) {
	DWORD bytesRead;
	HANDLE device = NULL;
	bool flag;
	BYTE* res = new BYTE[num];
	device = CreateFile(fileName,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	SetFilePointer(device, sector, NULL, FILE_BEGIN);
	flag = ReadFile(device, res, num, &bytesRead, NULL);
	CloseHandle(device);
	if (flag)
		return res;
	return NULL;
}

//Ham ghi 512bytes/ 1 sector
bool writeBlock(uint32_t sector, BYTE* res, LPCWSTR fileName, uint32_t num ) {
	DWORD bytesRead;
	HANDLE device = NULL;
	bool flag;
	device = CreateFile(fileName,
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	SetFilePointer(device, sector, NULL, FILE_BEGIN);
	flag = WriteFile(device, res, num, &bytesRead, NULL);
	CloseHandle(device);
	return flag;
}

LPCWSTR convertStringToLPCWSTR(string val) {
	wstring temp = wstring(val.begin(), val.end());
	LPCWSTR res = temp.c_str();
	return res;
}

void initVolume(string name, int volumeSize) {
	FILE* fp = nullptr;
	errno_t err = fopen_s(&fp, name.c_str(), "wb+");
	if (fp) {
		fseek(fp, volumeSize - 1, SEEK_SET);
		fwrite("", 1, 1, fp);
		fclose(fp);
	}
	err = fopen_s(&fp, name.c_str(), "rb+");
	if (fp) {
		BYTE buffer[512] = { 0 };
		for (int i = 0; i < 512; i++) {
			buffer[i] = BYTE(0);
		}

		int n = (volumeSize * 1048576) / 512;
		for (int i = 0; i < n; i++) {	//1 MB=1048576B
			fwrite(buffer, 512, 1, fp);
		}

		string passWord;
		BootSector* boot = new BootSector(volumeSize);
		//Nhap mat khau cho vol bat buoc
		cout << "Enter volumn password: ";
		cin >> passWord;
		boot->setPassWord(passWord);
		boot->setPassSize(passWord.length());
		boot->write(fp);
		//khoi tao fat table
		err = fopen_s(&fp, name.c_str(), "rb+");
		fseek(fp, 512, SEEK_SET);

		FatTable fatTable(boot->fatSize());

		for (int i = 0; i < fatTable.size(); i++) {
			uint16_t status = 0;
			fwrite(&status, 1, 2, fp);
		}
	}
	fclose(fp);
}
BootSector* readBootSector(FILE* f) {
	BootSector* boot = new BootSector();
	boot->read(f);
	return boot;
}
void readFatTable(FILE* f, BootSector* boot, FatTable& fatTable)
{
	fseek(f, boot->numSectorOfBoot() * boot->numBytePerSector(), SEEK_SET);
	for (int i = 0; i < fatTable.size(); i++)
	{
		uint16_t status = 0;
		fread(&status, 1, 2, f);
		fatTable.set_value_at_index(status, i);
	}
}
vector<Entry*> listEntry(FILE* f, BootSector* boot, FatTable fatTable) {
	vector<Entry*> result;
	uint32_t size = 512 + fatTable.size();
	for (int i = 0; i < boot->numberOfEntry(); i++) {
		fseek(f, size, SEEK_SET);
		Entry* e = new Entry();
		e->read(f);
		result.push_back(e);
		size += 32;
	}
	return result;
}

void deleteListEntry(vector<Entry*>& entry)
{
	for (int i = 0; i < entry.size(); ++i)
	{
		delete entry[i];
	}
}
bool isEnoughSpace(int numberOfCluster, FatTable fatTable, int k) {
	int noClus = numberOfCluster;
	bool found = true;
	for (int i = k; numberOfCluster > 0; i--, numberOfCluster--)
		if (fatTable.get_value_at_index(i) != 0)
			return false;
	return true;
}
int sizeOfFile(FILE* f)
{
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	return size;
}
int FindEmptyCluster(FatTable& fatTable, int dataSize)
{
	int numberOfClusters = dataSize / CLUSTER_SIZE + 1;
	int positionEnd = -1;
	for (int i = fatTable.size() - 1; i >= 0; i--) {
		if (fatTable.get_value_at_index(i) == 0) {
			if (isEnoughSpace(numberOfClusters, fatTable, i)) {
				positionEnd = i;
				break;
			}
			else {
				i -= numberOfClusters;
			}
		}
	}

	int positionStart = positionEnd - numberOfClusters;

	for (int i = positionStart; i <= positionEnd; i++) {
		fatTable.set_value_at_index(i, 1);
	}
	return positionStart;
}

bool changePassVolumn(BootSector* bootSector, string curPass, string passWord)
{
	bool successful = bootSector->checkPassword(curPass);
	if (successful)
	{
		bootSector->setPassWord(passWord);
		bootSector->setPassSize(passWord.length());
	}
	return true;
}

bool checkPassVolumn(BootSector* bootSector, string passWord) {
	return bootSector->checkPassword(passWord);
}

bool changeFilePassWord(vector<Entry*> entry, string fileName, string curPass, string newPass)
{
	bool successful = false;
	
	int i = 0;
	while (i < entry.size()) {
		if (entry[i]->getFileName() == fileName) {
			if (entry[i]->checkPassword(curPass)) {
				entry[i]->setNewPass(newPass);
				successful = true;
			}
		}
	}
	
	return successful;
}

//Inport file
bool importFile(FatTable* fat, string fileSource, string fileName, string password) {
	vector<Entry> entry;
}