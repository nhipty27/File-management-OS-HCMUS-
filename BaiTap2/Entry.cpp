#include "Entry.h"


Entry Entry::create_Entry(string name, string passWord,
	uint32_t _fileSize, uint16_t _startCluster, uint16_t _numEntryChild, bool _status) {
	dateTime tmp;
	tmp.getCurrentTime();
	BYTE* tg = new BYTE[4];
	tg = tmp.convert_dateTime_to_hex_with_little_endian();
	_Entry A;
	passWord = sha256(passWord);
	A._mEntry = A._mEntry.craete_mainEntry(name, passWord, tg, _fileSize, _startCluster, _numEntryChild, _status);
	this->listEntry.push_back(A);
	string data = name + passWord;
	if (name.length() > 6) {
		for (int i = 0; i < data.length() / 15 + 1; i++) {
			_Entry B;
			B._eEntry = B._eEntry.create_extraEntry(data, i);
			this->listEntry.push_back(B);
		}
	}
	return *this;
}

string Entry::getFileName() {
	int n = this->listEntry[0]._mEntry.getNameLength();
	string res = "";
	if (n <= 6)
		return this->listEntry[0]._mEntry.getName();
	int i = 0;
	int numEntry = n / 15;
	if (n % 15 != 0)
		numEntry += 1;
	while (true) {
		if ((i + 1) * 15 < n) {
			res += this->listEntry[i + 1]._eEntry.getContent();
		}
		else {
			res += this->listEntry[i + 1]._eEntry.getContent(0, n - 15 * i);
			break;
		}
		i += 1;
		if (i >= numEntry)
			break;
	}
	return res;
}

string Entry::getFilePassword() {
	int n = this->listEntry[0]._mEntry.getNameLength();
	int p = this->listEntry[0]._mEntry.getPassWordLength();
	string res = "";


	//vi tri entri chua password
	int numPass = n / 15;
	int i = 0;
	if (n % 15 != 0)
		numPass += 1;
	int startPos = (n - (i + 1) * 15) * 2;
	res += this->listEntry[numPass]._eEntry.getContent(startPos, startPos + p * 2 - 1);
	if ((i + numPass) * 15 > n + p) {
		return res;
	}
	i = numPass;
	while (true) {
		if ((i + 1) * 15 < n + p) {
			res += this->listEntry[i + 1]._eEntry.getContent();
		}
		else {
			res += this->listEntry[i + 1]._eEntry.getContent(0, (p - res.length() - 1) * 2);
			break;
		}
		i += 1;
		if (i >= numPass)
			break;
	}

	return res;

}

//Ghi entry moi vao file
//bool Entry::add(FILE* f, BootSector* boot, FatTable& fatTable) {
//	uint32_t position = -1;
//	uint32_t index = 0; //so cluster day 
//	int entrySize = this->listEntry.size();
//	for (int i = 0; i < fatTable.size(); i++)
//	{
//		if (fatTable.get_value_at_index(i) == 0)
//		{
//			index = i;
//			position = 0;
//			fatTable.set_value_at_index(1, position);
//			break;
//		}
//
//		uint32_t count = 0;
//		uint32_t numEntry = boot->numberOfEntry(); //so luong entry hien tai.
//		if (fatTable.get_value_at_index(i) == 1 && fatTable.get_value_at_index(i + 1) == 0)
//		{
//			uint32_t curNumEntry = numEntry - 16*4 * i;  //so luong entry trong cua cluster hien tai
//			if (curNumEntry < 16)
//			{
//				position = curNumEntry;
//				index = i;
//			}
//			break;
//		}
//	}
//
//	//neu tat ca cluster da chua file/folder
//	if (position == -1) {
//		for (int i = 0; i < fatTable.size(); i++) {
//			if (fatTable.get_value_at_index(i) == 2) {
//				position = i;
//				fatTable.set_value_at_index(1, position);
//				break;
//			}
//		}
//		return false;
//	}
//		
//	for (int i = 0; i < entrySize; i++) {
//		fseek(f, 512 + fatTable.size() + 2048 * index + 32 * (position +i), SEEK_SET);
//		if (i == 0) {
//			this->listEntry[0]._mEntry.setStartCluster(index);
//			this->listEntry[i]._mEntry.writeEntry(f);
//		}
//		else this->listEntry[i]._eEntry.writeEntry(f);
//	}
//
//	boot->setNumberOfEntry(boot->numberOfEntry() + entrySize);
//
//	boot->update(f, entrySize);
//	return true;
//}
//


bool Entry::add(string fileName, BootSector* boot, FatTable& fatTable) {
	uint32_t n = boot->numberOfEntry(); //so entry hien tai
	uint32_t cluster = n / (4*16);
	uint32_t sector = n / 16 +1 + fatTable.size()*2/512;
	LPCWSTR file = convertStringToLPCWSTR(fileName);
	/*if (fatTable.get_value_at_index(cluster) == 0)*/ {
		BYTE* res = new BYTE[512];
		res = readBlock(sector, file);
		BYTE* currEntry = this->listEntry[0]._mEntry.mainEntryToBYTE();
		for (int i = 0; i < 32; i++) {
			res[i] = currEntry[i];
		}
		writeBlock(0, res, file);
	}
}

void Entry::read(FILE* f)
{
	//BYTE* data = new BYTE[256];
	//fread(&(this->_sizeData), sizeof(this->_sizeData), 1, f);
	//fread(&(this->_nameLength), sizeof(this->_nameLength), 1, f);
	//fread(&(this->_passWordLength), sizeof(this->_passWordLength), 1, f);
	//fread(&(this->_offsetData), sizeof(this->_offsetData), 1, f);
	////fread(&(this->_fatPosition), sizeof(this->_fatPosition), 1, f);
	//fread(&(this->_check), sizeof(this->_check), 1, f);

	//string name(this->_nameLength, '\0');
	//fread(&name[0], sizeof(char), _nameLength, f);
	//this->_name = name;

	//string passWord(this->_passWordLength, '\0');
	//fread(&passWord, sizeof(char), _passWordLength, f);
	//this->_passWord = passWord;
	cout << "hi";
}

bool Entry::checkPassword(string value) {
	// Kiem tra string da hash
	return this->getFilePassword() == value;
}

void Entry::setNewPass(string pass) {
	this->listEntry[0]._mEntry.setPassWordLength(pass);

}