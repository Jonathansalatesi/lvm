/*********************************************
// Created by m1556 on 2024/4/25 21:17.
*********************************************/

#ifndef LVM_INST_OPERATORS_H
#define LVM_INST_OPERATORS_H

#include "../api/lua_vm.h"
#include "../api/api.h"
#include "vm.h"


void binaryArith_(Instruction *i, LuaVM *vm, ArithOp op);
void unaryArith_(Instruction *i, LuaVM *vm, ArithOp op);

void add(Instruction *i, LuaVM *vm);
void sub(Instruction *i, LuaVM *vm);
void mul(Instruction *i, LuaVM *vm);
void mod(Instruction *i, LuaVM *vm);
void pow(Instruction *i, LuaVM *vm);
void div(Instruction *i, LuaVM *vm);
void idiv(Instruction *i, LuaVM *vm);
void band(Instruction *i, LuaVM *vm);
void bor(Instruction *i, LuaVM *vm);
void bxor(Instruction *i, LuaVM *vm);
void shl(Instruction *i, LuaVM *vm);
void shr(Instruction *i, LuaVM *vm);
void unm(Instruction *i, LuaVM *vm);
void bnot(Instruction *i, LuaVM *vm);

void len_(Instruction *i, LuaVM *vm);

void concat(Instruction *i, LuaVM *vm);

void compare_(Instruction *i, LuaVM *vm, CompareOp op);
void eq(Instruction *i, LuaVM *vm);
void lt(Instruction *i, LuaVM *vm);
void le(Instruction *i, LuaVM *vm);

void not_(Instruction *i, LuaVM *vm);

void testSet(Instruction *i, LuaVM *vm);
void test(Instruction *i, LuaVM *vm);

void forPrep(Instruction *i, LuaVM *vm);
void forLoop(Instruction *i, LuaVM *vm);

#endif //LVM_INST_OPERATORS_H
