/*********************************************
// Created by m1556 on 2024/4/21 20:41.
*********************************************/

#ifndef LVM_API_COMPARE_H
#define LVM_API_COMPARE_H

#include "api.h"
#include "../lua_math.h"


bool _eq(Lua_type *a, Lua_type *b);
bool _lt(Lua_type *a, Lua_type *b);
bool _le(Lua_type *a, Lua_type *b);


#endif //LVM_API_COMPARE_H
