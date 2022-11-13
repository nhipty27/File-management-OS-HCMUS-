#include "BootSector.h"

bool BootSector::checkPassword(string value) {
	return _passWord == value;
}

void BootSector::write(FILE* f)
{
	fseek(f, 0, SEEK_SET);
	fwrite(&this->_numBytePerSector, 1, sizeof(this->_numBytePerSector), f);
	fwrite(&this->_numSectorPerCluster, 1, sizeof(this->_numSectorPerCluster), f);
	fwrite(&this->_numSectorOfBoot, 1, sizeof(this->_numSectorOfBoot), f);
	fwrite(&this->_numFatTable, 1, sizeof(this->_numFatTable), f);
	fwrite(&this->_volumeSize, 1, sizeof(this->_volumeSize), f);
	fwrite(&this->_fatSize, 1, sizeof(this->_fatSize), f);
	fwrite(&this->_numberOfEntry, 1, sizeof(this->_numberOfEntry), f);
	fwrite(&this->_passSize, 1, sizeof(this->_passSize), f);
	fwrite(this->_passWord.c_str(), 1, _passSize, f);
	fclose(f);
}

void BootSector::read(FILE* f)
{
	fread(&this->_numBytePerSector, 1, sizeof(this->_numBytePerSector), f);
	fread(&this->_numSectorPerCluster, 1, sizeof(this->_numSectorPerCluster), f);
	fread(&this->_numSectorOfBoot, 1, sizeof(this->_numSectorOfBoot), f);
	fread(&this->_numFatTable, 1, sizeof(this->_numFatTable), f);
	fread(&this->_volumeSize, 1, sizeof(this->_volumeSize), f);
	fread(&this->_fatSize, 1, sizeof(this->_fatSize), f);
	fread(&this->_numberOfEntry, 1, sizeof(this->_numberOfEntry), f);

	fread(&this->_passSize, 1, sizeof(this->_passSize), f);

	string passWord(this->_passSize, '\0');
	fread(&passWord[0], sizeof(char), _passSize, f);
	this->_passWord = passWord;
}

void BootSector::update(FILE* f, int num)
{
	fseek(f, 28, SEEK_SET);
	fwrite(&this->_numberOfEntry + num, 1, sizeof(this->_numberOfEntry), f);
}


void BootSector::writeBootsector(LPCWSTR fileName) {
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
	SetFilePointer(device, 0, NULL, FILE_BEGIN);
	flag = WriteFile(device, this, 512, &bytesRead, NULL);
	CloseHandle(device);
}

BYTE* BootSector::readBootsector(LPCWSTR fileName) {
	DWORD bytesRead;
	HANDLE device = NULL;
	bool flag;
	BYTE* res = new BYTE[512];
	device = CreateFile(fileName,
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