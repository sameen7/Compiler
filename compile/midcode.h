//
//  midcode.h
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#ifndef midcode_h
#define midcode_h

#include <stdio.h>
extern struct table{           //标识符表
    char name[1000];
    int kind;           //0常量，1变量，2函数, 3array
    int type;           //0int, 1char
    int ref;            //对于数组，记录其首地址；对于函数，记录其入口地址；其余为0；对于类型，记录其所需单元数目；对于常量填入值；对于形参or变量，填入其在运行栈中的位置。
    int adr;            //对于数组，记录其在atab中的位置；对于函数，记录其在ftab中的位置。
    int normal;         //标识符是否是函数形参
    int size;
    int lev;
    char arr[1000];
    int func;
    int reg;
};
extern struct table tab[10000];
extern struct middlecode{
    char op[10];
    struct table a;
    struct table b;
    struct table res;
};
extern struct middlecode code[10000];
extern struct ifRetFunc{           //函数表
    int flag;           //0表示无返回值函数
    char funcname[100];
    int retFlag;            //1表示函数中有return
    int size;
    int lastpar;
    int last;
    int parnum;
    int type;
    int regnum;
};
extern struct arr{           //数组表
    int high;
    int size;
    int type;
    char arrname[100];
    char ref[1000];
};
extern struct arr atab[1000];
extern char stab[1000][1000];           //字符串表
extern struct ifRetFunc func[1000];             //函数表
extern int funcNum;            //函数个数
extern int tab_num;            //符号个数
extern int arrNum;            //数组个数
extern int strNum;            //字符串个数
extern int emitNum;            //四元式个数
extern int regNum;             //临时变量个数
extern int tagNum;             //label个数
extern int m_emitNum;
extern int actNum;
extern struct table p;
extern char id[100];
extern struct table newreg(int kind, int type, int adr);
extern struct table newtag();
extern void emit(char op[], struct table a, struct table b, struct table res);
extern struct table equal(struct table a, struct table b);
extern int ifequal(struct table a, struct table b);
extern void printcode();
extern struct table init(struct table a);
extern int clear();
extern int locSym(char* id, int fn);
extern void dag();
extern void activity();
extern int map[10000][10000];
extern struct table actarr[10000];
extern struct table actarr2[10000];
extern void color();
extern struct table order[1000];
extern int orderNum;
#endif /* midcode_h */
