#include"Header.h"

vector<uint32_t> decToBinary(uint32_t n, uint32_t numBit);

void getBitsBlock(vector<uint32_t>& result, vector<uint32_t> bin, uint32_t from, uint32_t to);

uint32_t binToDec(vector<uint32_t> bin);

BYTE* create_offsets_zero(uint32_t n);

BYTE* convert_dateTime_to_hex_with_little_endian(dateTime dt);

dateTime convert_hex_to_dateTime(BYTE* hex);

void reverseStr(string& str);

string decToHexa(uint32_t n);

BYTE* string_to_hex(string str);