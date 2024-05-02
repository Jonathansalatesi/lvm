/*********************************************
// Created by m1556 on 2024/4/11.
*********************************************/

#ifndef LVM_LUA_STACK_H
#define LVM_LUA_STACK_H
#include <vector>

#include "api/api.h"

class LuaStack{
public:
    std::vector<Lua_type *> slots;
    int top{};

public:
    LuaStack();
    explicit LuaStack(int n);
    ~LuaStack();
    int getTop();
    void check(int n);
    void push(Lua_type *val);
    Lua_type *pop();
    int absIndex(int idx);
    bool isValid(int idx);
    Lua_type *get(int idx);
    void set(int idx, Lua_type *val);
    void reverse(int from, int to);
};

LuaStack *newLuaStack(int size);

#endif //LVM_LUA_STACK_H
