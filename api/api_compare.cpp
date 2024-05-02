/*********************************************
// Created by m1556 on 2024/4/21 20:41.
*********************************************/

#include <iostream>

#include "api_compare.h"


bool _eq(Lua_type *a, Lua_type *b) {
    switch (a->get_type()) {
        case Lua_type::Kind::Nil: {
            return b->get_type() == Lua_type::Kind::Nil;
        }
        case Lua_type::Kind::Boolean: {
            if (b->get_type() == Lua_type::Kind::Boolean) {
                return a->as<bool>()->get_value() == b->as<bool>()->get_value();
            } else {
                return convertToBoolean(b) == a->as<bool>()->get_value();
            }
        }
        case Lua_type::Kind::String: {
            if (b->get_type() == Lua_type::Kind::String) {
                return a->as<std::string>()->get_value() == b->as<std::string>()->get_value();
            }
            break;
        }
        case Lua_type::Kind::Integer: {
            if (b->get_type() == Lua_type::Kind::Integer) {
                return a->as<int64_t>()->get_value() == b->as<int64_t>()->get_value();
            } else if (b->get_type() == Lua_type::Kind::Number) {
                return ((double) a->as<int64_t>()->get_value()) == b->as<double>()->get_value();
            } else {
                return false;
            }
        }
        case Lua_type::Kind::Number: {
            if (b->get_type() == Lua_type::Kind::Integer) {
                return a->as<double>()->get_value() == ((double) b->as<int64_t>()->get_value());
            } else if (b->get_type() == Lua_type::Kind::Number) {
                return a->as<double>()->get_value() == b->as<double>()->get_value();
            } else {
                return false;
            }
        }
        default: {
            // need other fix...
            return false;
        }
    }
    return false;
}

bool _lt(Lua_type *a, Lua_type *b) {
    switch (a->get_type()) {
        //// all value isn't countable, think as incomparable
        case Lua_type::Kind::Integer: {
            if (b->get_type() == Lua_type::Kind::Integer) {
                return (a->as<int64_t>()->get_value()) < (b->as<int64_t>()->get_value());
            } else if (b->get_type() == Lua_type::Kind::Number) {
                return ((double) a->as<int64_t>()->get_value()) < (b->as<double>()->get_value());
            }
            break;
        }
        case Lua_type::Kind::Number: {
            if (b->get_type() == Lua_type::Kind::Integer) {
                return (a->as<double>()->get_value()) < ((double) b->as<int64_t>()->get_value());
            } else if (b->get_type() == Lua_type::Kind::Number) {
                return (a->as<double>()->get_value()) < (b->as<double>()->get_value());
            }
            break;
        }
        default: {
            break;
        }
    }
    std::cerr << "comparison error: in lt_" << std::endl;
    exit(1);
}

bool _le(Lua_type *a, Lua_type *b) {
    switch (a->get_type()) {
        //// all value isn't countable, think as incomparable
        case Lua_type::Kind::Integer: {
            if (b->get_type() == Lua_type::Kind::Integer) {
                return (a->as<int64_t>()->get_value()) <= (b->as<int64_t>()->get_value());
            } else if (b->get_type() == Lua_type::Kind::Number) {
                return ((double) a->as<int64_t>()->get_value()) <= (b->as<double>()->get_value());
            }
            break;
        }
        case Lua_type::Kind::Number: {
            if (b->get_type() == Lua_type::Kind::Integer) {
                return (a->as<double>()->get_value()) <= ((double) b->as<int64_t>()->get_value());
            } else if (b->get_type() == Lua_type::Kind::Number) {
                return (a->as<double>()->get_value()) <= (b->as<double>()->get_value());
            }
            break;
        }
        default: {
            break;
        }
    }
    std::cerr << "comparison error: in le_" << std::endl;
    exit(1);
}