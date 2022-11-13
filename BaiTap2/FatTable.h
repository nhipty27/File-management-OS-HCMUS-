#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;
class FatTable {
private:
    vector<uint16_t> _status;
    uint32_t _size;
public:
    FatTable() {
        this->_size = 256;
        for (uint32_t i = 0; i < _size; i++)
            this->_status.push_back(0);
    }
    FatTable(uint32_t size)
    {
        this->_size = size;
        for (uint32_t i = 0; i < size; i++)
            this->_status.push_back(0);

    }

    uint32_t size() { return this->_size; }

    void set_value_at_index(uint32_t value, uint32_t index)
    {
        if (index >= 0 && index < this->_size)
            this->_status[index] = value;
    }

    uint32_t get_value_at_index(uint32_t index) { return this->_status[index]; }

};