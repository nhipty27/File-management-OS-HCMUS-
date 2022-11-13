#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include "BootSector.h"
#include "FatTable.h"
#include "dateTime.h"
#include "sha256.h"
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
	mainEntry() {
		for (int i = 0; i < 6; i++) {
			this->_MainName[i] = BYTE(int(0));
		}
		this->_status = 1;
		this->_checkDel = 1;
		this->_UpdateTime[0] = 0;
		this->_UpdateTime[1] = 0;
		this->_UpdateDate[0] = 0;
		this->_UpdateDate[0] = 0;
		this->_fileSize = int(0);
		this->_startCluster = uint16_t(0);
		this->_passWordLength = uint16_t(0);
		this->_nameLength = uint16_t(0);
		this->_numEntryChild = uint16_t(0);
		this->_numExtraEntry = uint16_t(0);
		for (int i = 0; i < 6; i++) {
			this->_Null[i] = uint16_t(0);
		}
	}
	mainEntry craete_mainEntry(string fileName, string pass, BYTE* tg, uint32_t _fileSize, uint16_t _startCluster, uint16_t _numEntryChild, uint16_t _numExtraEntry, bool _status) {
		if (fileName.length() > 6) {
			for (int i = 0; i < 6; i++) {
				this->_MainName[i] = BYTE(int(fileName[i]));
			}
		}
		else {
			for(int i=0;i< fileName.length();i++)
				this->_MainName[i] = BYTE(int(fileName[i]));
		}
		
		this->_checkDel = 0;
		this->_UpdateTime[0] = tg[0];
		this->_UpdateTime[1] = tg[1];
		this->_UpdateDate[0] = tg[2];
		this->_UpdateDate[0] = tg[3];
		this->_fileSize = _fileSize;
		this->_status = _status;
		this->_startCluster = _startCluster;
		this->_passWordLength = uint16_t(pass.length());
		this->_nameLength = uint16_t(fileName.length());
		this->_numEntryChild = _numEntryChild;
		uint16_t n = (fileName.length() + pass.length()) / 15;
		this->_numExtraEntry = uint16_t(_numExtraEntry);
		for (int i = 0; i < 6; i++) {
			this->_Null[i] = uint16_t(0);
		}
		return *this;
	}
	bool getCheckDelete() {
		return this->_checkDel;
	}
	bool getStatus() {
		return this->_status;
	}
	BYTE* getTime() { return this->_UpdateTime; }
	BYTE* getDate() { return this->_UpdateDate; }
	uint32_t getFileSize() {
		return this->_fileSize;
	}
	uint16_t getStartCluster() {
		return this->_startCluster;
	}
	uint16_t getNameLength() {
		return this->_nameLength;
	}
	uint16_t getPassWordLength() {
		return this->_passWordLength;
	}
	uint16_t getNumEntryChild() {
		return this->_numEntryChild;
	}
	uint16_t getNumExtraEntry() {
		return this->_numExtraEntry;
	}
	string getMainName() {
		string res = "";
		for (int i = 0; i < 6; i++) {
			res += this->_MainName[i];
		}
		return res;
	}
	string getName() {
		string res = "";
		for (int i = 0; i < this->_nameLength; i++) {
			res += this->_MainName[i];
		}
		return res;
	}
	int get2Byte() {
		return int(this->_MainName[1]);
	}

	void setDeleted(bool value) {
		_checkDel = 0;
	}
	void setPassWordLength(uint16_t value) {
		_passWordLength = value;
	}

	void setFileName(string fileName)
	{
		for (int i = 0; i < 6; i++) {
			this->_MainName[i] = fileName[i];
		}
	}

	void setNumEntryChild(int num = 1) { this->_numEntryChild += num; }

	mainEntry convertMainEntry(BYTE* res, int start) {
		convertDataType tmp;
		for (int i = 0; i < 6; i++) {
			this->_MainName[i] = tmp.convertByteToInt16(res, start + i);
		}
		this->_status = boolean(res[start + 6]);
		this->_checkDel = boolean(res[start + 7]);
		this->_UpdateTime[0] = res[start + 8];
		this->_UpdateTime[1] = res[start + 9];
		this->_UpdateDate[0] = res[start + 10];
		this->_UpdateDate[1] = res[start + 11];
		this->_fileSize = tmp.convertByteToInt32(res, start + 12);
		this->_startCluster = tmp.convertByteToInt16(res, start + 16);
		this->_nameLength = tmp.convertByteToInt16(res, start + 18);
		this->_passWordLength = tmp.convertByteToInt16(res, start + 20);
		this->_numEntryChild = tmp.convertByteToInt16(res, start + 22);
		this->_numExtraEntry = tmp.convertByteToInt16(res, start + 24);
		for (int i = 0; i < 7; i++) {
			this->_Null[i] = 0;
		}
		return *this;
	}
};

class extraEntry
{
private:
	uint16_t _pos;
	BYTE _Content[30];
public:
	extraEntry() {
		this->_pos = -1;
		int i = 0;
		while (i < 30) {
			this->_Content[i] = BYTE(int(" "));
			this->_Content[i + 1] = BYTE(int(0));
			i += 2;
		}
	}
	extraEntry create_extraEntry(string val, int numEntry) {
		this->_pos = numEntry + 1;
		int i = 0;
		int posVal = numEntry * 15;
		while (i < 30) {
			this->_Content[i] = BYTE(int(val[posVal]));
			this->_Content[i + 1] = BYTE(int(0));
			posVal += 1;
			i += 2;
			if (posVal == val.length())
				break;
		}
		return *this;
	}
	void readExtraEntry(string& val, int length) {

	}
	uint16_t getPos() {
		return this->_pos;
	}
	string getContent(int start = 0, int end = 30) {
		string res = "";
		int i = start;
		while (true) {
			res += this->_Content[i];
			i += 2;
			if (i > end || i == 30)
				break;
		}
		return res;
	}
	void setContent(string val, int nameLength);

	extraEntry convertExtraEntry(BYTE* res, int start) {
		convertDataType tmp;
		this->_pos = tmp.convertByteToInt16(res, start);
		for (int i = 0; i < 30; i++) {
			this->_Content[i] = res[start + 2 + i];
		}
		return *this;
	}
};

struct _Entry {
	mainEntry _mEntry;
	extraEntry _eEntry;
};
class Entry
{
private:
	vector<_Entry> listEntry;
public:
	Entry() {
		return;
	}
	vector<_Entry> getList() { return this->listEntry; };
	Entry(string name, string passWord, uint32_t _fileSize = 0, uint16_t _startCluster = 0, uint16_t _numEntryChild = 0, bool _status = 1) {
		dateTime tmp;
		tmp.getCurrentTime();
		BYTE* tg = new BYTE[4];
		tg = tmp.convert_dateTime_to_hex_with_little_endian();
		passWord = sha256(passWord);
		_Entry A;
		A._mEntry = A._mEntry.craete_mainEntry(name, passWord, tg, _fileSize, _startCluster, _numEntryChild, (name + passWord).length() / 15 + 1, _status);
		this->listEntry.push_back(A);
		string data = name + passWord;
		for (int i = 0; i < data.length() / 15 + 1; i++) {
			_Entry B;
			B._eEntry = B._eEntry.create_extraEntry(data, i);
			this->listEntry.push_back(B);
		}
	};

	void set_value_at_index(_Entry value, uint32_t index)
	{
		if (index >= 0 && index < listEntry.size())
			listEntry[index] = value;
	}

	void setPassWord(string passWord)
	{
		vector<_Entry> entryList = this->getList();
		entryList[0]._mEntry.setPassWordLength(64);
		string data = this->getFileName() + sha256(passWord);
		for (int i = 0; i < data.length() / 15 + 1; i++) {
			_Entry B;
			B._eEntry = B._eEntry.create_extraEntry(data, i);
			set_value_at_index(B, i + 1);
		}

	}

	void setFileName(string fileName)
	{
		_Entry entry = this->getList()[0];
		entry._mEntry.setFileName(fileName);
		set_value_at_index(entry, 0);

		string passWord = this->getFilePassword();
		string data = fileName + passWord;
		for (int i = 0; i < data.length() / 15 + 1; i++) {
			_Entry B;
			B._eEntry = B._eEntry.create_extraEntry(data, i);
			set_value_at_index(B, i + 1);
		}
	}

	//ham lay ten file
	string getFileName() {
		int n = this->listEntry[0]._mEntry.getNameLength();
		string res = "";
		if (n <= 6)
			return this->listEntry[0]._mEntry.getName();
		int numEntry = n / 15;
		if (n % 15 != 0) numEntry += 1;
		for (int i = 0; i < numEntry; i++) {
			if (i == numEntry - 1) {
				res += this->listEntry[i + 1]._eEntry.getContent(0, (n - 15 * i - 1) * 2);
				break;
			}
			res += this->listEntry[i + 1]._eEntry.getContent();
		}
		return res;
	};

	//ham lay password
	string getFilePassword() {
		string res;
		int n = this->listEntry[0]._mEntry.getNameLength();
		int n1 = this->listEntry[0]._mEntry.getPassWordLength();
		uint16_t numEntry = this->listEntry[0]._mEntry.getNumExtraEntry();
		for (int i = 0; i < numEntry; i++) {
			if (i == numEntry - 1) {
				res += this->listEntry[i + 1]._eEntry.getContent(0, (n + n1 - 1 - 15 * (i)) * 2);
				break;
			}
			res += this->listEntry[i + 1]._eEntry.getContent();

		}
		res = res.substr(n, res.length());
		return res;
	}

	//Ham kiem tra password
	bool checkPassword(string value) {
		return this->getFilePassword() == sha256(value);
	}

	Entry convertEntry(string volName, int start, uint16_t& n) {
		_Entry A;
		BYTE* res = new BYTE[512];
		DWORD bytesRead;
		HANDLE device = NULL;
		bool flag;
		wstring temp = wstring(volName.begin(), volName.end());
		LPCWSTR fileN = temp.c_str();
		device = CreateFile(fileN,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		SetFilePointer(device, start, NULL, FILE_BEGIN);
		flag = ReadFile(device, res, 512, &bytesRead, NULL);
		CloseHandle(device);

		A._mEntry = A._mEntry.convertMainEntry(res, 0);
		this->listEntry.push_back(A);
		uint16_t numEx = A._mEntry.getNumExtraEntry();
		n = numEx + 1;
		if (numEx < 16) {
			for (uint16_t i = 0; i < numEx; i++) {
				A._eEntry = A._eEntry.convertExtraEntry(res, (i + 1) * 32);
				this->listEntry.push_back(A);
			}
		}
		else {
			uint16_t tmp = (numEx - 15);
			uint16_t numSec = tmp / 16;
			if (tmp % 16 != 0)
				numSec += 1;
			for (int i = 0; i < numSec; i++) {
				//doc 512 byte
				device = CreateFile(fileN,
					GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					0,
					NULL);
				SetFilePointer(device, start + 512 * (i + 1), NULL, FILE_BEGIN);
				flag = ReadFile(device, res, 512, &bytesRead, NULL);
				CloseHandle(device);

				//end
				if (tmp >= 16) {
					for (uint16_t i = 0; i < 16; i++) {
						A._eEntry = A._eEntry.convertExtraEntry(res, (start + i + 1) * 32);
						this->listEntry.push_back(A);
					}
					tmp -= 16;
				}
				else {
					for (uint16_t i = 0; i < tmp; i++) {
						A._eEntry = A._eEntry.convertExtraEntry(res, (start + i + 1) * 32);
						this->listEntry.push_back(A);
					}
				}
			}
		}
		return *this;
	}
};

