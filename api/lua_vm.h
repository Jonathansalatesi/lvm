/*********************************************
// Created by m1556 on 2024/4/25 7:29.
*********************************************/

#ifndef LVM_LUA_VM_H
#define LVM_LUA_VM_H

#include "../lua_type.h"


class LuaVM {
public:
    virtual int PC() = 0;       // return temporary PC for testing
    virtual void AddPC(int n) = 0;      // change temporary PC (for achieving JMP order)
    virtual uint32_t Fetch() = 0;       // get temporary order, and move PC to next order
    virtual void GetConst(int idx) = 0; // get constant from const table, push it to the stack top
    virtual void GetRK(int rk) = 0;     // push the determined const and value in stack to the stack top
};


#endif //LVM_LUA_VM_H
