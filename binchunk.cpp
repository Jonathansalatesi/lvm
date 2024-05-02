/*********************************************
// Created by m1556 on 2024/3/27.
*********************************************/
#include <iostream>
#include <utility>

#include "def.h"


Prototype *Undump(std::vector<char> data){
    auto *rder = new reader(data);
    rder->checkHeader();        // check the file header
    rder->readByte();           // jump the number of Upvalue
    return rder->readProto("");
}

Prototype *Undump(uint8_t *data, int len){
    std::vector<uint8_t> rdata;
    rdata.reserve(len);
    for (int i = 0; i < len; ++i) {
        rdata.push_back(data[i]);
    }
    auto *rder = new reader{rdata};
    rder->checkHeader();        // check the file header
    rder->readByte();           // jump the number of Upvalue
    return rder->readProto("");
}

uint8_t reader::readByte() {
    uint8_t ret = this->data[0];
    this->data.erase(this->data.begin());
    return ret;
}

unsigned int reader::readUint32() {
    // little endian read uint32
    Float32 val{};
    unsigned int ret = 0;
    for (int i = 0; i < 4; ++i) {
        val.arr[i] = this->data[i];
    }
    ret = val.uint32;
    for (int j = 3; j >= 0; --j) {
        this->data.erase(this->data.begin());
    }
    return ret;
}

uint64_t reader::readUint64(){
    // little endian read uint64
    Float64 val{};
    uint64_t ret = 0;
    for (int i = 0; i < 8; ++i) {
        val.arr[i] = this->data[i];
    }
    ret = val.uint64;
    for (int j = 7; j >= 0; --j) {
        this->data.erase(this->data.begin());
    }
    return ret;
}

int64_t reader::readLuaInteger() {
    return (long long)this->readUint64();
}

double reader::readLuaNumber() {
    Float64 val{};
    double ret = 0;
    for (int i = 0; i < 8; ++i) {
        val.arr[i] = this->data[i];
    }
    ret = val.float64;
    for (int j = 7; j >= 0; --j) {
        this->data.erase(this->data.begin());
    }
    return ret;
}

std::string reader::readString() {
    uint8_t size = this->readByte();
    if (size == 0){     // void string
        return "";
    }

    uint64_t size_l;
    uint8_t *ret;
    if (size == 0xff){
        size_l = this->readUint64();
        ret = this->readBytes(size_l - 1);
        std::string str((char *)ret);
        return str;
    } else {
        ret = this->readBytes(size - 1);
        std::string str((char *)ret);
        return str;
    }
}

uint8_t *reader::readBytes(uint64_t n) {
    auto *ret = new uint8_t[n + 1];
    ret[n] = 0x00;
    for (int i = 0; i < n; ++i) {
        ret[i] = this->data[i];
    }

    for (int j = 0; j < n; ++j) {
        this->data.erase(this->data.begin());
    }
    return ret;
}

void reader::checkHeader() {
    std::string str = (char *)this->readBytes(4);
    if (str != LUA_SIGNATURE){
        std::cerr << "not a precompiled chunk!" << std::endl;
        exit(1);
    } else if (this->readByte() != LUAC_VERSION){
        std::cerr << "version mismatch!" << std::endl;
        exit(1);
    } else if (this->readByte() != LUAC_FORMAT){
        std::cerr << "format mismatch!" << std::endl;
        exit(1);
    }

    str = (char *)this->readBytes(6);
    if (str != LUAC_DATA){
        std::cerr << "corrupted!" << std::endl;
        exit(1);
    } else if (this->readByte() != CINT_SIZE){
        std::cerr << "int size mismatch!" << std::endl;
        exit(1);
    } else if (this->readByte() != CSZIET_SIZE){
        std::cerr << "size_t size mismatch!" << std::endl;
        exit(1);
    } else if (this->readByte() != INSTRUCTION_SIZE){
        std::cerr << "instruction size mismatch!" << std::endl;
        exit(1);
    } else if (this->readByte() != LUA_INTEGER_SIZE){
        std::cerr << "lua_Integer size mismatch!" << std::endl;
        exit(1);
    } else if (this->readByte() != LUA_NUMBER_SIZE){
        std::cerr << "lua_Number size mismatch!" << std::endl;
        exit(1);
    } else if (this->readLuaInteger() != LUAC_INT){
        std::cerr << "endianness mismatch!" << std::endl;
        exit(1);
    } else if (this->readLuaNumber() != LUAC_NUM){
        std::cerr << "float format mismatch!" << std::endl;
        exit(1);
    }
}

Prototype *reader::readProto(std::string parentSource) {
    std::string source = this->readString();
    if (source.empty()){
        source = std::move(parentSource);
    }

    auto *ret = new Prototype;
    ret->Source = source;
    ret->LineDefined = this->readUint32();
    ret->LastLineDefined = this->readUint32();
    ret->NumParams = this->readByte();
    ret->IsVararg = this->readByte();
    ret->MaxStackSize = this->readByte();
    ret->Code = this->readCode();
    ret->Constants = this->readConstants();
    ret->Upvalues = this->readUpvalues();
    ret->Protos = this->readProtos(source);
    ret->LineInfo = this->readLineInfo();
    ret->Locvars = this->readLocVars();
    ret->UpvalueNames = this->readUpvalueNames();
    return ret;
}

std::vector<unsigned int> reader::readCode() {
    unsigned int len = this->readUint32();
    std::vector<unsigned int> ret;
    ret.reserve(len);
    for (int i = 0; i < len; ++i) {
        ret.push_back(this->readUint32());
    }
    return ret;
}

Lua_type *reader::readConstant() {
    Lua_type *ret;
    switch (this->readByte()) {     // tag
        case TAG_NIL:
            ret = new Lua_value<void *>();
            return ret;
        case TAG_BOOLEAN:
            ret = new Lua_value(false);
            ret->as<bool>()->set_value(this->readByte() != 0);
            return ret;
        case TAG_INTEGER:
            ret = new Lua_value((int64_t) 0);
            ret->as<int64_t>()->set_value(this->readLuaInteger());
            return ret;
        case TAG_NUMBER:
            ret = new Lua_value((double) 0);
            ret->as<double>()->set_value(this->readLuaNumber());
            return ret;
        case TAG_SHORT_STR:
        case TAG_LONG_STR:
            ret = new Lua_value((std::string)"");
            ret->as<std::string>()->set_value(this->readString());
            return ret;
        default:
            std::cerr << "corrupted!" << std::endl;
            exit(1);
    }
}

std::vector<Lua_type *> reader::readConstants() {
    std::vector<Lua_type *> ret;
    unsigned int len = this->readUint32();
    ret.reserve(len);
    for (int i = 0; i < len; ++i) {
        ret.push_back(this->readConstant());
    }
    return ret;
}

std::vector<Upvalue> reader::readUpvalues() {
    std::vector<Upvalue> ret;
    unsigned int len = this->readUint32();
    ret.reserve(len);
    for (int i = 0; i < len; ++i) {
        Upvalue upvalue{};
        upvalue.Instack = this->readByte();
        upvalue.Idx = this->readByte();
        ret.push_back(upvalue);
    }
    return ret;
}

std::vector<Prototype *> reader::readProtos(std::string parentSource) {
    std::vector<Prototype *> ret;
    unsigned int len = this->readUint32();
    ret.reserve(len);
    for (int i = 0; i < len; ++i) {
        ret.push_back(this->readProto(parentSource));
    }
    return ret;
}

std::vector<unsigned int> reader::readLineInfo() {
    std::vector<unsigned int> ret;
    unsigned int len = this->readUint32();
    ret.reserve(len);
    for (int i = 0; i < len; ++i) {
        ret.push_back(this->readUint32());
    }
    return ret;
}

std::vector<LocVar *> reader::readLocVars() {
    std::vector<LocVar *> ret;
    unsigned int len = this->readUint32();
    ret.reserve(len);
    for (int i = 0; i < len; ++i) {
        auto pLocVar= new LocVar;
        pLocVar->VarName = this->readString();
        pLocVar->StartPC = this->readUint32();
        pLocVar->EndPC = this->readUint32();
        ret.push_back(pLocVar);
    }
    return ret;
}

std::vector<std::string> reader::readUpvalueNames() {
    std::vector<std::string> ret;
    unsigned int len = this->readUint32();
    ret.reserve(len);
    for (int i = 0; i < len; ++i) {
        ret.push_back(this->readString());
    }
    return ret;
}