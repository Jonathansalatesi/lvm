/*********************************************
// Created by m1556 on 2024/4/13 19:47.
*********************************************/
#include <cmath>
#include <stdexcept>

#include "lua_math.h"



bool convertToBoolean(Lua_type *val){
    switch (val->get_type()) {
        case Lua_type::Kind::Nil:{
            return false;
        }
        case Lua_type::Kind::Boolean:{
            return val->as<bool>()->get_value();
        }
        default:{
            return true;
        }
    }
}

long long IFloorDiv(long long a, long long b){
    if ((a > 0 && b > 0) || (a < 0 && b < 0) || a % b == 0){
        return a / b;
    } else {
        return a / b - 1;
    }
}

double FFloorDiv(double a, double b){
    return floor(a / b);
}

long long IMod(long long a, long long b){
    return a - IFloorDiv(a, b) * b;
}

double FMod(double a, double b){
    return a - floor(a / b) * b;
}

long long ShiftLeft(long long a, long long n){
    if (n >= 0){
        return a << n;
    } else {
        return ShiftRight(a, -n);
    }
}

long long ShiftRight(long long a, long long n){
    if (n >= 0){
        return a >> n;
    } else {
        return ShiftLeft(a, -n);
    }
}

long long FloatToInteger(double f, bool &flag){
    auto ret = (long long) f;
    flag = ((double) ret) == f;
    return ret;
}

long long ParseInteger(std::string str, bool &flag){
    long long val = 0;
    try {
        val = std::stoll(str);
    } catch (const std::invalid_argument& e){
        flag = false;
        return 0;
    } catch (const std::out_of_range& e){
        flag = false;
        return 0;
    } catch (...){
        flag = false;
        return 0;
    }
    flag = true;
    return val;
}

double ParseFloat(std::string str, bool &flag){
    double val = 0;
    try {
        val = std::stod(str);
    } catch (const std::invalid_argument& e){
        flag = false;
        return 0;
    } catch (const std::out_of_range& e){
        flag = false;
        return 0;
    } catch (...){
        flag = false;
        return 0;
    }
    flag = true;
    return val;
}

double convertToFloat(Lua_type *val, bool &flag){
    switch (val->get_type()) {
        case Lua_type::Kind::Number:{
            flag = true;
            return val->as<double>()->get_value();
        }
        case Lua_type::Kind::Integer:{
            flag = true;
            return (double) val->as<long long>()->get_value();
        }
        case Lua_type::Kind::String:{
            return ParseFloat(val->as<std::string>()->get_value(), flag);
        }
        case Lua_type::Kind::Boolean:{
            return val->as<bool>()->get_value();
        }
        default:{
            flag = false;
            return 0;
        }
    }
}

long long convertToInteger(Lua_type *val, bool &flag) {
    switch (val->get_type()) {
        case Lua_type::Kind::Integer: {
            flag = true;
            return val->as<long long>()->get_value();
        }
        case Lua_type::Kind::Number: {
            return FloatToInteger(
                    val->as<double>()->get_value(),
                    flag
                    );
        }
        case Lua_type::Kind::String: {
            return stringToInteger_(
                    val->as<std::string>()->get_value(),
                    flag
                    );
        }
        default: {
            flag = false;
            return 0;
        }
    }
}

long long stringToInteger_(std::string str, bool &flag) {
     bool f_val = false;
     long long val = ParseInteger(str, f_val);
     if (f_val) {
         flag = true;
         return val;
     }

     double d_value = ParseInteger(str, f_val);
    if (f_val) {
        flag = true;
        return (long long)d_value;
    }
    flag = false;

    return 0;
}