/*********************************************
// Created by m1556 on 2024/4/11.
*********************************************/
#include <iostream>
#include "lua_stack.h"


LuaStack *newLuaStack(int size){
    return new LuaStack(size);
}

LuaStack::LuaStack() = default;

LuaStack::LuaStack(int n) {
    this->slots.reserve(n);
    this->top = 0;
    for (int i = 0; i < n; ++i) {
        this->slots.push_back(nullptr);
    }
}

LuaStack::~LuaStack() {
    for (auto & slot : this->slots) {
            delete slot;
    }
}

int LuaStack::getTop() {
    return this->top;
}

void LuaStack::check(int n) {
    int free = this->slots.size() - this->top;
    if (n > free){
        this->slots.resize(this->slots.size() + n - free, new Lua_value<void *>());
    }
}

void LuaStack::push(Lua_type *val) {
    if (this->top == this->slots.size()){
        std::cerr << "Stack overflow!" << std::endl;
        exit(1);
    }
    this->slots[this->top] = val;
    this->top ++;
}

Lua_type *LuaStack::pop() {
    if (this->top < 1){
        std::cerr << "stack underflow!" << std::endl;
        exit(1);
    }
    this->top --;
    Lua_type *val = this->slots[this->top];
    this->slots[this->top] = nullptr;
    return val;
}

int LuaStack::absIndex(int idx) {
    if (idx >= 0){
        return idx;
    }
    return idx + this->top + 1;
}

bool LuaStack::isValid(int idx) {
    int absIdx = this->absIndex(idx);
    return (absIdx > 0) && (absIdx <= this->top);
}

Lua_type *LuaStack::get(int idx) {
    int absIdx = this->absIndex(idx);
    if (absIdx > 0 && absIdx <= this->top){
        return this->slots[absIdx - 1];
    }
    return nullptr;
}

void LuaStack::set(int idx, Lua_type *val) {
    int absIdx = this->absIndex(idx);
    if (absIdx > 0 && absIdx <= this->top){
        if (this->slots[absIdx - 1] != nullptr){
            // delete original object
            delete this->slots[absIdx - 1];
        }
        this->slots[absIdx - 1] = val;
        return;
    }
    std::cerr << "invalid index!" << std::endl;
    exit(1);
}

void LuaStack::reverse(int from, int to) {
    Lua_type *pType;
    while (from < to){
        pType = this->slots[to];
        this->slots[to] = this->slots[from];
        this->slots[from] = pType;
        from ++;
        to --;
    }
}
