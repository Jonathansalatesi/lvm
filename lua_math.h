/*********************************************
// Created by m1556 on 2024/4/13 19:47.
*********************************************/

#ifndef LVM_LUA_MATH_H
#define LVM_LUA_MATH_H
#include <string>

#include "lua_type.h"


bool convertToBoolean(Lua_type *val);

// a // b -> -\infin
int64_t IFloorDiv(int64_t a, int64_t b);
double FFloorDiv(double a, double b);

// a % b
int64_t IMod(int64_t a, int64_t b);
double FMod(double a, double b);

// <<
int64_t ShiftLeft(int64_t a, int64_t n);
// >>
int64_t ShiftRight(int64_t a, int64_t n);

// float -> integer
int64_t FloatToInteger(double f, bool &flag);
// str -> integer
int64_t ParseInteger(std::string str, bool &flag);
// str -> double
double ParseFloat(std::string str, bool &flag);

// any lua type -> double
double convertToFloat(Lua_type *val, bool &flag);

// any lua type -> int64_t integer
int64_t convertToInteger(Lua_type *val, bool &flag);

int64_t stringToInteger_(std::string str, bool &flag);



#endif //LVM_LUA_MATH_H
