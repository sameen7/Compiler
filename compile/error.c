//
//  error.c
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#include "error.h"
void error(int n){
    char *msg[] = {
        "wrong identify",       //0非法标识符
        "wrong char",       //1非法字符
        "wrong string",     //2非法字符串
        "single quote",     //3单引号缺失
        "double quote",     //4双引号缺失
        "unknown",       //5非法符号
        "right parent",     //6小括号缺失
        "left parent",
        "right brace",      //8大括号缺失
        "left brace",
        "right bracket",        //10中括号缺失
        "left bracket",
        "!= without a =",       //12
        "expect a number",      //13
        "function without a return value",       //14
        "function with a return value",      //15
        "expected semicolon",         //16
        "constant type",            //17
        "expect a constant",             //18
        "right side of equal sign is wrong",         //19
        "expected initialization of const",              //20
        "wrong key word",            //21
        "expect a function with return value",           //22
        "expect a else",             //23
        "expect a colon",                //24
        "expect an expression",          //25
        "identify redefine",             //26标识符已存在
        "parameter number doesn't match",            //27参数不匹配
        "identify undefine",                //28标识符未定义
        "constant can not be assigned",          //29
        "function can not be assigned"              //30
    };
    printf("error at line %d: %s\n", line, msg[n]);
}
