/*********************************************
// Created by m1556 on 2024/3/27.
*********************************************/

#ifndef LVM_DEF_H
#define LVM_DEF_H

#include <vector>
#include <string>

#include "lua_type.h"


#define LUA_SIGNATURE "\x1bLua"
#define LUAC_VERSION 0x53
#define LUAC_FORMAT 0
#define LUAC_DATA "\x19\x93\r\n\x1a\n"
#define CINT_SIZE 4
#define CSZIET_SIZE 8
#define INSTRUCTION_SIZE 4
#define LUA_INTEGER_SIZE 8
#define LUA_NUMBER_SIZE 8
#define LUAC_INT 0x5678
#define LUAC_NUM 370.5

class Header{
public:
    char signature[4];
    char version;
    char format;
    char luacData[6];
    char cintSize;
    char sizetSize;
    char instructionSize;
    char luaIntegerSize;
    char luaNumberSize;
    int64_t luacInt;
    double luacNum;
};

struct Upvalue{
public:
    uint8_t Instack;
    uint8_t Idx;
};

class LocVar{
public:
    std::string VarName;
    uint32_t StartPC;
    uint32_t EndPC;
};

class Prototype{
public:
    std::string Source;
    uint32_t LineDefined;
    uint32_t LastLineDefined;
    uint8_t NumParams;
    uint8_t IsVararg;
    uint8_t MaxStackSize;
    int num_of_code;
    std::vector<uint32_t> Code;
    std::vector<Lua_type *> Constants;
    std::vector<Upvalue> Upvalues;
    std::vector<Prototype *> Protos;
    std::vector<uint32_t> LineInfo;
    std::vector<LocVar *> Locvars;
    std::vector<std::string> UpvalueNames;

public:
    ~Prototype();
};

class binaryChunk{
public:
   Header *header;
   char sizeUpvalues;
   Prototype *mainFunc;
};

union Float32{
    float float32;
    uint32_t uint32;
    uint8_t arr[4];
};

union Float64{
    double float64;
    uint64_t uint64;
    uint8_t arr[8];
};

class reader{
public:
    std::vector<uint8_t> data;

public:
    reader();
    explicit reader(std::vector<char> rdata);
    explicit reader(std::vector<uint8_t > rdata);
    uint8_t readByte();
    uint32_t readUint32();
    uint64_t readUint64();
    int64_t readLuaInteger();
    double readLuaNumber();
    std::string readString();
    uint8_t *readBytes(uint64_t n);

    std::vector<uint32_t> readCode();
    Lua_type *readConstant();
    std::vector<Lua_type *> readConstants();
    std::vector<Upvalue> readUpvalues();
    std::vector<Prototype *> readProtos(std::string parentSource);
    std::vector<uint32_t> readLineInfo();
    std::vector<LocVar *> readLocVars();
    std::vector<std::string> readUpvalueNames();

    void checkHeader();

    Prototype *readProto(std::string parentSource);
};

Prototype *Undump(uint8_t *data, int len);
Prototype *Undump(std::vector<char> data);

#endif //LVM_DEF_H
