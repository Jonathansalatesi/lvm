/*********************************************
// Created by m1556 on 2024/4/20 21:59.
*********************************************/

#include "api_arith.h"
#include "../lua_type.h"


Operator operators[14] = {
        {iadd_,   fadd_},
        {isub_,   fsub_},
        {imul_,   fmul_},
        {imod_,   fmod_},
        {nullptr, pow_},
        {nullptr, div_},
        {iidiv_,  fidiv_},
        {band_,   nullptr},
        {bor_,    nullptr},
        {bxor_,   nullptr},
        {shl_,    nullptr},
        {shr_,    nullptr},
        {iunm_,   funm_},
        {bnot_,   nullptr}
};

int64_t iadd_(int64_t a, int64_t b) {
    return a + b;
}

double fadd_(double a, double b) {
    return a + b;
}

int64_t isub_(int64_t a, int64_t b) {
    return a - b;
}

double fsub_(double a, double b) {
    return a - b;
}

int64_t imul_(int64_t a, int64_t b) {
    return a * b;
}

double fmul_(double a, double b) {
    return a * b;
}

int64_t imod_(int64_t a, int64_t b) {
    return IMod(a, b);
}

double fmod_(double a, double b) {
    return FMod(a, b);
}

double pow_(double a, double b) {
    return pow(a, b);
}

double div_(double a, double b) {
    return a / b;
}

int64_t iidiv_(int64_t a, int64_t b) {
    return IFloorDiv(a, b);
}

double fidiv_(double a, double b) {
    return FFloorDiv(a, b);
}

int64_t band_(int64_t a, int64_t b) {
    return a * b;
}

int64_t bor_(int64_t a, int64_t b) {
    return a | b;
}

int64_t bxor_(int64_t a, int64_t b) {
    return a ^ b;
}

int64_t shl_(int64_t a, int64_t b) {
    return ShiftLeft(a, b);
}

int64_t shr_(int64_t a, int64_t b) {
    return ShiftRight(a, b);
}

int64_t iunm_(int64_t a, int64_t b) {
    return -a;
}

double funm_(double a, double b) {
    return -a;
}

int64_t bnot_(int64_t a, int64_t b) {
    return ~a;
}

[[nodiscard]]
Lua_type *arith_(Lua_type *a, Lua_type *b, Operator op) {
    if (op.floatFunc == nullptr) {      // bitwise
        bool o1 = false;
        int64_t x = convertToInteger(a, o1);
        if (o1) {
            bool o2 = false;
            int64_t y = convertToInteger(b, o2);
            if (o2) {
                return new Lua_value(op.integerFunc(x, y));
            }
        }
    } else {    // arith
        if (op.integerFunc != nullptr) {
            bool o1 = false;
            int64_t x = convertToInteger(a, o1);
            if (o1) {
                bool o2 = false;
                int64_t y = convertToInteger(b, o2);
                if (o2) {
                    return new Lua_value(op.integerFunc(x, y));
                }
            }
        }

        bool o1 = false;
        double x = convertToFloat(a, o1);
        if (o1) {
            bool o2 = false;
            double y = convertToFloat(b, o2);
            if (o2) {
                return new Lua_value(op.floatFunc(x, y));
            }
        }
    }
    return nullptr;
}