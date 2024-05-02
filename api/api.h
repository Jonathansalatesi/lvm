/*********************************************
// Created by m1556 on 2024/4/11.
*********************************************/

#ifndef LVM_API_H
#define LVM_API_H
#include "../lua_type.h"


enum LUA_TYPE{
    LUA_TNONE = -1,
    LUA_TNIL = 0,
    LUA_TBOOLEAN,
    LUA_TLIGHTUSERDATA,
    LUA_TNUMBER,
    LUA_TSTRING,
    LUA_TTABLE,
    LUA_TFUNCTION,
    LUA_TUSERDATA,
    LUA_TTHREAD
};

enum class ArithOp {
    LUA_OPADD = 0,          // +
    LUA_OPSUB = 1,          // -
    LUA_OPMUL = 2,          // *
    LUA_OPMOD = 3,          // %
    LUA_OPPOW = 4,          // ^
    LUA_OPDIV = 5,          // /
    LUA_OPIDIV = 6,         // //
    LUA_OPBAND = 7,         // &
    LUA_OPBOR = 8,          // |
    LUA_OPBXOR = 9,         // -
    LUA_OPSHL = 10,          // <<
    LUA_OPSHR = 11,          // >>
    LUA_OPUNM = 12,          // - (unary minus)
    LUA_OPBNOT = 13          // -
};

enum class CompareOp {
    LUA_OPEQ,           // ==
    LUA_OPLT,           // <
    LUA_OPLE,           // <=
};

LUA_TYPE typeOf(Lua_type *val);

#endif //LVM_API_H
