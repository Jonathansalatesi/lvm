/*********************************************
// Created by m1556 on 2024/3/28.
*********************************************/

#ifndef LVM_LUA_TYPE_H
#define LVM_LUA_TYPE_H
#include <string>


#define TAG_NIL 0x00
#define TAG_BOOLEAN 0x01
#define TAG_NUMBER 0x03
#define TAG_INTEGER 0x13
#define TAG_SHORT_STR 0x04
#define TAG_LONG_STR 0x14

template<typename T>
class Lua_value;

class Lua_type{
public:
    enum class Kind {
        Unknown,
        Nil,
        Boolean,
        Integer,
        Number,
        String
    };

    Kind get_type() {
        return kind;
    };

    virtual Lua_type *copy() = 0;
    virtual ~Lua_type() = default;

    template<class T>
    Lua_value<T> *as() {
        return (Lua_value<T> *) this;
    };

protected:
    Kind kind = Kind::Unknown;
};

template<typename T>
class Lua_value: public Lua_type {
public:
    Lua_value(){
        kind = Kind::Nil;
    };

    explicit Lua_value(T value): value_(value) {
        if (std::is_same_v<T, bool>) {
            kind = Kind::Boolean;
        } else if (std::is_same_v<T, int64_t>) {
            kind = Kind::Integer;
        } else if (std::is_same_v<T, std::string>) {
            kind = Kind::String;
        } else if (std::is_same_v<T, double>){
            kind = Kind::Number;
        } else {
            kind = Kind::Unknown;
        }
    };

    [[nodiscard]]
    Lua_type *copy() override {
        return new Lua_value<T>(*this);
    };

    ~Lua_value() override = default;

    T get_value() {
        return value_;
    };
    void set_value(T value) {
        value_ = value;
    };

private:
    T value_;
};

#endif //LVM_LUA_TYPE_H