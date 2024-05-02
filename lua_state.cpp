/*********************************************
// Created by m1556 on 2024/4/11 20:44.
*********************************************/
#include <iostream>
#include <sstream>

#include "lua_state.h"
#include "lua_math.h"
#include "lua_type.h"
#include "api/api_arith.h"
#include "api/api_compare.h"


IluaState::~IluaState()= default;

LuaState::LuaState() {
    this->stack = new LuaStack(20);
}

LuaState::LuaState(int n, Prototype *proto) {
    this->stack = new LuaStack(n);
    this->proto = proto;
    this->pc = 0;
}

LuaState::~LuaState(){
    delete this->stack;
}

int LuaState::getTop() {
    return this->stack->getTop();
}

int LuaState::absIndex(int idx) {
    return this->stack->absIndex(idx);
}

bool LuaState::checkStack(int n) {
    this->stack->check(n);
    return true;    // never fails
}

void LuaState::pop(int n) {
    for (int i = 0; i < n; ++i) {
        Lua_type * pLuaType = this->stack->pop();
        delete pLuaType;
    }
}

void LuaState::copy(int fromIdx, int toIdx) {
    Lua_type *val = this->stack->get(fromIdx);
    this->stack->set(toIdx, val->copy());
}

void LuaState::pushValue(int idx) {
    Lua_type *lType = this->stack->get(idx);
    this->stack->push(lType->copy());
}

void LuaState::replace(int idx) {
    Lua_type *val = this->stack->pop();
    this->stack->set(idx, val);
}

void LuaState::insert(int idx) {
    this->rotate(idx, 1);
}

void LuaState::remove(int idx) {
    this->rotate(idx, -1);
    this->pop(1);
}

void LuaState::rotate(int idx, int n) {
    int t = this->stack->getTop() - 1;
    int p = this->stack->absIndex(idx) - 1;
    int m;
    if (n >= 0){
        m = t - n;
    } else {
        m = p - n - 1;
    }
    this->stack->reverse(p, m);
    this->stack->reverse(m + 1, t);
    this->stack->reverse(p, t);
}

void LuaState::setTop(int idx) {
    int newTop = this->stack->absIndex(idx);
    if (newTop < 0){
        std::cerr << "stack underflow!" << std::endl;
        exit(1);
    }
    int n = this->stack->getTop() - newTop;
    if (n > 0){
        for (int i = 0; i < n; ++i) {
            Lua_type *pluaType = this->stack->pop();
            delete pluaType;
        }
    } else if (n < 0){
        for (int i = 0; i > n; --i) {
            this->stack->push(new Lua_value<void *>());
        }
    }
}

void LuaState::pushNil() {
    Lua_type *pt = new Lua_value<void *>();
    this->stack->push(pt);
}

void LuaState::pushBoolean(bool b) {
    this->stack->push(new Lua_value((bool) b));
}

void LuaState::pushInteger(int64_t n) {
    this->stack->push(new Lua_value((int64_t) n));
}

void LuaState::pushNumber(double n) {
    this->stack->push(new Lua_value((double) n));
}

void LuaState::pushString(std::string s) {
    this->stack->push(new Lua_value((std::string) s));
}

std::string LuaState::typeName(LUA_TYPE tp) {
    switch (tp) {
        case LUA_TNONE:{
            return "no value";
        }
        case LUA_TNIL:{
            return "nil";
        }
        case LUA_TBOOLEAN:{
            return "boolean";
        }
        case LUA_TNUMBER:{
            return "number";
        }
        case LUA_TSTRING:{
            return "string";
        }
        case LUA_TTABLE:{
            return "table";
        }
        case LUA_TFUNCTION:{
            return "function";
        }
        case LUA_TTHREAD:{
            return "thread";
        }
        default:{
            return "userdata";
        }
    }
}

Lua_type::Kind LuaState::typeKind(int idx) {
    if (this->stack->isValid(idx)){
        Lua_type *val = this->stack->get(idx);
        return val->get_type();
    }
    return Lua_type::Kind::Unknown;
}

LUA_TYPE LuaState::type(int idx) {
    if (this->stack->isValid(idx)){
        Lua_type *val = this->stack->get(idx);
        return typeOf(val);
    }
    return LUA_TNONE;
}

bool LuaState::isNone(int idx) {
    return this->type(idx) == LUA_TNONE;
}

bool LuaState::isNil(int idx) {
    return this->type(idx) == LUA_TNIL;
}

bool LuaState::isNoneOrNil(int idx) {
    return this->type(idx) <= LUA_TNIL;
}

bool LuaState::isBoolean(int idx) {
    return this->type(idx) == LUA_TBOOLEAN;
}

bool LuaState::isString(int idx) {
    LUA_TYPE t = this->type(idx);
    return t == LUA_TSTRING || t == LUA_TNUMBER;
}

bool LuaState::isNumber(int idx) {
    bool is0X = false;
    this->toNumberX(idx, is0X);
    return is0X;
}

bool LuaState::isInteger(int idx) {
    // need other function to convert the string
    Lua_type *val = this->stack->get(idx);
    return val->get_type() == Lua_type::Kind::Integer;
}

bool LuaState::toBoolean(int idx) {
    Lua_type *pType = this->stack->get(idx);
    return convertToBoolean(pType);
}

double LuaState::toNumber(int idx) {
    bool is0x;
    double n = this->toNumberX(idx, is0x);
    return n;
}

double LuaState::toNumberX(int idx, bool &is0x) {
    Lua_type *pType = this->stack->get(idx);
    return convertToFloat(pType, is0x);
//    switch (pType->get_type()) {
//        case TAG_NUMBER:{
//            is0x = true;
//            return dynamic_cast<Lua_number *>(pType)->get_val();
//        }
//        case TAG_INTEGER:{
//            is0x = true;
//            return dynamic_cast<Lua_integer *>(pType)->get_val();
//        }
//        default:{
//            is0x = false;
//            return 0;
//        }
//    }
}


int64_t LuaState::toInteger(int idx) {
    bool is0x = false;
    return this->toIntegerX(idx, is0x);
}

int64_t LuaState::toIntegerX(int idx, bool &is0x) {
    Lua_type *pType = this->stack->get(idx);
    return convertToInteger(pType, is0x);
//    if (pType->get_type() == Lua_type::Kind::Integer){
//        is0x = true;
//        return pType->as<int64_t>()->get_value();
//    } else {
//        is0x = false;
//        return 0;
//    }
}

std::string LuaState::toStringX(int idx, bool &is0x) {
    Lua_type *pType = this->stack->get(idx);
    switch (pType->get_type()) {
        case Lua_type::Kind::String:{
            is0x = true;
            return pType->as<std::string>()->get_value();
        }
        case Lua_type::Kind::Integer:{
            int64_t val = pType->as<int64_t>()->get_value();
            is0x = true;
            std::ostringstream oss;
            oss << val;
            return oss.str();
        }
        case Lua_type::Kind::Number:{
            double val = pType->as<double>()->get_value();
            is0x = true;
            std::ostringstream oss;
            oss << val;
            return oss.str();
        }
        default:{
            is0x = false;
            return "";
        }
    }
}

std::string LuaState::toString(int idx) {
    bool is0x;
    return this->toStringX(idx, is0x);
}

void LuaState::Arith(ArithOp op) {
    Lua_type *a, *b;
    b = this->stack->pop();
    if (op != ArithOp::LUA_OPUNM && op != ArithOp::LUA_OPBNOT) {
        a = this->stack->pop();
    } else {
        a = b;
    }

    Operator anOperator = operators[(int) op];
    Lua_type *result = arith_(a, b, anOperator);
    if (a == b) {
        delete a;
    } else {
        delete a;
        delete b;
    }

    if (result != nullptr) {
        this->stack->push(result);
    } else {
        std::cerr << "arithmetic error!" << std::endl;
        exit(1);
    }
}

bool LuaState::Compare(int idx1, int idx2, CompareOp op) {
    Lua_type *a = this->stack->get(idx1);
    Lua_type *b = this->stack->get(idx2);
    switch (op) {
        case CompareOp::LUA_OPEQ: {
            return _eq(a, b);
        }
        case CompareOp::LUA_OPLT: {
            return _lt(a, b);
        }
        case CompareOp::LUA_OPLE: {
            return _le(a, b);
        }
        default: {
            std::cerr << "invalid compare op!" << std::endl;
            exit(1);
        }
    }
    return false;
}

void LuaState::Len(int idx) {
    Lua_type *val = this->stack->get(idx);
    if (val->get_type() == Lua_type::Kind::String) {
        // only consider the length of string.
        this->stack->push(new Lua_value((int64_t)val->as<std::string>()->get_value().size()));
    } else {
        std::cerr << "length error: the object to get length isn't string." << std::endl;
        exit(1);
    }
}

void LuaState::Concat(int n) {
    if (n == 0) {
        this->stack->push(new Lua_value<std::string>(""));
    } else if (n >= 2) {
        for (int i = 1; i < n; ++i) {
            if (this->isString(-1) && this->isString(-2)) {
                auto s2 = this->toString(-1);
                auto s1 = this->toString(-2);
                delete this->stack->pop();
                delete this->stack->pop();
                this->stack->push(new Lua_value<std::string>(s1 + s2));
                continue;
            }
            std::cerr << "Concatenation error: not string object." << std::endl;
            exit(1);
        }
    }
}

int LuaState::PC() {
    return this->pc;
}

void LuaState::AddPC(int n) {
    this->pc += n;
}

uint32_t LuaState::Fetch() {
    uint32_t i = this->proto->Code[this->pc];
    this->pc ++;
    return i;
}

void LuaState::GetConst(int idx) {
    auto c = this->proto->Constants[idx];
    this->stack->push(c->copy());
}

void LuaState::GetRK(int rk) {
    if (rk > 0xff) {    // constant
        this->GetConst(rk & 0xff);
    } else {            // register
        this->pushValue(rk + 1);
    }
}