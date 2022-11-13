#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <vector>
#include <time.h>
using namespace std;

class dateTime {
private:
	uint32_t tm_sec;
	uint32_t tm_min;
	uint32_t tm_hour;
	uint32_t tm_mday;
	uint32_t tm_mon;
	uint32_t tm_year;
public:
	dateTime() {
		this->tm_sec = 0;
		this->tm_min = 0;
		this->tm_hour = 0;
		this->tm_mday = 0;
		this->tm_mon = 0;
		this->tm_year = 0;
	}
	dateTime(uint32_t sec, uint32_t min, uint32_t hour, uint32_t day, uint32_t mon, uint32_t year) {
		this->tm_sec = sec;
		this->tm_min = min;
		this->tm_hour = hour;
		this->tm_mday = day;
		this->tm_mon = mon;
		this->tm_year = year;
	}
	void getCurrentTime() {
		time_t rawtime;
		struct tm* info;
		time(&rawtime);
		info = gmtime(&rawtime);
		this->tm_hour = uint32_t((info->tm_hour + 7) % 24);
		this->tm_min = uint32_t(info->tm_min);
		this->tm_sec = uint32_t(info->tm_sec);
		this->tm_mday = uint32_t(info->tm_mday);
		this->tm_mon = uint32_t(info->tm_mon + 1);
		this->tm_year = uint32_t(info->tm_year + 1900);
	}

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

	BYTE* convert_dateTime_to_hex_with_little_endian() {

		//2 byte dau la thoi gian, 2 byte sau la ngay thang
		BYTE* byte = create_offsets_zero(4);
		uint32_t numBit = 8;
		vector<uint32_t> binSec = decToBinary(this->tm_sec / 2, numBit);
		vector<uint32_t> binMin = decToBinary(this->tm_min, numBit);
		vector<uint32_t> binHour = decToBinary(this->tm_hour, numBit);

		vector<uint32_t> binTime;
		getBitsBlock(binTime, binHour, 5);
		getBitsBlock(binTime, binMin, 6);
		getBitsBlock(binTime, binSec, 5);

		vector<uint32_t> binTime1;
		getBitsBlock(binTime1, binTime, 0, 7);
		vector<uint32_t> binTime2;
		getBitsBlock(binTime2, binTime, 8);

		uint32_t decTime1 = binToDec(binTime1);
		uint32_t decTime2 = binToDec(binTime2);

		vector<uint32_t> binDay = decToBinary(this->tm_mday, numBit);
		vector<uint32_t> binMon = decToBinary(this->tm_mon, numBit);
		vector<uint32_t> binYear = decToBinary(this->tm_year - 1980, numBit);
		vector<uint32_t> binDate;
		getBitsBlock(binDate, binYear, 7);
		getBitsBlock(binDate, binMon, 4);
		getBitsBlock(binDate, binDay, 5);

		vector<uint32_t> binDate1;
		getBitsBlock(binDate1, binDate, 0, 7);
		vector<uint32_t> binDate2;
		getBitsBlock(binDate2, binDate, 8);

		uint32_t decDate1 = binToDec(binDate1);
		uint32_t decDate2 = binToDec(binDate2);

		byte[0] = BYTE(decTime2);
		byte[1] = BYTE(decTime1);
		byte[2] = BYTE(decDate2);
		byte[3] = BYTE(decDate1);
		return byte;
	}

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

};