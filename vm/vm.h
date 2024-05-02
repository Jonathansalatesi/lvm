/*********************************************
// Created by m1556 on 2024/4/9.
*********************************************/

#ifndef LVM_VM_H
#define LVM_VM_H
#include <string>

#include "../api/lua_vm.h"


// In LUA VM, there are 4 type of Code Mode:
// @ iABC: A, B, C are 3 arguments, A[8], B[9], C[9];
// @ iABx: Ax, Bx are 2 arguments, Ax[8], Bx[18];
// @ iAsBx: A, sBx are 2 arguments, A[8], sBx[18]; here, sBx means signed number;
// @ iAx: A is the only 1 argument, A[26]
enum INSR_MODE{
    IABC = 0,
    IABx,
    IAsBx,
    IAx
};

// In 47 instructions of LUA VM, 39 instructions are iABC mode,
// 3 instructions are iABx, 4 instructions are iAsBx,
// 1 instruction is iAx.
// Opcode [6]
enum OPCODE{
     OP_MOVE = 0,
     OP_LOADK,
     OP_LOADKX,
     OP_LOADBOOL,
     OP_LOADNIL,
     OP_GETUPVAL,
     OP_GETTABUP,
     OP_GETTABLE,
     OP_SETTABUP,
     OP_SETUPVAL,
     OP_SETTABLE,
     OP_NEWTABLE,
     OP_SELF,
     OP_ADD,
     OP_SUB,
     OP_MUL,
     OP_MOD,
     OP_POW,
     OP_DIV,
     OP_IDIV,
     OP_BAND,
     OP_BOR,
     OP_BXOR,
     OP_SHL,
     OP_SHR,
     OP_UNM,
     OP_BNOT,
     OP_NOT,
     OP_LEN,
     OP_CONCAT,
     OP_JMP,
     OP_EQ,
     OP_LT,
     OP_LE,
     OP_TEST,
     OP_TESTSET,
     OP_CALL,
     OP_TAILCALL,
     OP_RETURN,
     OP_FORLOOP,
     OP_FORPREP,
     OP_TFORCALL,
     OP_TFORLOOP,
     OP_SETLIST,
     OP_CLOSURE,
     OP_VARARG,
     OP_EXTRARG
};

// OpArgN, OpArgU, OpArgR, OpArgK are 4 type of op args.
// OpArgN is useless, for example, move(iABC), C is useless.
// OpArgR is the index of reg in iABC, for example, move(iABC): R(A) := R(B)
// OpArgK is index of const table or index of reg. 2 conditions: 1. LOADK(iABx): R(A) := Kst(Bx); 2. ADD(iABC): R(A) := RK(B) + RK(C)
enum OPARG{
    OpArgN = 0,     // argument is not used
    OpArgU,         // argument is used
    OpArgR,         // argument is a register or a jump offset
    OpArgK          // argument is a constant or register / constant
};

class Instruction;

struct Opcode{
    unsigned char testFlag;     // operator is a test (next instruction must be a jump)
    unsigned char setAFlag;     // instruction set register A
    OPARG argBMode;             // B arg mode
    OPARG argCMode;             // C arg mode
    INSR_MODE opMode;           // op mode
    std::string name;
    void (*action)(Instruction *i, LuaVM *vm);
};

class Instruction{
protected:
    unsigned int instruction;

public:
    explicit Instruction(unsigned int a);
    unsigned int get_inst();
    void set_inst(unsigned int a);
    unsigned int Opcode();
    void ABC(int &a, int &b, int &c);
    void ABx(int &a, int &bx);
    void AsBx(int &a, int &sbx);
    void Ax(int &a);

    std::string OpName();
    INSR_MODE OpMode();
    OPARG BMode();
    OPARG CMode();

    void execute(LuaVM *vm);
};

#endif //LVM_VM_H
