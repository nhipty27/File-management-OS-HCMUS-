#define _CRT_SECURE_NO_WARNINGS

#include "Header.h"
#include "Entry.h"
////#include "FatTable.h"
int main()
{
	
	string name = "MyFS.dat";
	uint32_t size = 0;
	cout << "Nhap ten volume muon tao: ";
	getline(cin, name);
	cout << "Nhap kich thuoc volume(>100, theo MB): ";
	cin >> size;

	initVolume(name, size);
	cout << "Tao volume thanh cong!" << endl;
	FILE* f = nullptr;
	fopen_s(&f, name.c_str(), "r");
	BootSector* boot = readBootSector(f);
	fclose(f);

	FatTable fatTable(boot->fatSize());
	string fileName = "da.xt";
	string passWord = "123456";
	Entry A;
	A.create_Entry(fileName, passWord, 10, 0, 0, 1);

	return 0;
}