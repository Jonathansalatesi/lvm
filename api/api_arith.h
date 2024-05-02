/*********************************************
// Created by m1556 on 2024/4/20 21:59.
*********************************************/

#ifndef LVM_API_ARITH_H
#define LVM_API_ARITH_H

#include <cmath>

#include "../lua_math.h"
#include "api.h"


struct Operator {
    int64_t (*integerFunc)(int64_t, int64_t);
    double (*floatFunc)(double, double);
};

int64_t iadd_(int64_t a, int64_t b);
double fadd_(double a, double b);
int64_t isub_(int64_t a, int64_t b);
double fsub_(double a, double b);
int64_t imul_(int64_t a, int64_t b);
double fmul_(double a, double b);
int64_t imod_(int64_t a, int64_t b);
double fmod_(double a, double b);
double pow_(double a, double b);
double div_(double a, double b);
int64_t iidiv_(int64_t a, int64_t b);
double fidiv_(double a, double b);
int64_t band_(int64_t a, int64_t b);
int64_t bor_(int64_t a, int64_t b);
int64_t bxor_(int64_t a, int64_t b);
int64_t shl_(int64_t a, int64_t b);
int64_t shr_(int64_t a, int64_t b);
int64_t iunm_(int64_t a, int64_t b);
double funm_(double a, double b);
int64_t bnot_(int64_t a, int64_t b);

extern Operator operators[14];

[[nodiscard]]
Lua_type *arith_(Lua_type *a, Lua_type *b, Operator op);

#endif //LVM_API_ARITH_H
