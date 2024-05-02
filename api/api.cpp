/*********************************************
// Created by m1556 on 2024/4/11.
*********************************************/
#include <iostream>

#include "api.h"


LUA_TYPE typeOf(Lua_type *val){
    switch (val->get_type()) {
        case Lua_type::Kind::Nil: {
            return LUA_TNIL;
        }
        case Lua_type::Kind::Boolean: {
            return LUA_TBOOLEAN;
        }
        case Lua_type::Kind::Integer:
        case Lua_type::Kind::Number:{
            return LUA_TNUMBER;
        }
        case Lua_type::Kind::String:{
            return LUA_TSTRING;
        }
        default:{
            std::cerr << "todo!" << std::endl;
            exit(1);
        }
    }
}