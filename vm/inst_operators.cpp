/*********************************************
// Created by m1556 on 2024/4/25 21:17.
*********************************************/

#include "inst_operators.h"
#include "../lua_state.h"


void binaryArith_(Instruction *i, LuaVM *vm, ArithOp op) {
    int a, b, c;
    i->ABC(a, b, c);
    a += 1;

    vm->GetRK(b);
    vm->GetRK(c);

    auto vm_ = dynamic_cast<LuaState *>(vm);
    vm_->Arith(op);
    vm_->replace(a);
}

void unaryArith_(Instruction *i, LuaVM *vm, ArithOp op) {
    int a, b, c;
    i->ABC(a, b, c);
    a += 1;
    b += 1;

    auto vm_ = dynamic_cast<LuaState *>(vm);
    vm_->pushValue(b);
    vm_->Arith(op);
    vm_->replace(a);
}

void add(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPADD);
}

void sub(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPSUB);
}

void mul(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPMUL);
}

void mod(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPMOD);
}

void pow(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPPOW);
}

void div(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPDIV);
}

void idiv(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPIDIV);
}

void band(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPBAND);
}

void bor(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPBOR);
}

void bxor(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPBXOR);
}

void shl(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPSHL);
}

void shr(Instruction *i, LuaVM *vm) {
    binaryArith_(i, vm, ArithOp::LUA_OPSHR);
}

void unm(Instruction *i, LuaVM *vm) {
    unaryArith_(i, vm, ArithOp::LUA_OPUNM);
}

void bnot(Instruction *i, LuaVM *vm) {
    unaryArith_(i, vm, ArithOp::LUA_OPBNOT);
}

void len_(Instruction *i, LuaVM *vm) {
    int a, b, c;
    i->ABC(a, b, c);
    a += 1;
    b += 1;
    auto vm_ = dynamic_cast<LuaState *>(vm);
    vm_->Len(b);
    vm_->replace(a);
}

void concat(Instruction *i, LuaVM *vm) {
    int a, b, c;
    i->ABC(a, b, c);
    a += 1;
    b += 1;
    c += 1;
    int n = c - b + 1;
    auto vm_ = dynamic_cast<LuaState *>(vm);
    vm_->checkStack(n);
    for (int j = b; j <= c; ++j) {
        vm_->pushValue(j);
    }
    vm_->Concat(n);
    vm_->replace(a);
}

void compare_(Instruction *i, LuaVM *vm, CompareOp op) {
    int a, b, c;
    i->ABC(a, b, c);
    auto vm_ = dynamic_cast<LuaState *>(vm);
    vm_->GetRK(b);
    vm_->GetRK(c);
    if (vm_->Compare(-2, -1, op) != (a != 0)) {
        vm_->AddPC(1);
    }
    vm_->pop(2);
}

void eq(Instruction *i, LuaVM *vm) {
    compare_(i, vm, CompareOp::LUA_OPEQ);
}

void lt(Instruction *i, LuaVM *vm) {
    compare_(i, vm, CompareOp::LUA_OPLT);
}

void le(Instruction *i, LuaVM *vm) {
    compare_(i, vm, CompareOp::LUA_OPLE);
}

void not_(Instruction *i, LuaVM *vm) {
    int a, b, c;
    i->ABC(a, b, c);
    a += 1;
    b += 1;
    auto vm_ = dynamic_cast<LuaState *>(vm);
    vm_->pushBoolean(!vm_->toBoolean(b));
    vm_->replace(a);
}

void testSet(Instruction *i, LuaVM *vm) {
    int a, b, c;
    i->ABC(a, b, c);
    a += 1;
    b += 1;
    auto vm_= dynamic_cast<LuaState *>(vm);
    if (vm_->toBoolean(b) == (c != 0)) {
        vm_->copy(b, a);
    } else {
        vm_->AddPC(1);
    }
}

void test(Instruction *i, LuaVM *vm) {
    int a, b, c;
    i->ABC(a, b, c);
    a += 1;
    auto vm_ = dynamic_cast<LuaState *>(vm);
    if (vm_->toBoolean(a) != (c != 0)) {
        vm_->AddPC(1);
    }
}

void forPrep(Instruction *i, LuaVM *vm) {
    int a, sBx;
    i->AsBx(a, sBx);
    a += 1;

    auto vm_ = dynamic_cast<LuaState *>(vm);
    // R(A) -= R(A + 2)
    vm_->pushValue(a);
    vm_->pushValue(a + 2);
    vm_->Arith(ArithOp::LUA_OPSUB);
    vm_->replace(a);
    // pc += sBx
    vm_->AddPC(sBx);
}

void forLoop(Instruction *i, LuaVM *vm) {
    int a, sBx;
    i->AsBx(a, sBx);
    a += 1;

    auto vm_ = dynamic_cast<LuaState *>(vm);
    // R(A) += R(A + 2)
    vm_->pushValue(a + 2);
    vm_->pushValue(a);
    vm_->Arith(ArithOp::LUA_OPADD);
    vm_->replace(a);

    // R(A) <?= R(A + 1)
    auto isPositiveStep = vm_->toNumber(a + 2) >= 0;
    if (isPositiveStep && vm_->Compare(a, a + 1, CompareOp::LUA_OPLE) ||
            !isPositiveStep && vm_->Compare(a + 1, a, CompareOp::LUA_OPLE)) {
        vm_->AddPC(sBx);                    // pc += sBx
        vm_->copy(a, a + 3);    // R(A + 3) = R(A)
    }
}
