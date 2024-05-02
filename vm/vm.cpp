/*********************************************
// Created by m1556 on 2024/4/9.
*********************************************/
#include <iostream>

#include "vm.h"
#include "inst_load.h"
#include "inst_misc.h"
#include "inst_operators.h"

#define MAXARG_Bx ((1 << 18) - 1)             // 2 ^ 18 - 1 = 262143
#define MAXARG_sBx (MAXARG_Bx >> 1)           // 262143 / 2 = 131071

Opcode opcodes[] = {
        //             T            A               B                       C           mode             name             action
        Opcode{0, 1, OpArgR, OpArgN, IABC, "MOVE    ", move},
        Opcode{0, 1, OpArgK, OpArgN, IABx, "LOADK   ", loadK},
        Opcode{0, 1, OpArgN, OpArgN, IABx, "LOADKX  ", loadKx},
        Opcode{0, 1, OpArgU, OpArgU, IABC, "LOADBOOL", loadBool},
        Opcode{0, 1, OpArgU, OpArgN, IABC, "LOADNIL ", loadNil},
        Opcode{0, 1, OpArgU, OpArgN, IABC, "GETUPVAL", nullptr},
        Opcode{0, 1, OpArgU, OpArgK, IABC, "GETTABUP", nullptr},
        Opcode{0, 1, OpArgR, OpArgK, IABC, "GETTABLE", nullptr},
        Opcode{0, 0, OpArgK, OpArgK, IABC, "SETTABUP", nullptr},
        Opcode{0, 0, OpArgU, OpArgN, IABC, "SETUPVAL", nullptr},
        Opcode{0, 0, OpArgK, OpArgK, IABC, "SETTABLE", nullptr},
        Opcode{0, 1, OpArgU, OpArgU, IABC, "NEWTABLE", nullptr},
        Opcode{0, 1, OpArgR, OpArgK, IABC, "SELF    ", nullptr},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "ADD     ", add},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "SUB     ", sub},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "MUL     ", mul},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "MOD     ", mod},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "POW     ", pow},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "DIV     ", div},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "IDIV    ", idiv},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "BAND    ", band},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "BOR     ", bor},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "BXOR    ", bxor},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "SHL     ", shl},
        Opcode{0, 1, OpArgK, OpArgK, IABC, "SHR     ", shr},
        Opcode{0, 1, OpArgR, OpArgN, IABC, "UNM     ", unm},
        Opcode{0, 1, OpArgR, OpArgN, IABC, "BNOT    ", bnot},
        Opcode{0, 1, OpArgR, OpArgN, IABC, "NOT     ", not_},
        Opcode{0, 1, OpArgR, OpArgN, IABC, "LEN     ", len_},
        Opcode{0, 1, OpArgR, OpArgR, IABC, "CONCAT  ", concat},
        Opcode{0, 0, OpArgR, OpArgN, IAsBx, "JMP     ", jmp},
        Opcode{1, 0, OpArgK, OpArgK, IABC, "EQ      ", eq},
        Opcode{1, 0, OpArgK, OpArgK, IABC, "LT      ", lt},
        Opcode{1, 0, OpArgK, OpArgK, IABC, "LE      ", le},
        Opcode{1, 0, OpArgN, OpArgU, IABC, "TEST    ", test},
        Opcode{1, 1, OpArgR, OpArgU, IABC, "TESTSET ", testSet},
        Opcode{0, 1, OpArgU, OpArgU, IABC, "CALL    ", nullptr},
        Opcode{0, 1, OpArgU, OpArgU, IABC, "TAILCALL", nullptr},
        Opcode{0, 0, OpArgU, OpArgN, IABC, "RETURN  ", nullptr},
        Opcode{0, 1, OpArgR, OpArgN, IAsBx, "FORLOOP ", forLoop},
        Opcode{0, 1, OpArgR, OpArgN, IAsBx, "FORPREP ", forPrep},
        Opcode{0, 0, OpArgN, OpArgU, IABC, "TFORCALL", nullptr},
        Opcode{0, 1, OpArgR, OpArgN, IAsBx, "TFORLOOP", nullptr},
        Opcode{0, 0, OpArgU, OpArgU, IABC, "SETLIST ", nullptr},
        Opcode{0, 1, OpArgU, OpArgN, IABx, "CLOSURE ", nullptr},
        Opcode{0, 1, OpArgU, OpArgN, IABC, "VARARG  ", nullptr},
        Opcode{0, 0, OpArgU, OpArgU, IAx, "EXTRAARG", nullptr}
};

Instruction::Instruction(unsigned int a) {
    this->instruction = a;
}

unsigned int Instruction::get_inst() {
    return this->instruction;
}

void Instruction::set_inst(unsigned int a) {
    this->instruction = a;
}

unsigned int Instruction::Opcode() {
    return (this->instruction & 0x3f);
}

void Instruction::ABC(int &a, int &b, int &c) {
    a = ((this->instruction >> 6) & 0xff);
    c = ((this->instruction >> 14) & 0x1ff);
    b = ((this->instruction >> 23) & 0x1ff);
}

void Instruction::ABx(int &a, int &bx) {
    a = (this->instruction >> 6) & 0xff;
    bx = this->instruction >> 14;
}

void Instruction::AsBx(int &a, int &sbx) {
    int at, bx;
    this->ABx(at, bx);
    a = at;
    sbx = bx - MAXARG_sBx;
}

void Instruction::Ax(int &a) {
    a = this->instruction >> 6;
}

std::string Instruction::OpName() {
    return opcodes[this->Opcode()].name;
}

INSR_MODE Instruction::OpMode() {
    return opcodes[this->Opcode()].opMode;
}

OPARG Instruction::BMode() {
    return opcodes[this->Opcode()].argBMode;
}

OPARG Instruction::CMode() {
    return opcodes[this->Opcode()].argCMode;
}

void Instruction::execute(LuaVM *vm) {
    auto action = opcodes[this->Opcode()].action;
    if (action != nullptr) {
        action(this, vm);
    } else {
        std::cerr << this->OpName() << std::endl;
        exit(1);
    }
}