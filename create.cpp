/*********************************************
// Created by m1556 on 2024/3/28.
*********************************************/
#include <utility>

#include "def.h"


reader::reader() = default;

reader::reader(std::vector<char> rdata) {
    this->data.reserve(rdata.size());
    for (int i = 0; i < rdata.size(); ++i) {
        this->data.push_back((uint8_t) rdata[i]);
    }
}

reader::reader(std::vector<uint8_t> rdata) {
    this->data.reserve(rdata.size());
    for (uint8_t i : rdata) {
        this->data.push_back(i);
    }
}


