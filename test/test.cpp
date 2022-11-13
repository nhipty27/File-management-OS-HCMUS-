#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <vector>
#include <ctime>
#include <time.h>
#include "sha256.h"
#include <fstream>

using namespace std;
class mainEntry
{
private:
	BYTE _MainName[6];
	bool _checkDel; // folder or file 1: file 0:folder
	bool _status; // is deleted 0: deleted
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
	string getName() {
		string res = "";
		for (int i = 0; i < this->_nameLength; i++) {
			res += this->_MainName[i];
		}
		return res;
	}
	uint16_t getPassword() { return this->_passWordLength; }
	uint16_t getLength() { return this->_nameLength; };
	bool checkDelelete() { return _checkDel; }
	void setStatus(bool value) { _status = value; }
	bool getStatus() { return _status;}
	void write(string name) {
		//string name = "phanthiyennhi.dat";
		for (int i = 0; i < 6; i++) {
			this->_MainName[i] = BYTE(int(name[i]));
		}
		this->_status = 1;
		this->_checkDel = 1;
		this->_UpdateTime[0] = 22;
		this->_UpdateTime[1] = 74;
		this->_UpdateDate[0] = 187;
		this->_UpdateDate[0] = 52;
		this->_fileSize = int(100);
		this->_startCluster = uint16_t(2);
		this->_passWordLength = uint16_t(16);
		this->_nameLength = uint16_t(name.length());
		this->_numEntryChild = uint16_t(0);
		this->_numExtraEntry = uint16_t((name.length() - 6) / 15 );
		for (int i = 0; i < 6; i++) {
			this->_Null[i] = uint16_t(0);
		}
	}
	void read() {
		string name = "phanthiyennhi.dat";
		for (int i = 0; i < 6; i++) {
			this->_MainName[i] = BYTE(int(name[i]));
		}
		this->_status = 1;
		this->_checkDel = 1;
		this->_UpdateTime[0] = 22;
		this->_UpdateTime[1] = 74;
		this->_UpdateDate[0] = 187;
		this->_UpdateDate[0] = 52;
		this->_fileSize = int(100);
		this->_startCluster = uint16_t(2);
		this->_passWordLength = uint16_t(10);
		this->_nameLength = uint16_t(name.length());
		this->_numEntryChild = uint16_t(0);
		this->_numExtraEntry = (name.length() / 15+1);
		for (int i = 0; i < 7; i++) {
			this->_Null[i] = uint16_t(0);
		}
	}
	bool checkPassword(string value);
	int get22B() {
		return int(this->_MainName[1])  + int(this->_MainName[0]);
	}
	
};

class extraEntry {
private:
	uint16_t _pos;
	BYTE _Content[30];

public:
	void readEx(string& val, int length) {

	}

	uint16_t getPos() { return _pos; };

	extraEntry createE(string val, int numEntry) {
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
	void writeEx(string val, int numEntry) {
		this->_pos = numEntry+1;
		int i = 0;
		int posVal = numEntry * 15;
		while (i < 30 ) {
			this->_Content[i] = BYTE(int(val[posVal]));
			this->_Content[i+1] = BYTE(int(0));
			posVal += 1;
			i += 2;
			if (posVal == val.length())
				break;
		}
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
	void chang(string pass, int pos) {
		this->_pos = pos;
		int posVal = pos * 15;
		int i = 0;
		while (i < 30) {
			this->_Content[i] = BYTE(int(pass[posVal]));
			this->_Content[i + 1] = BYTE(int(0));
			posVal += 1;
			i += 2;
			if (posVal == pass.length())
				break;
		}
	}

};

union _Entry
{
	mainEntry _mEntry;
	extraEntry _eEntry;
};

class Entry {
private:
	vector<_Entry> list;
public:
	Entry(string name, string pass) {
		_Entry A;
		A._mEntry.write(name);
		list.push_back(A);
		string data = name + pass;
		if (name.length() > 6) {
			for (int i = 0; i < data.length() / 15 + 1;i++) {
				_Entry B;
				B._eEntry.writeEx(data, i);
				list.push_back(B);
			}
		}
	}
	_Entry getA() {
		return list[1];
	}

	string getFileName() {
		int n = this->list[0]._mEntry.getLength();
		string res = "";
		if (n <= 6)
			return this->list[0]._mEntry.getName();
		int i = 0;
		int numEntry = n / 15;
		if (n % 15 != 0)
			numEntry += 1;
		while (true) {
			if ((i + 1) * 15 < n) {
				res += this->list[i + 1]._eEntry.getContent();
			}
			else {
				res += this->list[i + 1]._eEntry.getContent(0, n - 15 * i);
				break;
			}
			i += 1;
			if (i >= numEntry)
				break;
		}
		return res;
	}

	string getFilePassword() {
		int n = this->list[0]._mEntry.getLength();
		int p = this->list[0]._mEntry.getPassword();
		string res = "";
		

		//vi tri entri chua password
		int numPass = n / 15;
		int i = 0;
		if (n % 15 != 0)
			numPass += 1;
		int startPos = (n - (i + 1) * 15) * 2;
		res += this->list[numPass]._eEntry.getContent(startPos, startPos + p * 2 - 1);
		if ((i + numPass) * 15 > n+p ) {
			return res;
		}
			i = numPass;
			while (true) {
				if ((i + 1) * 15 < n + p) {
					res += this->list[i + 1]._eEntry.getContent();
				}
				else {
					res += this->list[i + 1]._eEntry.getContent(0, (p- res.length() -1)*2);
					break;
				}
				i += 1;
				if (i >= numPass)
					break;
			}
		
		return res;
	}

	mainEntry getM() { return this->list[0]._mEntry; };

	void changePass(string pass) {
		uint16_t n = this->list[0]._mEntry.getLength();
		int pos = n / 15;
		if (n % 15 != 0) {
			n += 1;
		}
		for (int i = pos; i < this->list.size(); i++) {
			this->list[i]._eEntry.chang(this->list[0]._mEntry.getName()+pass, i);
		}
	}
};

union MyUnion
{
	int a;
	BYTE b[4];
};

string getFileName(string path) {
	int i = 0;
	while (true) {
		int n = path.find("\\");
		if (n<0 || n> path.length())
			break;
		path.erase(0, n+1);
	}
	return path;
}

uint32_t getFileSize(string sourcePath) {
	FILE* fp; // tạo con trỏ tới file
	fp = fopen(sourcePath.c_str(), "rb"); // đọc file dưới dạng binary
	if (!fp) {
		return 0;
	}
	fseek(fp, 0, SEEK_END); // đọc file từ đầu tới cuối
	return ftell(fp); // trả về vị trí cuối cùng
}

BYTE* readBlock(uint32_t sector, string fileName, uint32_t num=512) {
	DWORD bytesRead;
	wstring temp = wstring(fileName.begin(), fileName.end());
	LPCWSTR name = temp.c_str();
	HANDLE device = NULL;
	bool flag;
	BYTE* res = new BYTE[num];
	device = CreateFile(name,
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

int main() {
	BYTE* a = new BYTE[512];
	a = readBlock(0, "D:\\abcd.txt");
	
	return(0);
}