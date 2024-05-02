/*********************************************
// Created by m1556 on 2024/4/25 21:17.
*********************************************/

#ifndef LVM_INST_LOAD_H
#define LVM_INST_LOAD_H

#include "vm.h"
#include "../api/lua_vm.h"

void loadNil(Instruction *i, LuaVM *vm);
void loadBool(Instruction *i, LuaVM *vm);
void loadK(Instruction *i, LuaVM *vm);
void loadKx(Instruction *i, LuaVM *vm);



#endif //LVM_INST_LOAD_H
