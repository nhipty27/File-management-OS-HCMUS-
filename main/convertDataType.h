#pragma once
#include <Windows.h>
#include <iostream>
#include <map>
#include <string>
using namespace std;
union int16Bit {
	uint16_t num16;
	BYTE res[2];
};

union int32Bit {
	uint32_t num32;
	BYTE res[4];
};

union int64Bit {
	uint64_t num64;
	BYTE res[8];
};

union booleanBit {
	boolean numBoolean;
	BYTE res;
};
class convertDataType {
private:
	int16Bit int16;
	int32Bit int32;
	int64Bit int64;
	booleanBit booleanBit;

public:
	convertDataType() {
		this->int16.num16 = 0;
		this->int32.num32 = 0;
		this->int64.num64 = 0;
		this->booleanBit.numBoolean = 0;
	}

	void setInt16(uint16_t num) {
		this->int16.num16 = num;
	}

	void setInt32(uint32_t num) {
		this->int32.num32 = num;
	}

	void setInt64(uint64_t num) {
		this->int64.num64 = num;
	}

	void setBool(boolean num) {
		this->booleanBit.numBoolean = num;
	}

	uint16_t convertByteToInt16(BYTE* num, int start) {
		int16Bit result;
		for (int i = 0; i < 2; i++) {
			result.res[i] = num[start + i];
		}
		return result.num16;
	}

	uint32_t convertByteToInt32(BYTE* num, int start) {
		int32Bit result;
		for (int i = 0; i < 4; i++) {
			result.res[i] = num[start + i];
		}
		return result.num32;
	}

	uint64_t convertByteToInt64(BYTE* num, int start) {
		int64Bit result;
		for (int i = 0; i < 8; i++) {
			result.res[i] = num[start + i];
		}
		return result.num64;
	}

	//boolean convertByteToBoolean(BYTE* num, int start) {
	//	booleanBit result;
	//	result.res = num[start];
	//	return result.numBoolean;
	//}

	string convertByteToString(BYTE* num, int start) {
		string res;
		for (int i = 0; i < 64; i++) {
			res += num[start + i];
		}
		return res;
	}

	BYTE* string_to_hex(string str)
	{
		uint32_t  n = str.length() / 2;
		BYTE* result = new BYTE[n];

		uint32_t k = 0;
		for (int i = 0; i <= str.length() - 2; i = i + 2)
		{
			map<string, uint32_t> dict = {
				{"1",1},{"2",2},{"3",3},{"4",4},{"5",5},
				{"6",6},{"7",7},{"8",8},{"9",9},{"a",10},
				{"b",11},{"c",12},{"d",13},{"e",14},{"f",15}
			};
			string hex1 = "";
			hex1 += str[i];

			string hex2 = "";
			hex2 += str[i + 1];

			uint32_t hexValue = dict[hex1] * 16 + dict[hex2];
			result[k] = BYTE(hexValue);
			k++;
		}
		return result;
	}
};


//BYTE* convertInt16ToByte(uint16_t num) {
//	int16Bit result;
//	result.num16 = num;
//	return result.res;
//}



//BYTE* convertInt32ToByte(uint32_t num) {
//	int32Bit result;
//	result.num32 = num;
//	return result.res;
//}



//BYTE* convertInt64ToByte(uint64_t num) {
//	int64Bit result;
//	result.num64 = num;
//	return result.res;
//}



//BYTE* convertBooleanToByte(boolean num) {
//	booleanBit result;
//	result.numBoolean = num;
//	return result.res;
//}

