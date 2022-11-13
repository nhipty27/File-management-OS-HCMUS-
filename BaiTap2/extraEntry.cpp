#include "extraEntry.h"

extraEntry::extraEntry() {
	this->_pos = -1;
	int i = 0;
	while (i < 30) {
		this->_Content[i] = BYTE(int(" "));
		this->_Content[i + 1] = BYTE(int(0));
		i += 2;
	}
}
 extraEntry::extraEntry(string val, int numEntry) {
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
 }

 extraEntry extraEntry::create_extraEntry(string val, int numEntry) {
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
 uint16_t extraEntry::getPos() {
	 return this->_pos;
 }

 void extraEntry::readExtraEntry(string& val, int length) {

 }

 string extraEntry::getContent(int start, int end) {
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

 void extraEntry::writeEntry(FILE* f) {
	 fwrite(&(this->_pos), sizeof(this->_pos), 2, f);
	 fwrite(&(this->_Content), sizeof(this->_Content), 30, f);
 }

 void extraEntry::setContent(string val, int nameLength) {
	 /*int currPos = */
 }