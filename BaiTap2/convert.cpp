#include "Header.h"
#include "convert.h"
#include <sstream>
#include <map>

vector<uint32_t> decToBinary(uint32_t n, uint32_t numBit)
{
	vector<uint32_t> binaryNum;
	for (int i = 0; i < numBit; i++)
		binaryNum.push_back(0);

	uint32_t i = numBit - 1;
	while (n > 0) {

		binaryNum[i] = n % 2;
		n = n / 2;
		i--;
	}

	return binaryNum;
}

void getBitsBlock(vector<uint32_t>& result, vector<uint32_t> bin, uint32_t from, uint32_t to)
{
	for (uint32_t i = from; i <= to; i++)
		result.push_back(bin[i]);
}
void getBitsBlock(vector<uint32_t>& result, vector<uint32_t> bin, uint32_t numBit)
{
	for (uint32_t i = bin.size() - numBit; i < bin.size(); i++)
		result.push_back(bin[i]);
}

uint32_t binToDec(vector<uint32_t> bin)
{
	uint32_t result = 0;
	for (int i = 0; i < bin.size(); i++)
		result += bin[i] * pow(2, bin.size() - i - 1);

	return result;
}

BYTE* create_offsets_zero(uint32_t n) {
	BYTE* offsets = new BYTE[n];
	for (int i = 0; i < n; i++) {
		offsets[i] = BYTE(0);
	}
	return offsets;
}

//BYTE* convert_dateTime_to_hex_with_little_endian(dateTime dt) {
//
//	//2 byte dau l� thoi gian, 2 byte sau la ngay thang
//	BYTE* byte = create_offsets_zero(4);
//	uint32_t numBit = 8;
//	vector<uint32_t> binSec = decToBinary(dt.tm_sec / 2, numBit);
//	vector<uint32_t> binMin = decToBinary(dt.tm_min, numBit);
//	vector<uint32_t> binHour = decToBinary(dt.tm_hour, numBit);
//
//	vector<uint32_t> binTime;
//	getBitsBlock(binTime, binHour, 5);
//	getBitsBlock(binTime, binMin, 6);
//	getBitsBlock(binTime, binSec, 5);
//
//	vector<uint32_t> binTime1;
//	getBitsBlock(binTime1, binTime, 0, 7);
//	vector<uint32_t> binTime2;
//	getBitsBlock(binTime2, binTime, 8);
//
//	uint32_t decTime1 = binToDec(binTime1);
//	uint32_t decTime2 = binToDec(binTime2);
//
//	vector<uint32_t> binDay = decToBinary(dt.tm_mday, numBit);
//	vector<uint32_t> binMon = decToBinary(dt.tm_mon, numBit);
//	vector<uint32_t> binYear = decToBinary(dt.tm_year - 1980, numBit);
//	vector<uint32_t> binDate;
//	getBitsBlock(binDate, binYear, 7);
//	getBitsBlock(binDate, binMon, 4);
//	getBitsBlock(binDate, binDay, 5);
//
//	vector<uint32_t> binDate1;
//	getBitsBlock(binDate1, binDate, 0, 7);
//	vector<uint32_t> binDate2;
//	getBitsBlock(binDate2, binDate, 8);
//
//	uint32_t decDate1 = binToDec(binDate1);
//	uint32_t decDate2 = binToDec(binDate2);
//
//	byte[0] = BYTE(decTime2);
//	byte[1] = BYTE(decTime1);
//	byte[2] = BYTE(decDate2);
//	byte[3] = BYTE(decDate1);
//	return byte;
//}

dateTime convert_hex_to_dateTime(BYTE* hex)
{

	uint32_t hexTime1 = hex[1];
	uint32_t hexTime2 = hex[0];
	uint32_t hexDate1 = hex[3];
	uint32_t hexDate2 = hex[2];

	vector<uint32_t> binTime1 = decToBinary(uint32_t(hexTime1), 8);
	vector<uint32_t> binTime2 = decToBinary(uint32_t(hexTime2), 8);
	vector<uint32_t> binTime;
	getBitsBlock(binTime, binTime1, 8);
	getBitsBlock(binTime, binTime2, 8);

	vector<uint32_t> binSec;
	vector<uint32_t> binMin;
	vector<uint32_t> binHour;

	getBitsBlock(binSec, binTime, 5);
	getBitsBlock(binMin, binTime, 5, 10);
	getBitsBlock(binHour, binTime, 0, 4);

	uint32_t sec = binToDec(binSec) * 2;
	uint32_t min = binToDec(binMin);
	uint32_t hour = binToDec(binHour);

	vector<uint32_t> binDate1 = decToBinary(uint32_t(hexDate1), 8);
	vector<uint32_t> binDate2 = decToBinary(uint32_t(hexDate2), 8);
	vector<uint32_t> binDate;
	getBitsBlock(binDate, binDate1, 8);
	getBitsBlock(binDate, binDate2, 8);

	vector<uint32_t> binDay;
	vector<uint32_t> binMon;
	vector<uint32_t> binYear;

	getBitsBlock(binDay, binDate, 5);
	getBitsBlock(binMon, binDate, 7, 10);
	getBitsBlock(binYear, binDate, 0, 6);

	uint32_t date = binToDec(binDay);
	uint32_t mon = binToDec(binMon);
	uint32_t year = binToDec(binYear);

	dateTime result = { sec,min,hour,date,mon,year + 1980 };
	return result;
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

void reverseStr(string& str)
{
	int n = str.length();

	for (int i = 0; i < n / 2; i++)
		swap(str[i], str[n - i - 1]);
}

string decToHexa(uint32_t n)
{
	stringstream ss;
	while (n != 0) {
		uint32_t rem = 0;

		char ch;
		rem = n % 16;
		if (rem < 10) {
			ch = rem + 48;
			ss << ch;
		}
		else {
			ch = rem + 87;
			ss << ch;
		}

		n = n / 16;
	}

	string result = ss.str();
	reverseStr(result);
	return result;
}