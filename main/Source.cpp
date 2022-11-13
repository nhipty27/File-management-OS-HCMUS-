#include "Header.h"

//Ham doc 512bytes/ 1 sector
BYTE* readBlock(uint32_t offset, string fileName, uint32_t num=512) {
	DWORD bytesRead;
	HANDLE device = NULL;
	bool flag;
	wstring temp = wstring(fileName.begin(), fileName.end());
	LPCWSTR fileN = temp.c_str();
	BYTE* res = new BYTE[512];
	device = CreateFile(fileN,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	SetFilePointer(device, offset, NULL, FILE_BEGIN);
	flag = ReadFile(device, res, 512, &bytesRead, NULL);
	CloseHandle(device);
	if (flag) {
		return res;
	}
	return NULL;
}

//Ham ghi 512bytes/ 1 sector
bool writeBlock(int offset, BYTE value[], string fileName, uint32_t num=512) {
	DWORD bytesRead;
	HANDLE hFile;
	bool flag;
	wstring temp = wstring(fileName.begin(), fileName.end());
	LPCWSTR res = temp.c_str();
	hFile = CreateFile(res,
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
	flag = WriteFile(hFile, value, 512, &bytesRead, NULL);
	CloseHandle(hFile);
	return flag;
}

LPCWSTR convertStringToLPCWSTR(string val) {
	wstring temp = wstring(val.begin(), val.end());
	LPCWSTR res = temp.c_str();
	return res;
}

//Ham tao volume
void initEmptyVool(string name, int volumeSize) {
	FILE* f = fopen(name.c_str(), "wb+");
	BYTE buffer[512];
	for (int i = 0; i < 512; i++) {
		buffer[i] = BYTE(0);
	}

	for (int i = 0; i < (volumeSize * 1024 * 1024) / 512; i++) {
		fwrite(buffer, 512, 1, f);
	}
	fclose(f);
}

//Them thuoc tinh cho volume
void initVolume(string name, string passWord, int volumeSize) {
	initEmptyVool(name, volumeSize);

	BootSector* boot = new BootSector(volumeSize, passWord);
	if (!boot->writeBootsector(name)) {
		cout << "loi!!! khong the tao boot sector" << endl;
		return;
	}

	uint16_t* buffer = new uint16_t[256];
	for (int i = 0; i < 256; i++) {
		buffer[i] = 0;
	}

	BYTE* fatValue = new BYTE[512];
	for (int i = 0; i < 512; i++)
	{
		fatValue[i] = *((BYTE*)buffer + i);
	}

	FatTable fatTable(boot->fatSize());

	for (int i = 512; i <= fatTable.size(); i += 512) {
		writeBlock(i, fatValue, name);
	}
	cout << "Tao vol thanh cong" << endl;
}

//doc bootsector
BootSector* readBootSector(string name) {
	BootSector* boot = new BootSector();
	boot->convertBootsector(name);
	return boot;
}

//Doc fat table.
FatTable readFatTable( string volName) {

	BootSector* boot = new BootSector();
	boot->readBootsector(volName);
	uint32_t fatSize = boot->fatSize();
	FatTable fat(fatSize);
	
	int n = fatSize / 512;
	if (fatSize % 512 != 0)
		n += 1;
	int totalFatItem = 0;
	int count = 0;
	for (int i = 0; i < n; i++) {
		BYTE* tmpFat = new BYTE[512];
		count = fatSize - totalFatItem;
		tmpFat = readBlock((i+1)*512, volName);
		if (count < 256) {
			uint16_t* res1= new uint16_t[count];
			for (int i = 0; i < 256; i++) {
				res1[i] = *((uint16_t*)tmpFat+i);
				fat.set_value_at_index(res1[i], totalFatItem + i);
			}
			totalFatItem += count;
		}
		else {
			uint16_t res1[256];
			for (int i = 0; i < 256; i++) {
				res1[i] = *((uint16_t*)tmpFat + i);
				fat.set_value_at_index(res1[i], totalFatItem + i);
			}
			totalFatItem += 256;
		}
	}
	return fat;
}


void deleteListEntry(vector<Entry*>& entry)
{
	for (int i = 0; i < entry.size(); ++i)
	{
		delete entry[i];
	}
}

bool changePassVolumn(BootSector* bootSector, string curPass, string passWord)
{
	bool successful = bootSector->checkPassword(curPass);
	if (successful)
	{
		bootSector->setPassWord(passWord);
		bootSector->setPassSize(passWord.length());
		return true;
	}
	return false;
}

bool checkPassVolumn(BootSector* bootSector, string passWord) {
	return bootSector->checkPassword(passWord);
}

void PassAction(string volName, int option) {
	string password, confirmPass;
	BootSector* b = new BootSector();
	b = readBootSector(volName);
	cin.ignore();
	string newPass = "";
	switch (option)
	{
	case 1:
		while (1) {
			cout << "Nhap mat khau: " << endl;
			getline(cin, password);
			cout << "Nhap lai mat khau: " << endl;
			getline(cin, confirmPass);
			if (password != confirmPass) {
				cout << "Mat khau khong chinh xac. Moi nhap lai!" << endl;
			}
			else break;
		}


		b->setPassWord(password);
		b->writeBootsector(volName);
		cout << "Thiet lap mat khau thanh cong" << endl;
		break;


	case 2:
		while (1) {
			cout << "Nhap mat khau mat khau hien tai: " << endl;
			getline(cin, password);
			cout << "Nhap lai mat khau hien tai: " << endl;
			getline(cin, confirmPass);

			if (password != confirmPass) {
				cout << "Mat khau khong chinh xac. Moi nhap lai!" << endl;
			}
			else break;
		}
		cout << "Nhap mat khau moi: " << endl;
		getline(cin, newPass);

		if (changePassVolumn(b, password, newPass)) {
			b->writeBootsector(volName);
			cout << "Doi mat khau thanh cong" << endl;
		}
		else cout << "Doi mat khau khong thanh cong. Mat khau hien tai khong trung khop" << endl;

		break;
	case 3:
		cout << "Nhap mat khau: ";
		getline(cin, password);
		if (checkPassVolumn(b, password)) {
			cout << "Mat khau chinh xac";
		}
		else cout << "Mat khau khong chinh xac";
		break;
	default:
		break;
	}
}

int getClusterStatus(FatTable fat) {
	if (fat.get_value_at_index(0) == 0)
		return 0;
	if (fat.get_value_at_index(fat.size() - 1) == 0)
		return fat.size() - 1;
	for (int i = 0; i < fat.size()-1; i++) {
		if (fat.get_value_at_index(i) == 1 && fat.get_value_at_index(i+1)==0) {
			return i;
		}
	}
}

//Ghi entry
void writeEntry(string volname,BootSector* boot, FatTable fat, Entry entry) {
	int pos = getClusterStatus(fat);
	int sum = 512 + boot->fatSize() + boot->numberOfEntry() * 32;
	uint32_t sector = (sum ) / 512;
	int index = (sum/16) / 32 ; 
	BYTE* currSector = new BYTE[512];
	currSector = readBlock(sector * 512, volname);
	int i = 0;
	for (; i < 16; i++) {
		if (currSector[i * 32] == BYTE(0)) {
			break;
		}
	}
	for (int i = 0; i < entry.getList().size(); i++) {
		DWORD bytesRead;
		HANDLE hFile;
		bool flag;
		wstring temp = wstring(volname.begin(), volname.end());
		LPCWSTR res = temp.c_str();
		hFile = CreateFile(res,
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		SetFilePointer(hFile, (sum +32*i), NULL, FILE_BEGIN);
		if(i==0)
			flag = WriteFile(hFile, &entry.getList()[i]._mEntry, 32, &bytesRead, NULL);
		else 
			flag = WriteFile(hFile, &entry.getList()[i]._eEntry, 32, &bytesRead, NULL);
		CloseHandle(hFile);
	}

	//update boot , fat
	boot->setNumberOfEntry(boot->numberOfEntry() + entry.getList().size());
	boot->writeBootsector(volname);

	fat.set_value_at_index(uint16_t(1), uint32_t(boot->numberOfEntry() / 16 / 4) );
	
}

//Doc tat ca entry
vector<Entry> readEntry(string volname, BootSector* boot) {
	vector<Entry>listEntry;
	convertDataType tmp;
	int posData = 512 + boot->fatSize();
	int numEntry = boot->numberOfEntry();
	int count = 0;
	while (count < numEntry) {
		Entry A;
		uint16_t n = 0;
		A.convertEntry(volname, posData + count * 32, n);
		count += n;
		listEntry.push_back(A);
	}
	return listEntry;
}

//Ghi de entry
void overwriteEntry(string volname, BootSector* boot, Entry entry, int numPreEntry)
{
	int sum = 512 + boot->fatSize() + numPreEntry * 32;

	for (int i = 0; i < entry.getList().size(); i++) {
		DWORD bytesRead;
		HANDLE hFile;
		bool flag;
		wstring temp = wstring(volname.begin(), volname.end());
		LPCWSTR res = temp.c_str();
		hFile = CreateFile(res,
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		SetFilePointer(hFile, (sum + 32 * i), NULL, FILE_BEGIN);
		if (i == 0)
			flag = WriteFile(hFile, &entry.getList()[i]._mEntry, 32, &bytesRead, NULL);
		else
			flag = WriteFile(hFile, &entry.getList()[i]._eEntry, 32, &bytesRead, NULL);
		CloseHandle(hFile);
	}
}

int changePassFile(vector<Entry>& list, string fileName, string curPass, string newPass)
{
	int indexChange = -1;
	for (int i = 0; i < list.size(); i++) {

		if (fileName == list[i].getFileName())
		{
			if (list[i].checkPassword(curPass))
			{
				list[i].setPassWord(newPass);
				indexChange = i;
				break;
			}
		}

	}
	return indexChange;
}

void changePassWordFile(string volname, BootSector* boot, vector<Entry>& list, string fileName, string curPass, string newPass)
{
	int index = changePassFile(list, fileName, curPass, newPass);
	if (index == 0)
	{
		overwriteEntry(volname, boot, list[index], index);
	}
	else
	{
		int count = 0;
		for (int i = 0; i < index; i++)
		{
			count = count + 1 + list[i].getList()[0]._mEntry.getNumExtraEntry();
		}
		overwriteEntry(volname, boot, list[index], count);
	}

}

int deleteNomal(vector<Entry>& list, string fileName, string passWord)
{
	int index = -1;
	for (int i = 0; i < list.size(); i++) {

		if (fileName == list[i].getFileName())
		{
			if (list[i].checkPassword(passWord))
			{
				list[i].getList()[0]._mEntry.setDeleted(0);
				string name = fileName;
				name[0] = 0xE5;
				list[i].setFileName(name);
				index = i;
				break;
			}
		}

	}
	return index;
}

void deleteFileNormal(string volName, string fileName, string passWord)
{
	BootSector* boot = new BootSector();
	boot->convertBootsector(volName);
	vector<Entry>list;
	list = readEntry(volName, boot);

	int index = deleteNomal(list, fileName, passWord);
	if (index != -1)
	{
		if (index == 0)
		{
			overwriteEntry(volName, boot, list[index], index);
		}
		else
		{
			int count = 0;
			for (int i = 0; i < index; i++)
			{
				count = count + 1 + list[i].getList()[0]._mEntry.getNumExtraEntry();
			}
			overwriteEntry(volName, boot, list[index], count);
		}
	}
}

int deleteForever(vector<Entry>& list, string fileName, string passWord)
{
	int index = -1;
	for (int i = 0; i < list.size(); i++) {

		if (fileName == list[i].getFileName())
		{
			if (list[i].checkPassword(passWord))
			{
				index = i;
				break;
			}
		}

	}
	return index;
}

void deleteFileForever(string volName, string fileName, string passWord)
{
	BootSector* boot = new BootSector();
	boot->convertBootsector(volName);
	vector<Entry>list;
	list = readEntry(volName, boot);

	int index = deleteForever(list, fileName, passWord);
	if (index != -1)
	{
		int count = 0;
		for (int i = 0; i < index; i++)
		{
			count = count + 1 + list[i].getList()[0]._mEntry.getNumExtraEntry();
		}

		for (int i = index + 1; i < list.size(); i++)
		{
			overwriteEntry(volName, boot, list[i], count);
		}

		//so cluster da luu entry
		uint64_t numPreClusterEntry = (uint64_t(ceil(boot->numberOfEntry() * 1.0 / 128)));
		uint64_t numEntryDel = list[index].getList().size();
		boot->setNumberOfEntry(boot->numberOfEntry() - numEntryDel);
		boot->writeBootsector(volName);

		//So cluster co the bi anh huong gia tri
		uint64_t numClusterAfter = boot->numberOfEntry() / 64;
		uint64_t startCluster = numPreClusterEntry / 64;
		uint64_t n = numPreClusterEntry - numClusterAfter;
		if (numClusterAfter % 64 == 0)
			n++;
		for (; n > 0; n--)
		{
			uint16_t fat_value = 0;
			FILE* f;
			f = fopen(volName.c_str(), "rb+");
			fseek(f, 512 + startCluster * 2, SEEK_SET);
			startCluster--;
			fwrite((char*)&fat_value, 2, 1, f);
			fclose(f);
		}
	}
}

//ham lay ten file tu path
string getFileName(string path) {
	int i = 0;
	while (true) {
		int n = path.find("\\");
		if (n<0 || n> path.length())
			break;
		path.erase(0, n + 1);
	}
	return path;
}

//ham lau kich thuoc file
uint32_t getFileSize(string sourcePath) {
	FILE* fp; // tạo con trỏ tới file
	fp = fopen(sourcePath.c_str(), "rb"); // đọc file dưới dạng binary
	if (!fp) {
		return 0;
	}
	fseek(fp, 0, SEEK_END); // đọc file từ đầu tới cuối
	return uint32_t(ftell(fp)); // trả về vị trí cuối cùng
}

//Ham tim vi tri cluster trong
bool checkSapce(int numberOfCluster, FatTable fatTable, int k) {
	int noClus = numberOfCluster;
	bool found = true;
	for (int i = k; numberOfCluster > 0; i--, numberOfCluster--)
		if (fatTable.get_value_at_index(i) != 0)
			found = false;
	if (!found)
		for (int i = k; noClus > 0; i--, noClus--)
			if (fatTable.get_value_at_index(i) == 1)
				return false;
	return true;
}

int FindEmptyCluster(FatTable& fatTable, uint32_t size) {
	uint32_t numberOfClusters = size / (4*512);
	if (size % (4 * 512) != 0)
		numberOfClusters += 1;
	int positionEnd = -1;
	for (int i = fatTable.size() - 1; i >= 0; i--) {
		if (fatTable.get_value_at_index(i) == 0) {
			if (checkSapce(numberOfClusters, fatTable, i)) {
				positionEnd = i;
				break;
			}
			else {
				i -= numberOfClusters;
			}
		}
	}

	int positionStart = positionEnd - numberOfClusters;
	if (size % (4 * 512) != 0)
		positionStart += 1;
	//cap nhat lai FAT
	for (int i = positionStart; i <= positionEnd; i++) {
		fatTable.set_value_at_index(1,i);
	}
	return positionStart;
}

//Inport file
vector<Entry> importFileNomal(string volName, string filePath, FatTable &fatTable, string password, int &posClusterStart, int &posClusterEnd) {
	vector<Entry> entries;
	string fileName = getFileName(filePath);
	bool isFolder = false;
	uint32_t fileSize = getFileSize(filePath);
	posClusterStart = FindEmptyCluster(fatTable, fileSize);
	int n = fileSize / (4 * 512);
	posClusterEnd = posClusterStart + n;
	if (fileSize % (4 * 512) != 0)
		n += 1;
	BYTE* res = new BYTE[512];
	int startFile = 0;
	int start = 512 + fatTable.size() + 2048 * posClusterStart;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 4; j++) {
			res = readBlock(startFile, filePath);
			writeBlock(start, res, volName);
			start += 512;
			startFile += 512;
			if (startFile > fileSize)
				break;
		}
	}
	Entry entry(fileName, password, fileSize, posClusterStart, 0, false);
	entries.push_back(entry);
	return entries;
}

void importFolder() {

}

bool importFile(string volName, BootSector* boot, FatTable fatTable, string filePath, string password) {
	vector<Entry> entries;
	int posClusterStart, posClusterEnd;
	//Thong tin day du cua File/Folder
	WIN32_FIND_DATAA fileData;
	HANDLE handle = nullptr;
	bool isFolder = false;
	string pathOutside;
	string parentPath;

	deque<pair<Entry, string>> s;

	handle = FindFirstFileA(filePath.c_str(), &fileData);
	FindClose(handle);
	if (handle == INVALID_HANDLE_VALUE)
		return false;
	if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		isFolder = true;
		string path = filePath;
		pathOutside = filePath;
		string fileName = getFileName(filePath);
		Entry entry(fileName, password, 0, 0, 0);
		entries.push_back(entry);
		pathOutside += "\\*";
		s.push_back(make_pair(entry, pathOutside));
		while (!s.empty()) {
			Entry folder = s.front().first;
			parentPath = s.front().second;
			s.pop_front();
			handle = FindFirstFileA(parentPath.c_str(), &fileData);
			if (handle == INVALID_HANDLE_VALUE)
				return false;
			//Ignore 2 special directory . and ..
			FindNextFileA(handle, &fileData);
			//Check xem folder co trong hay khong
			bool notEmptyFolder = FindNextFileA(handle, &fileData);
			if (!notEmptyFolder)
				break;
			do {
				//path trong volume
				path = fileName
					+ parentPath.substr(filePath.length(), parentPath.length() - filePath.length() - 1)
					+ fileData.cFileName;
				//Neu la folder
				if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					isFolder = true;
					pathOutside = parentPath.substr(0, parentPath.length() - 1) + fileData.cFileName + "\\*";
					Entry e (getFileName(path), password, 0, 0, 0, true);
					entries.push_back(e);
					s.push_back(make_pair(e, pathOutside));
				}
				//Nguoc lai neu la file
				else {
					string path = filePath;
					pathOutside = parentPath.substr(0, parentPath.length() - 1) + fileData.cFileName;
					string fileName = getFileName(pathOutside);
					isFolder = false;
					uint32_t fileSize = getFileSize(pathOutside);
					posClusterStart = FindEmptyCluster(fatTable, fileSize);
					int n = fileSize / (4*512);
					posClusterEnd = posClusterStart + n;
					if (fileSize % (4 * 512) != 0)
						n += 1;
					BYTE* res = new BYTE[512];
					int startFile = 0;
					int start = 512 + fatTable.size() + 2048 * posClusterStart;
					for (int i = 0; i < n; i++) {
						for (int j = 0; j < 4; j++) {
							res = readBlock(startFile, filePath);
							writeBlock(start, res, volName);
							start += 512;
							startFile += 512;
							if (startFile > fileSize)
								break;
						}
					}
					Entry entry(fileName, password, fileSize, posClusterStart, 0, false);
					entries.push_back(entry);
				}
			} while (FindNextFileA(handle, &fileData));
			FindClose(handle);
		}
	}
	else {
		/*string path = filePath;
		string fileName = getFileName(filePath);
		bool isFolder = false;
		uint32_t fileSize = getFileSize(filePath);
		posClusterStart = FindEmptyCluster(fatTable, fileSize);
		int n = fileSize / (4*512);
		posClusterEnd = posClusterStart + n;
		if (fileSize % (4 * 512) != 0)
			n += 1;
		BYTE* res = new BYTE[512];
		int startFile = 0;
		int start = 512 + fatTable.size() + 2048 * posClusterStart;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < 4; j++) {
				res = readBlock(startFile, filePath);
				writeBlock(start, res, volName);
				start += 512;
				startFile += 512;
				if (startFile > fileSize)
					break;
			}
		}
		Entry entry(fileName, password, fileSize, posClusterStart, 0, false);
		entries.push_back(entry);*/
		entries = importFileNomal(volName, filePath, fatTable, password, posClusterStart, posClusterEnd);
	}

	//cap nhat lai so entry con cua folder
	for (int i = 0; i < s.size(); i++) {
		if ((s[i].first).getList()[0]._mEntry.getStatus() == 0) {
			int count = 0;
			for (int j = i+1; j < s.size(); j++) {
				if ((s[j].first).getList()[0]._mEntry.getStatus() == 0)
					break;
				count++;
			}
			(s[i].first).getList()[0]._mEntry.setNumEntryChild(count);
		}
	}
	//cap nhat lai entry trong volume
	for (int i = 0; i < entries.size(); i++) {
		writeEntry(volName, boot, fatTable, entries[i]);
		boot->setNumberOfEntry(boot->numberOfEntry() + (entries[0]).getList()[0]._mEntry.getNumEntryChild()+1); 
	}
	//cap nhat lai Bootsector
	boot->writeBootsector(volName);

	//cap nhat lai FatTalbe
	fatTable.writeFatTable(volName, posClusterStart, posClusterEnd);
	return true;
}
