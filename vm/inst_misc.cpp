/*********************************************
// Created by m1556 on 2024/4/25 21:18.
*********************************************/
#include <iostream>

#include "inst_misc.h"
#include "../lua_state.h"


void move(Instruction *i, LuaVM *vm) {
    int a, b, c;
    i->ABC(a, b, c);
    a += 1;
    b += 1;
    auto state_ = dynamic_cast<LuaState *>(vm);
    state_->copy(b, a);
}

void jmp(Instruction *i, LuaVM *vm) {
    int a, sBx;
    i->AsBx(a, sBx);
    vm->AddPC(sBx);
    if (a != 0) {
        std::cerr << "todo" << std::endl;
        exit(1);
    }
}