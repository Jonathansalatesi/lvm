#include <iostream>
#include <fstream>
#include <cstdio>

#include "def.h"
#include "vm/vm.h"
#include "lua_state.h"
#include "lua_type.h"


void printStack(LuaState *ls){
    int top = ls->getTop();
    Lua_type::Kind tag_tp;
    for (int i = 1; i <= top; ++i) {
        tag_tp = ls->typeKind(i);
        switch (tag_tp) {
            case Lua_type::Kind::Nil:{
                printf("[nil]");
                break;
            }
            case Lua_type::Kind::Boolean:{
                bool bt = ls->toBoolean(i);
                if (bt){
                    printf("[true]");
                } else {
                    printf("[false]");
                }
                break;
            }
            case Lua_type::Kind::Integer:{
                printf("[%lld]", ls->toInteger(i));
                break;
            }
            case Lua_type::Kind::Number:{
                printf("[%lf]", ls->toNumber(i));
                break;
            }
            case Lua_type::Kind::String:{
                printf("[\"%s\"]", ls->toString(i).c_str());
                break;
            }
            default:{
                printf("[%s]", "None");
                break;
            }
        }
    }
    printf("\n");
}


void printHeader(Prototype *f){
    std::string funcType = "main";
    if (f->LineDefined > 0){
        funcType = "function";
    }

    std::string varargFlag = "";
    if (f->IsVararg > 0){
        varargFlag = "+";
    }

    printf("\n%s <%s:%d, %d> (%zu instruction)\n", funcType.c_str(), f->Source.c_str(), f->LineDefined, f->LastLineDefined, f->Code.size());
    printf("%d%s params, %d slots, %zu upvalues, ", f->NumParams, varargFlag.c_str(), f->MaxStackSize, f->Code.size());
    printf("%zu locals, %zu constants, %zu functions\n", f->Locvars.size(), f->Constants.size(), f->Protos.size());
}

void printOperands(Instruction *i){
    switch (i->OpMode()) {
        case IABC:{
            int a, b, c;
            i->ABC(a, b, c);
            printf("%d", a);
            if (i->BMode() != OpArgN){
                if (b > 0xff){
                    printf(" %d", -1 - b & 0xff);
                } else {
                    printf(" %d", b);
                }
            }
            if (i->CMode() != OpArgN){
                if (c > 0xff){
                    printf(" %d", -1 - c & 0xff);
                } else {
                    printf(" %d", c);
                }
            }
            break;
        }
        case IABx:{
            int a, bx;
            i->ABx(a, bx);
            printf("%d", a);
            if (i->BMode() == OpArgK){
                printf(" %d", -1 - bx);
            } else if(i->BMode() == OpArgU){
                printf(" %d", bx);
            }
            break;
        }
        case IAsBx:{
            int a, sbx;
            i->AsBx(a, sbx);
            printf("%d %d", a, sbx);
            break;
        }
        case IAx:{
            int ax;
            i->Ax(ax);
            printf("%d", -1 - ax);
            break;
        }
    }
}

void printCode(Prototype *f){
    std::string line = "";
    for (int i = 0; i < f->Code.size(); ++i) {
        line = "-";
        if (!f->LineInfo.empty()){
            line = std::to_string(f->LineInfo[i]);
        }
        auto *pInstruction = new Instruction(f->Code[i]);
        printf("\t%d\t[%s]\t%s \t", i + 1, line.c_str(), pInstruction->OpName().c_str());
        printOperands(pInstruction);
        delete pInstruction;
        printf("\n");
    }
}

std::string constantToString(Lua_type *k){
    std::string str;
    switch (k->get_type()) {
        case Lua_type::Kind::Nil:{
            str = "nil";
            break;
        }
        case Lua_type::Kind::Boolean:{
            auto kb = k->as<bool>();
            str = std::to_string(kb->get_value());
            break;
        }
        case Lua_type::Kind::Number:{
            auto kn = k->as<double>();
            str = std::to_string(kn->get_value());
            break;
        }
        case Lua_type::Kind::Integer:{
            auto ki = k->as<int64_t>();
            str = std::to_string(ki->get_value());
            break;
        }
        case Lua_type::Kind::String:{
            auto kls = k->as<std::string>();
            str = kls->get_value();
            break;
        }
        default:{
            str = "?";
            break;
        }
    }
    return str;
}

bool isExistUpVal(Prototype *f){
    return !f->UpvalueNames.empty();
}

void printDetail(Prototype *f){
    printf("constants (%zu):\n", f->Constants.size());
    char *content = nullptr;
    for (int i = 0; i < f->Constants.size(); ++i) {
        printf("\t%d\t%s\n", i + 1, constantToString(f->Constants[i]).c_str());
    }
    printf("locals (%zu): \n", f->Locvars.size());
    for (int i = 0; i < f->Locvars.size(); ++i) {
        printf("\t%d\t%s\t%d\t%d\n", i, f->Locvars[i]->VarName.c_str(), f->Locvars[i]->StartPC + 1, f->Locvars[i]->EndPC + 1);
    }

    printf("upvalues (%zu): \n", f->Upvalues.size());
    for (int i = 0; i < f->Upvalues.size(); ++i) {
        if (isExistUpVal(f)){
            printf("\t%d\t%s\t%d\t%d\n", i, f->UpvalueNames[i].c_str(), f->Upvalues[i].Instack, f->Upvalues[i].Idx);
        } else {
            printf("\t%d\t%s\t%d\t%d\n", i, "-", f->Upvalues[i].Instack, f->Upvalues[i].Idx);
        }
    }
}

void list(Prototype *f){
    printHeader(f);
    printCode(f);
    printDetail(f);
    for (int i = 0; i < f->Protos.size(); ++i) {
        list(f->Protos[i]);
    }
}

void luaMain(Prototype *proto) {
    auto nRegs = (int) proto->MaxStackSize;
    LuaState ls(nRegs + 8, proto);
    ls.setTop(nRegs);
    while (true) {
        auto pc = ls.PC();
        Instruction inst(ls.Fetch());
        if (inst.Opcode() != OP_RETURN) {
            inst.execute(&ls);
            printf("[%02d] %s", pc + 1, inst.OpName().c_str());
            printStack(&ls);
        } else {
            break;
        }
    }
}

int main() noexcept {
    std::ifstream file(R"(D:\usr\lua-5.3.4_Win64_bin\sample\luac.out)", std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Fail to open file." << std::endl;
        return 1;
    }

    // get the size of file
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // prepare to read
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        // success to read, and convert it as binary data
        auto *proto = Undump(buffer);
        luaMain(proto);
        delete proto;
    } else {
        std::cerr << "Fail to read file." << std::endl;
    }

    file.close();

//    auto *ls = new LuaState();
//    ls->pushInteger(1);
//    ls->pushString("2.0");
//    ls->pushString("3.0");
//    ls->pushNumber(4.0);
//    printStack(ls);
//
//    ls->Arith(ArithOp::LUA_OPADD);
//    printStack(ls);
//    ls->Arith(ArithOp::LUA_OPBNOT);
//    printStack(ls);
//    ls->Len(2);
//    printStack(ls);
//    ls->Concat(3);
//    printStack(ls);
//    ls->pushBoolean(ls->Compare(1, 2, CompareOp::LUA_OPEQ));
//    printStack(ls);
//
//    delete ls;

//    std::ifstream file(R"(D:\usr\lua-5.3.2_Win64_bin\sample\luac.out)", std::ios::binary | std::ios::ate);
//    if (!file.is_open()) {
//        std::cerr << "Fail to open file." << std::endl;
//        return 1;
//    }
//
//    // get the size of file
//    std::streamsize size = file.tellg();
//    file.seekg(0, std::ios::beg);
//
//    // prepare to read
//    std::vector<char> buffer(size);
//    if (file.read(buffer.data(), size)) {
//        // success to read, and convert it as binary data
//        auto *p = Undump(buffer);
//        list(p);
//        delete p;
//    } else {
//        std::cerr << "Fail to read file." << std::endl;
//    }
//
//    file.close();
    return 0;
}