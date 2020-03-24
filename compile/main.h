//
//  main.h
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#ifndef main_h
#define main_h
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
extern FILE *file;
extern struct table init(struct table a);
extern void program();
extern void getMips();
#endif /* main_h */
