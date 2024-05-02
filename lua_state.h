/*********************************************
// Created by m1556 on 2024/4/11 20:44.
*********************************************/

#ifndef LVM_LUA_STATE_H
#define LVM_LUA_STATE_H

#include <string>

#include "api/api.h"
#include "api/lua_vm.h"
#include "lua_stack.h"
#include "lua_type.h"
#include "def.h"


class IluaState{
public:
    virtual ~IluaState() = 0;
    // basic stack manipulation
    virtual int getTop() = 0;
    virtual int absIndex(int idx) = 0;
    virtual bool checkStack(int n) = 0;
    virtual void pop(int n) = 0;
    virtual void copy(int fromIdx, int toIdx) = 0;
    virtual void pushValue(int idx) = 0;
    virtual void replace(int idx) = 0;
    virtual void insert(int idx) = 0;
    virtual void remove(int idx) = 0;
    virtual void rotate(int n, int idx) = 0;
    virtual void setTop(int idx) = 0;

    // access functions (stack -> c++)
    virtual Lua_type::Kind typeKind(int idx) = 0;
    virtual std::string typeName(LUA_TYPE tp) = 0;
    virtual LUA_TYPE type(int idx) = 0;
    virtual bool isNone(int idx) = 0;
    virtual bool isNil(int idx) = 0;
    virtual bool isNoneOrNil(int idx) = 0;
    virtual bool isBoolean(int idx) = 0;
    virtual bool isInteger(int idx) = 0;
    virtual bool isNumber(int idx) = 0;
    virtual bool isString(int idx) = 0;
    virtual bool toBoolean(int idx) = 0;
    virtual int64_t toInteger(int idx) = 0;
    virtual int64_t toIntegerX(int idx, bool &is0x) = 0;
    virtual double toNumber(int idx) = 0;
    virtual double toNumberX(int idx, bool &is0x) = 0;
    virtual std::string toString(int idx) = 0;
    virtual std::string toStringX(int idx, bool &is0x) = 0;

    // push functions (cpp -> stack)
    virtual void pushNil() = 0;
    virtual void pushBoolean(bool b) = 0;
    virtual void pushInteger(int64_t n) = 0;
    virtual void pushNumber(double n) = 0;
    virtual void pushString(std::string s) = 0;

    virtual void Arith(ArithOp op) = 0;
    virtual bool Compare(int idx1, int idx2, CompareOp op) = 0;
    virtual void Len(int idx) = 0;
    virtual void Concat(int n) = 0;
};

class LuaState: public IluaState, public LuaVM{
protected:
    LuaStack *stack{};
    Prototype *proto;
    int pc;

public:
    LuaState();
    LuaState(int n, Prototype *proto);
    ~LuaState() override;
    int getTop() override;
    int absIndex(int idx) override;
    bool checkStack(int n) override;
    void pop(int n) override;
    void copy(int fromIdx, int toIdx) override;

    void pushValue(int idx) override;
    void replace(int idx) override;
    void insert(int idx) override;
    void remove(int idx) override;
    void rotate(int idx, int n) override;
    void setTop(int idx) override;

    void pushNil() override;
    void pushBoolean(bool b) override;
    void pushInteger(int64_t n) override;
    void pushNumber(double n) override;
    void pushString(std::string s) override;

    Lua_type::Kind typeKind(int idx) override;
    std::string typeName(LUA_TYPE tp) override;
    LUA_TYPE type(int idx) override;
    bool isNone(int idx) override;
    bool isNil(int idx) override;
    bool isNoneOrNil(int idx) override;
    bool isBoolean(int idx) override;
    bool isString(int idx) override;
    bool isNumber(int idx) override;
    bool isInteger(int idx) override;
    bool toBoolean(int idx) override;
    double toNumber(int idx) override;
    double toNumberX(int idx, bool &is0x) override;
    int64_t toInteger(int idx) override;
    int64_t toIntegerX(int idx, bool &is0x) override;
    std::string toStringX(int idx, bool &is0x) override;
    std::string toString(int idx) override;

    void Arith(ArithOp op) override;
    bool Compare(int idx1, int idx2, CompareOp op) override;
    void Len(int idx) override;
    void Concat(int n) override;

    int PC() override;
    void AddPC(int n) override;
    uint32_t Fetch() override;
    void GetConst(int idx) override;
    void GetRK(int rk) override;
};

#endif //LVM_LUA_STATE_H
