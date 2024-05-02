/*********************************************
// Created by m1556 on 2024/4/25 21:17.
*********************************************/

#include "inst_load.h"
#include "../lua_state.h"


void loadNil(Instruction *i, LuaVM *vm) {
    int a, b, c;
    i->ABC(a, b, c);
    a += 1;

    auto vm_ = dynamic_cast<LuaState *>(vm);
    for (int j = a; j <= a + b ; ++i) {
        vm_->copy(-1, j);
    }
    vm_->pop(1);
}

void loadBool(Instruction *i, LuaVM *vm) {
    int a, b, c;
    i->ABC(a, b, c);
    a += 1;

    auto vm_ = dynamic_cast<LuaState *>(vm);
    vm_->pushBoolean(b != 0);
    vm_->replace(a);
    if (c != 0) {
        vm_->AddPC(1);
    }
}

void loadK(Instruction *i, LuaVM *vm) {
    int a, bx;
    i->ABx(a, bx);
    a += 1;
    auto vm_ = dynamic_cast<LuaState *>(vm);
    vm_->GetConst(bx);
    vm_->replace(a);
}

void loadKx(Instruction *i, LuaVM *vm) {
    int a, bx;
    i->ABx(a, bx);
    a += 1;

    int ax;
    Instruction(vm->Fetch()).Ax(ax);
    auto vm_ = dynamic_cast<LuaState *>(vm);
    vm_->GetConst(ax);
    vm_->replace(a);
}