/*********************************************
// Created by m1556 on 2024/4/25 21:18.
*********************************************/

#ifndef LVM_INST_MISC_H
#define LVM_INST_MISC_H

#include "../api/lua_vm.h"
#include "vm.h"


void move(Instruction *i, LuaVM *vm);
void jmp(Instruction *i, LuaVM *vm);

#endif //LVM_INST_MISC_H
