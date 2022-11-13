#include "mainEntry.h"

mainEntry::mainEntry() {
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

 mainEntry::mainEntry(string fileName) {
	for (int i = 0; i < 6; i++) {
		this->_MainName[i] = BYTE(int(fileName[i]));
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
	this->_nameLength = uint16_t(fileName.length());
	this->_numEntryChild = uint16_t(0);
	this->_numExtraEntry = uint16_t((fileName.length() - 6) / 15);
	for (int i = 0; i < 6; i++) {
		this->_Null[i] = uint16_t(0);
	}
}

 mainEntry mainEntry::craete_mainEntry(string fileName, string pass,
	 BYTE* tg, uint32_t _fileSize, uint16_t _startCluster, uint16_t _numEntryChild, bool _status) {
	 for (int i = 0; i < 6; i++) {
		 this->_MainName[i] = BYTE(int(fileName[i]));
	 }
	 this->_status = 1;
	 this->_checkDel = 1;
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
	 uint16_t n = (fileName.length() + pass.length())/15;
	 if ((fileName.length() + pass.length()) % 15 != 0)
		 n += 1;
	 this->_numExtraEntry = uint16_t(n);
	 for (int i = 0; i < 6; i++) {
		 this->_Null[i] = uint16_t(0);
	 }

	 return *this;
 }

 bool mainEntry::getCheckDelete() {
	 return this->_checkDel;
 }
 bool mainEntry::getStatus() {
	 return this->_status;
 }

 uint32_t mainEntry::getFileSize() {
	 return this->_fileSize;
 }

 uint16_t mainEntry::getStartCluster() {
	 return this->_startCluster;
 }

 uint16_t mainEntry::getNameLength() {
	 return this->_nameLength;
 }
 uint16_t mainEntry::getPassWordLength() {
	 return this->_passWordLength;
 }
 uint16_t mainEntry::getNumEntryChild() {
	 return this->_numEntryChild;
 }

 string mainEntry::getName() {
	 string res = "";
	 for (int i = 0; i < this->_nameLength; i++) {
		 res += this->_MainName[i];
	 }
	 return res;
 }

int mainEntry::get2Byte() {
	return int(this->_MainName[1]);
 }

//ham ghi entry vao file
void mainEntry::writeEntry(FILE* f) {
	fwrite(&(this->_MainName), sizeof(this->_MainName), 6, f);
	fwrite(&(this->_checkDel), sizeof(this->_checkDel), 1, f);
	fwrite(&(this->_status), sizeof(this->_status), 1, f);
	fwrite(&(this->_UpdateTime), sizeof(this->_UpdateTime), 2, f);
	fwrite(&(this->_UpdateDate), sizeof(this->_UpdateDate), 2, f);
	fwrite(&(this->_fileSize), sizeof(this->_fileSize), 1, f);
	fwrite(&(this->_startCluster), sizeof(this->_startCluster), 1, f);
	fwrite(&(this->_nameLength), sizeof(this->_nameLength), 1, f);
	fwrite(&(this->_passWordLength), sizeof(this->_passWordLength), 1, f);
	fwrite(&(this->_numEntryChild), sizeof(this->_numEntryChild), 1, f);
	fwrite(&(this->_numExtraEntry), sizeof(this->_numExtraEntry), 1, f);
	fwrite(&(this->_Null), sizeof(this->_Null), 6, f);
}

void mainEntry::setStartCluster(int pos) {
	this->_startCluster = uint16_t(pos);
}

uint16_t mainEntry::getNumExtraEntry() {
	return this->_numExtraEntry;
}

void mainEntry::setPassWordLength(string pass) {
	this->_passWordLength = uint16_t(pass.length());
}

union u32 {
	uint32_t a;
	BYTE b[4];
};

union boolByte {
	bool a;
	BYTE b;
};

union u16
{
	uint16_t a;
	BYTE b[2];
};
BYTE* mainEntry::mainEntryToBYTE() {
	u32 a;
	boolByte b;
	u16 c;
	int n = 0;
	BYTE* res = new BYTE[32];
	for (int i = 0; i < 6; i++) {
		res[i] = this->_MainName[i];
		n += 1;
	}
	b.a = this->_checkDel;
	res[6] = b.b;
	b.a = this->_status;
	res[7] = b.b;

	res[8] = this->_UpdateTime[0];
	res[9] = this->_UpdateTime[1];
	res[10] = this->_UpdateDate[0];
	res[11] = this->_UpdateDate[1];
	n += 6;
	a.a = this->_fileSize;
	for (int i = 0; i < 4; i++) {
		res[n] = a.b[i];
		n++;
	}
	c.a = this->_startCluster;
	for (int i = 0; i < 2; i++) {
		res[n] = a.b[i];
		n++;
	}
	c.a = this->_nameLength;
	for (int i = 0; i < 2; i++) {
		res[n] = a.b[i];
		n++;
	}
	c.a = this->_passWordLength;
	for (int i = 0; i < 2; i++) {
		res[n] = a.b[i];
		n++;
	}
	c.a = this->_numEntryChild;
	for (int i = 0; i < 2; i++) {
		res[n] = a.b[i];
		n++;
	}
	c.a = this->_numExtraEntry;
	for (int i = 0; i < 2; i++) {
		res[n] = a.b[i];
		n++;
	}
}