//
//  lrparser.c
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#include "lrparser.h"
//＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void program(){
    insymbol();
    if(sym == CONSTSY){     //常量说明
        enum SYMBOL t;
        t = constDec(0);
        while(t == SEMISY){
            if(sym == CONSTSY){
                t = constDec(0);
            }else{
                break;
            }
        }
    }
    if(sym == INTSY || sym == CHARSY){
        while(sym == INTSY || sym == CHARSY){
            enum SYMBOL temp = sym;
            insymbol();
            if(sym == IDENSY){
                char iden_temp[100];
                strcpy(iden_temp, id);
                insymbol();
                if(sym == LPARENTSY){         //有返回值有参数的函数定义
                    regNum = 1;
                    int g;
                    if(temp == INTSY){
                        enter(id, 2, 0, funcNum, 0, 0, 0);
                        func[funcNum].flag = 1;
                        func[funcNum].type = 0;
                        strcpy(func[funcNum++].funcname, iden_temp);
                        struct table a;
                        a = init(a);
                        strcpy(a.name, "int");
                        a.adr = -1;
                        a.type = 0;
                        emit("start", a, tab[0], tab[tab_num-1]);
                    }else{
                        enter(id, 2, 1, funcNum, 0, 0, 0);
                        func[funcNum].flag = 1;
                        func[funcNum].type = 1;
                        strcpy(func[funcNum++].funcname, iden_temp);
                        struct table a;
                        a = init(a);
                        strcpy(a.name, "char");
                        a.adr = -1;
                        a.type = 1;
                        emit("start", a, tab[0], tab[tab_num-1]);
                    }
                    g = tab_num - 1;
                    insymbol();
                    if(sym == INTSY || sym == CHARSY){
                        paramList();
                    }
                    if(sym != RPARENTSY){
                        error(6);
                        while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        continue;
                    }
                    insymbol();
                    if(sym != LBRACESY){
                        error(9);
                        while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        continue;
                    }
                    insymbol();
                    compoundstatement();
                    if(sym != RBRACESY){
                        error(8);
                        while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        continue;
                    }
                    if(func[funcNum-1].retFlag != 1){
                        error(14);
                        while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        continue;
                    }
                    printf("This is a function definition, which has a return value.\n");
                    emit("end", tab[0], tab[0], tab[g]);
                    func[funcNum-1].size = clear();
//                    if(tab[tab_num - 1].kind == 3){
//                        func[funcNum-1].last = tab[tab_num - 1].ref + atab[tab[tab_num - 1].adr].size - 1;
//                    }else{
//                        func[funcNum-1].last = tab[tab_num - 1].ref;
//                    }
//                    func[funcNum-1].last = tab[tab_num - 1].ref;
                    func[funcNum-1].regnum = regNum - 1;
                    insymbol();
                    break;
                }else if(sym == LBRACESY){                  //有返回值无参数的函数定义
                    regNum = 1;
                    int g;
                    if(temp == INTSY){
                        enter(id, 2, 0, funcNum, 0, 0, 0);
                        func[funcNum].flag = 1;
                        func[funcNum].type = 0;
                        strcpy(func[funcNum++].funcname, iden_temp);
                        struct table a;
                        a = init(a);
                        strcpy(a.name, "int");
                        a.adr = -1;
                        a.type = 0;
                        emit("start", a, tab[0], tab[tab_num-1]);
                    }else{
                        enter(id, 2, 1, funcNum, 0, 0, 0);
                        func[funcNum].flag = 1;
                        func[funcNum].type = 1;
                        strcpy(func[funcNum++].funcname, iden_temp);
                        struct table a;
                        a = init(a);
                        strcpy(a.name, "char");
                        a.adr = -1;
                        a.type = 1;
                        emit("start", a, tab[0], tab[tab_num-1]);
                    }
                    g = tab_num - 1;
                    insymbol();
                    compoundstatement();
                    if(sym != RBRACESY){
                        error(8);
                        while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        continue;
                    }
                    if(func[funcNum-1].retFlag != 1){
                        error(14);
                        while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        continue;
                    }
                    printf("This is a function definition, which has a return value.\n");
                    emit("end", tab[0], tab[0], tab[g]);
                    func[funcNum-1].size = clear();
//                    func[funcNum-1].last = tab[tab_num - 1].ref;
                    func[funcNum-1].regnum = regNum - 1;
                    insymbol();
                    break;
                }else if(sym == LBRACKETSY){                //数组变量
                    insymbol();
                    if(sym == NUMSY){
                        insymbol();
                        if(sym != RBRACKETSY){
                            error(10);
                            while (sym != COMMASY && sym != SEMISY) {
                                if (ch == EOF){
                                    exit(0);
                                }
                                insymbol();
                            }
                            continue;
                        }
                        if(temp == INTSY){
                            enter(id, 3, 0, arrNum, 0, number, 0);
                            atab[arrNum].size = number;
                            strcpy(atab[arrNum].arrname, id);
                            atab[arrNum++].type = 0;
                            emit("int", tab[0], tab[0], tab[tab_num-1]);
                        }else{
                            enter(id, 3, 1, arrNum, 0, number, 0);
                            strcpy(atab[arrNum].arrname, id);
                            atab[arrNum].size = number;
                            atab[arrNum++].type = 1;
                            emit("char", tab[0], tab[0], tab[tab_num-1]);
                        }
                        insymbol();
                        if(sym == COMMASY){
                            //                            printf("This is a variable definition./n");
                            varDef(temp, 0);
                            if(sym != SEMISY){
                                error(16);
                                while (sym != INTSY && sym != CHARSY && sym != VOIDSY && sym != CONSTSY && sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){
                                    if (ch == EOF){
                                        exit(0);
                                    }
                                    insymbol();
                                }
                                return;
                            }
                            printf("This is a variable definition.\n");
                            insymbol();
                            continue;
                        }else if(sym == SEMISY){
                            printf("This is a variable definition.\n");
                            insymbol();
                            continue;
                        }else{
                            sym = temp;
                            break;
                        }
                    }else{
                        error(13);
                        while (sym != COMMASY && sym != SEMISY) {
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        continue;
                    }
                }else if(sym == COMMASY){
                    //                    printf("This is a variable definition.\n");
                    if(temp == INTSY){
                        enter(id, 1, 0, tab_num, 0, 1, 0);
                        emit("int", tab[0], tab[0], tab[tab_num-1]);
                    }else{
                        enter(id, 1, 1, tab_num, 0, 1, 0);
                        emit("char", tab[0], tab[0], tab[tab_num-1]);
                    }
                    varDef(temp, 0);
                    if(sym != SEMISY){
                        error(16);
                        while (sym != INTSY && sym != CHARSY && sym != VOIDSY && sym != CONSTSY && sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        return;
                    }
                    printf("This is a variable definition.\n");
                    insymbol();
                    continue;
                }else if(sym == SEMISY){
                    if(temp == INTSY){
                        enter(id, 1, 0, tab_num, 0, 1, 0);
                        emit("int", tab[0], tab[0], tab[tab_num-1]);
                    }else{
                        enter(id, 1, 1, tab_num, 0, 1, 0);
                        emit("char", tab[0], tab[0], tab[tab_num-1]);
                    }
                    printf("This is a variable definition.\n");
                    insymbol();
                    continue;
                }
            }else{
                error(0);
            }
        }
    }
    //函数部分
    while(sym == INTSY || sym == CHARSY || sym == VOIDSY){
        enum SYMBOL temp = sym;
        //有返回值的函数定义
        if(sym == INTSY || sym == CHARSY){
            regNum = 1;
            int g;
            enum SYMBOL temp = sym;
            defHead();
            if(temp == INTSY){
                enter(id, 2, 0, funcNum, 0, 0, 0);
                func[funcNum].type = 0;
                struct table a;
                a = init(a);
                strcpy(a.name, "int");
                a.adr = -1;
                a.type = 0;
                emit("start", a, tab[0], tab[tab_num-1]);
            }else{
                enter(id, 2, 1, funcNum, 0, 0, 0);
                func[funcNum].type = 1;
                struct table a;
                a = init(a);
                strcpy(a.name, "int");
                a.adr = -1;
                a.type = 1;
                emit("start", a, tab[0], tab[tab_num-1]);
            }
            func[funcNum].flag = 1;
            strcpy(func[funcNum++].funcname, id);
            g = tab_num - 1;
            if(sym == LPARENTSY){   //有参数
                insymbol();
                paramList();
                if(sym != RPARENTSY){
                    error(6);
                    while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    continue;
                }
                insymbol();
            }
            if(sym != LBRACESY){
                error(9);
                while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                continue;
            }
            insymbol();
            compoundstatement();
            if(sym != RBRACESY){
                error(8);
                while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                continue;
            }
            if(func[funcNum-1].retFlag != 1){
                error(14);
                while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                continue;
            }
            printf("This is a function definition, which has a return value.\n");
            emit("end", tab[0], tab[0], tab[g]);
            func[funcNum-1].size = clear();
//            func[funcNum-1].last = tab[tab_num - 1].ref;
            func[funcNum-1].regnum = regNum - 1;
            insymbol();
        }
        //无返回值函数定义
        if(sym == VOIDSY){
            regNum = 1;
            int g;
            insymbol();
            if(sym == MAINSY){          //主函数
                enter(id, 2, -1, funcNum, 0, 0, 0);
                func[funcNum].flag = 0;
                strcpy(func[funcNum++].funcname, id);
                emit("start", tab[0], tab[0], tab[tab_num-1]);
                g = tab_num - 1;
                insymbol();
                if(sym != LPARENTSY){
                    error(7);
                    while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    continue;
                }
                insymbol();
                if(sym != RPARENTSY){
                    error(6);
                    while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    continue;
                }
                insymbol();
                if(sym != LBRACESY){
                    error(9);
                    while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    continue;
                }
                insymbol();
                compoundstatement();
                if(sym != RBRACESY){
                    error(8);
                    while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    continue;
                }
                printf("This is a main function definition.\n");
                emit("end", tab[0], tab[0], tab[g]);
                func[funcNum-1].size = clear();
//                func[funcNum-1].last = tab[tab_num - 1].ref;
                func[funcNum-1].regnum = regNum - 1;
                continue;
            }
            //无返回值函数定义
            if(sym != IDENSY){
                error(0);
                while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                continue;
            }
            enter(id, 2, -1, funcNum, 0, 0, 0);
            func[funcNum].flag = 0;
            strcpy(func[funcNum++].funcname, id);
            emit("start", tab[0], tab[0], tab[tab_num-1]);
            g = tab_num - 1;
            insymbol();
            if(sym == LPARENTSY){           //有参数
                insymbol();
                paramList();
                if(sym != RPARENTSY){
                    error(6);
                    while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    continue;
                }
                insymbol();
            }
            if(sym != LBRACESY){
                error(9);
                while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                continue;
            }
            insymbol();
            compoundstatement();
            if(sym != RBRACESY){
                error(8);
                while (sym != INTSY && sym != CHARSY && sym != VOIDSY){
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                continue;
            }
            printf("This is a function definition, which dose not have a return value.\n");
            emit("end", tab[0], tab[0], tab[g]);
            func[funcNum-1].size = clear();
//            func[funcNum-1].last = tab[tab_num - 1].ref;
            func[funcNum-1].regnum = regNum - 1;
            insymbol();
        }
    }
    printf("This is a program definition.\n");
//    printcode();
}

//常量说明
enum SYMBOL constDec(int lev){
    enum SYMBOL per;
    if(sym == CONSTSY){
        insymbol();
        if(sym == INTSY || sym == CHARSY){
            per = sym;
            insymbol();
            constDef(per, lev);
            while(sym == COMMASY){
                insymbol();
                constDef(per, lev);
            }
            if(sym != SEMISY){
                error(16);
                while (sym != INTSY && sym != CHARSY && sym != VOIDSY && sym != CONSTSY && sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                return -1;
            }
            printf("This is a constant declaration.\n");
            insymbol();
            return SEMISY;
        }else{
            error(17);
            while (sym != INTSY && sym != CHARSY && sym != VOIDSY && sym != CONSTSY && sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return -1;
        }
    }else{
        error(18);
        while (sym != INTSY && sym != CHARSY && sym != VOIDSY && sym != CONSTSY && sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return -1;
    }
}

void constDef(enum SYMBOL per, int lev){
    if(sym == IDENSY){
        insymbol();
        if(sym == EQUOPSY){
            insymbol();
            if(sym == ADDOPSY || sym == SUBOPSY){
                enum SYMBOL temp = sym;
                insymbol();
                if(per == INTSY && sym == NUMSY){
                    enter(id, 0, 0, tab_num, 0, 0, lev);
                    if(temp == ADDOPSY){
                        //to do
                        struct table c;
                        c = init(c);
                        strcpy(c.name, cnum);
                        c.type = 0;
                        c.adr = -1;
                        struct table a;
                        a = init(a);
                        strcpy(a.name, "int");
                        a.type = 0;
                        a.adr = -1;
                        emit("const", a, c, tab[tab_num-1]);
                    }else{
                        //to do
                        char str[100];
                        memset(str, '\0', sizeof(str));
                        str[0] = '-';
                        strcat(str, cnum);
                        struct table c;
                        c = init(c);
                        strcpy(c.name, str);
                        c.type = 0;
                        c.adr = -1;
                        struct table a;
                        a = init(a);
                        strcpy(a.name, "int");
                        strcpy(a.name, "int");
                        a.type = 0;
                        a.adr = -1;
                        emit("const", a, c, tab[tab_num-1]);
                    }
                    //                    printf("This is a constant definition.\n");
                }else{
                    error(19);
                    while (sym != COMMASY && sym != SEMISY){
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }
            }else if((per == INTSY && sym == NUMSY) || (per == CHARSY && sym == CHAR)){
                if(per == INTSY){
                    enter(id, 0, 0, tab_num, 0, 0, lev);
                    //to do
                    struct table c;
                    c = init(c);
                    strcpy(c.name, cnum);
                    c.type = 0;
                    c.adr = -1;
                    struct table a;
                    a = init(a);
                    strcpy(a.name, "int");
                    a.type = 0;
                    a.adr = -1;
                    emit("const", a, c, tab[tab_num-1]);
                }else{
                    enter(id, 0, 1, tab_num, 0, 0, lev);
                    //to do
                    struct table c;
                    c = init(c);
                    strcpy(c.name, cha);
                    c.type = 1;
                    c.adr = -1;
                    struct table a;
                    a = init(a);
                    strcpy(a.name, "char");
                    a.type = 0;
                    a.adr = -1;
                    emit("const", a, c, tab[tab_num-1]);
                }
                //                printf("This is a constant definition.\n");
            }else{
                error(19);
                while (sym != COMMASY && sym != SEMISY){
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                return;
            }
        }else{
            error(20);
            while (sym != COMMASY && sym != SEMISY){
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
    }else{
        error(0);
        while (sym != COMMASY && sym != SEMISY){
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    insymbol();
    return;
}

//声明头部
void defHead(){
    if(sym == INTSY || sym == CHARSY){
        insymbol();
        if(sym != IDENSY){
            error(0);
            while (sym != COMMASY && sym != LPARENTSY && sym != RPARENTSY){
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }else{
            //to do
            //            printf("This is a head definition.\n");
        }
    }else{
        error(21);
        while (sym != COMMASY && sym != LPARENTSY && sym != RPARENTSY){
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    insymbol();
    return;
}

//参数表
void paramList(){
    int num = 0;    //记录参数个数
    if(sym == INTSY || sym == CHARSY){
        enum SYMBOL temp = sym;
        do{
            if(sym == COMMASY){
                insymbol();
            }
            defHead();
            if(temp == INTSY){
                enter(id, 1, 0, funcNum-1, 1, 1, 1);
                //to do
            }else if(temp == CHARSY){
                enter(id, 1, 1, funcNum-1, 1, 1, 1);
                //to do
            }
            num++;
        }while(sym == COMMASY);
        //        printf("This is a parameter list.\n");
    }else{
        error(21);
        while (sym != COMMASY && sym != LPARENTSY && sym != RPARENTSY){
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    func[funcNum-1].parnum = num;
    return;
}

//变量定义
void varDef(enum SYMBOL temp, int lev){
    while(sym == COMMASY){
        insymbol();
        if(sym == IDENSY){
            insymbol();
            if(sym == LBRACKETSY){                  //数组变量
                insymbol();
                if(sym != NUMSY){
                    error(13);
                    while (sym != COMMASY && sym != SEMISY) {
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    continue;
                }
                insymbol();
                if(sym != RBRACKETSY){
                    error(10);
                    while (sym != COMMASY && sym != SEMISY) {
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    continue;
                }
                if(temp == INTSY){
                    enter(id, 3, 0, arrNum, 0, number, lev);
                    strcpy(atab[arrNum].arrname, id);
                    atab[arrNum].size = number;
                    atab[arrNum++].type = 0;
                    emit("int", tab[0], tab[0], tab[tab_num-1]);
                }else{
                    enter(id, 3, 1, arrNum, 0, number, lev);
                    strcpy(atab[arrNum].arrname, id);
                    atab[arrNum].size = number;
                    atab[arrNum++].type = 1;
                    emit("char", tab[0], tab[0], tab[tab_num-1]);
                }
                insymbol();
            }else{
                if(temp == INTSY){
                    enter(id, 1, 0, tab_num, 0, 1, lev);
                    emit("int", tab[0], tab[0], tab[tab_num-1]);
                }else{
                    enter(id, 1, 1, tab_num, 0, 1, lev);
                    emit("char", tab[0], tab[0], tab[tab_num-1]);
                }
            }
        }else{
            error(0);
            while (sym != INTSY && sym != CHARSY && sym != VOIDSY && sym != CONSTSY && sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
    }
    return;
}

//复合语句
void compoundstatement(){
    if(sym == CONSTSY){     //常量说明
        enum SYMBOL t;
        t = constDec(1);
        while(t == SEMISY){
            if(sym == CONSTSY){
                t = constDec(1);
            }else{
                break;
            }
        }
    }
    if(sym == INTSY || sym == CHARSY){
        while(sym == INTSY || sym == CHARSY){
            enum SYMBOL temp = sym;
            insymbol();
            if(sym == IDENSY){
                insymbol();
                if(sym == LBRACKETSY){
                    insymbol();
                    if(sym == NUMSY){
                        insymbol();
                        if(sym != RBRACKETSY){
                            error(10);
                            while (sym != COMMASY && sym != SEMISY) {
                                if (ch == EOF){
                                    exit(0);
                                }
                                insymbol();
                            }
                            continue;
                        }
                        if(temp == INTSY){
                            enter(id, 3, 0, arrNum, 0, number, 1);
                            strcpy(atab[arrNum].arrname, id);
                            atab[arrNum].size = number;
                            atab[arrNum++].type = 0;
                            emit("int", tab[0], tab[0], tab[tab_num-1]);
                        }else{
                            enter(id, 3, 1, arrNum, 0, number, 1);
                            strcpy(atab[arrNum].arrname, id);
                            atab[arrNum].size = number;
                            atab[arrNum++].type = 1;
                            emit("char", tab[0], tab[0], tab[tab_num-1]);
                        }
                        insymbol();
                        if(sym == COMMASY){
//                            sym = temp;
                            varDef(temp, 1);
                            if(sym != SEMISY){
                                error(16);
                                while (sym != INTSY && sym != CHARSY && sym != VOIDSY && sym != CONSTSY && sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){
                                    if (ch == EOF){
                                        exit(0);
                                    }
                                    insymbol();
                                }
                                return;
                            }
                            printf("This is a variable definition.\n");
                            insymbol();
                            continue;
                        }else if(sym == SEMISY){
                            printf("This is a variable definition.\n");
                            insymbol();
                            continue;
                        }else{
                            sym = temp;
                            break;
                        }
                    }else{
                        error(13);
                        while (sym != COMMASY && sym != SEMISY) {
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        continue;
                    }
                }else if(sym == COMMASY){
                    if(temp == INTSY){
                        enter(id, 1, 0, tab_num, 0, 1, 1);
                        emit("int", tab[0], tab[0], tab[tab_num-1]);
                    }else{
                        enter(id, 1, 1, tab_num, 0, 1, 1);
                        emit("char", tab[0], tab[0], tab[tab_num-1]);
                    }
                    varDef(temp, 1);
                    if(sym != SEMISY){
                        error(16);
                        while (sym != INTSY && sym != CHARSY && sym != VOIDSY && sym != CONSTSY && sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        return;
                    }
                    printf("This is a variable definition.\n");
                    insymbol();
                    continue;
                }else if(sym == SEMISY){
                    if(temp == INTSY){
                        enter(id, 1, 0, tab_num, 0, 1, 1);
                        emit("int", tab[0], tab[0], tab[tab_num-1]);
                    }else{
                        enter(id, 1, 1, tab_num, 0, 1, 1);
                        emit("char", tab[0], tab[0], tab[tab_num-1]);
                    }
                    printf("This is a variable definition.\n");
                    insymbol();
                    continue;
                }else{
                    error(16);
                    while (sym != INTSY && sym != CHARSY && sym != VOIDSY && sym != CONSTSY && sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }
                
            }else{
                error(0);
                while (sym != INTSY && sym != CHARSY && sym != VOIDSY && sym != CONSTSY && sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                return;
            }
        }
    }
    clear();
    statementList();
    printf("This is a compound statement.\n");
    return;
}

//语句列
void statementList(){
    //    statement();
    while(sym == IFSY || sym == WHILESY || sym == LBRACESY || sym == IDENSY || sym == PRINTFSY || sym == SCANFSY || sym == SWITCHSY || sym == RETURNSY || sym == SEMISY){
        statement();
    }
}

//＜语句＞    ::= ＜条件语句＞｜＜循环语句＞| ‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞;|＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;
void statement(){
    if(sym == IFSY){        //条件语句
        ifstatement();
        return;
    }
    if(sym == WHILESY){         //循环语句
        whilestatement();
        return;
    }
    if(sym == LBRACESY){            //语句列
        insymbol();
        statementList();
        if(sym != RBRACESY){
            error(8);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        insymbol();
        return;
    }
    if(sym == IDENSY){                  //赋值语句或函数调用
        int i, flag = 0;
        int k;
        k = locSym(id, funcNum - 1);
        if(k == 0){
            error(28);
            while (sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        for(i = 0; i < funcNum; i++){
            if(strcmp(id, func[i].funcname) == 0){
                flag = 1;
                break;
            }
        }
        if(flag == 1){               //函数调用
            if(func[i].flag == 1){
                //有返回值函数
                //to do
                insymbol();
                if(sym == LPARENTSY){
                    int j;
                    insymbol();
                    j = vParamList(k);
                    if(j != func[i].parnum){
                        error(27);
                        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        return;
                    }
                    if(sym != RPARENTSY){
                        error(6);
                        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        return;
                    }
                    insymbol();
                    if(sym != SEMISY){
                        error(16);
                        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        return;
                    }
                    insymbol();
                    emit("call", tab[k], tab[0], tab[0]);
                    printf("This is a function call with a return value\n");
                    return;
                }
                if(sym != SEMISY){
                    error(16);
                    while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }
                insymbol();
                emit("call", tab[k], tab[0], tab[0]);
                printf("This is a function call with a return value\n");
                return;
            }else{
                //无返回值函数
                //to do
                insymbol();
                if(sym == LPARENTSY){
                    int j;
                    insymbol();
                    j = vParamList(k);
                    if(j != func[i].parnum){
                        error(27);
                        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        return;
                    }
                    if(sym != RPARENTSY){
                        error(6);
                        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        return;
                    }
                    insymbol();
                    if(sym != SEMISY){
                        error(16);
                        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        return;
                    }
                    insymbol();
                    emit("call", tab[k], tab[0], tab[0]);
                    printf("This is a function call without a return value\n");
                    return;
                }
                if(sym != SEMISY){
                    error(16);
                    while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }
                insymbol();
                emit("call", tab[k], tab[0], tab[0]);
                printf("This is a function call without a return value\n");
                return;
            }
        }
        insymbol();
        if(sym == EQUOPSY){
            if(tab[k].kind != 1){
                if(tab[k].kind == 0){
                    error(29);
                    while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }else{
                    error(30);
                    while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }
            }
            insymbol();
            expression();
            if(sym != SEMISY){
                error(16);
                while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                return;
            }
            
            emit("=", p, tab[0], tab[k]);
            printf("This is a assignment statement.\n");
            insymbol();
            return;
        }else if(sym == LBRACKETSY){
            insymbol();
            expression();
            if(sym != RBRACKETSY){
                error(10);
                while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                return;
            }
            emit("p_arr", tab[0], tab[0], p);
            strcpy(tab[k].arr, p.name);
            char arr[1000];
            strcpy(arr, tab[k].arr);
            insymbol();
            if(sym == EQUOPSY){
                insymbol();
                expression();
                if(sym != SEMISY){
                    error(16);
                    while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }
                insymbol();
                strcpy(tab[k].arr, arr);
                emit("=", p, tab[0], tab[k]);
                return;
            }
        }else{
            error(5);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
    }
    
    if(sym == PRINTFSY){            //写语句
        printfstatement();
        if(sym != SEMISY){
            error(16);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        insymbol();
        return;
    }
    
    if(sym == SCANFSY){         //读语句
        scanfstatement();
        if(sym != SEMISY){
            error(16);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        insymbol();
        return;
    }
    
    if(sym == RETURNSY){                //返回语句
        returnstatement();
        if(sym != SEMISY){
            error(16);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        insymbol();
        return;
    }
    
    if(sym == SWITCHSY){                //情况语句
        switchstatement();
        return;
    }
    
    if(sym == SEMISY){          //空
        insymbol();
        return;
    }
}

//表达式
void expression(){
    struct table place1, place2, place3;
    place1 = init(place1);
    place2 = init(place2);
    place3 = init(place3);
    if(sym == ADDOPSY || sym == SUBOPSY){
        if(sym == ADDOPSY){
            insymbol();
            //to do
            term();
            place3 = equal(place3, p);
        }else{
            insymbol();
            //to do
            term();
            place1 = equal(place1, p);
            place3 = equal(place3, newreg(1, p.type, p.adr));
            emit("neq", place1, tab[0], place3);
        }
    }else{
        term();
        place3 = equal(place3, p);
    }
    while(sym == ADDOPSY || sym == SUBOPSY){
        place1 = equal(place1, place3);
        if(sym == ADDOPSY || sym == SUBOPSY){
            if(sym == ADDOPSY){
                insymbol();
                //to do
                term();
                place2 = equal(place2, p);
                place3 = equal(place3, newreg(1, p.type, p.adr));
                emit("+", place1, place2, place3);
                continue;
            }else{
                insymbol();
                //to do
                term();
                place2 = equal(place2, p);
                place3 = equal(place3, newreg(1, p.type, p.adr));
                emit("-", place1, place2, place3);
                continue;
            }
        }
    }
    //    printf("This is a expression statement.\n");
    p = equal(p, place3);
    return;
}

//项
void term(){
    struct table place1, place2, place3;
    place1 = init(place1);
    place2 = init(place2);
    place3 = init(place3);
    factor();
    place3 = equal(place3, p);
    while(sym == MULOPSY || sym == DIVOPSY){
        place1 = equal(place1, place3);
        if(sym == MULOPSY || sym == DIVOPSY){
            if(sym == MULOPSY){
                insymbol();
                //to do
                factor();
                place2 = equal(place2, p);
                place3 = equal(place3, newreg(1, p.type, p.adr));
                emit("*", place1, place2, place3);
                continue;
            }else{
                insymbol();
                //to do
                factor();
                place2 = equal(place2, p);
                place3 = equal(place3, newreg(1, p.type, p.adr));
                emit("/", place1, place2, place3);
                continue;
            }
        }
    }
    p = equal(p, place3);
    //    printf("This is a term.\n");
    return;
}

//＜因子＞::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’|‘(’＜表达式＞‘)’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞
void factor(){
    struct table place3;
    place3 = init(place3);
    if(sym == IDENSY){
        int k;
        k = locSym(id, funcNum - 1);
        if(k == 0){
            error(28);
            while (sym != COMMASY && sym != RPARENTSY && sym != RBRACKETSY && sym != SEMISY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        int i, flag = 0;
        for(i = 0; i < funcNum; i++){
            if(strcmp(id, func[i].funcname) == 0){
                if(func[i].flag != 1){
                    error(22);
                    while (sym != COMMASY && sym != RPARENTSY && sym != RBRACKETSY && sym != SEMISY){      //to do
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }
                flag = 1;
                break;
            }
        }
        if(flag == 1){
            //有返回值函数调用语句
            //to do
            place3 = equal(place3, newreg(1, func[i].type, i));
            insymbol();
            if(sym == LPARENTSY){
                insymbol();
                int j;
                j = vParamList(k);
                if(j != func[i].parnum){
                    error(27);
                    while (sym != COMMASY && sym != RPARENTSY && sym != RBRACKETSY && sym != SEMISY){      //to do
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }
                if(sym != RPARENTSY){
                    error(6);
                    while (sym != COMMASY && sym != RPARENTSY && sym != RBRACKETSY && sym != SEMISY){      //to do
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }
                emit("call", tab[k], tab[0], place3);
                p = equal(p, place3);
                insymbol();
                printf("This is a function call with a return value\n");
                return;
            }
            emit("call", tab[k], tab[0], place3);
            p = equal(p, place3);
            printf("This is a function call with a return value\n");
            return;
        }else{
            insymbol();
            if(sym == LBRACKETSY){
                place3 = equal(place3, newreg(3, tab[k].type, tab[k].adr));         //new
                insymbol();
                expression();
                if(sym != RBRACKETSY){
                    error(10);
                    while (sym != COMMASY && sym != RPARENTSY && sym != RBRACKETSY && sym != SEMISY){      //to do
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return;
                }
                emit("p_arr", tab[0], tab[0], p);
                strcpy(tab[k].arr, p.name);
                emit("=", tab[k], tab[0], place3);      //new
                p = equal(p, place3);           //new
//                p = equal(p, tab[k]);
                insymbol();
                return;
            }else{
                p = equal(p, tab[k]);
                return;
            }
        }
    }else if(sym == LPARENTSY){
        insymbol();
        expression();
        if(sym != RPARENTSY){
            error(6);
            while (sym != COMMASY && sym != RPARENTSY && sym != RBRACKETSY && sym != SEMISY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        insymbol();
        return;
    }else if(sym == CHAR){
        struct table c;
        c = init(c);
        strcpy(c.name, cha);
        c.kind = 4;
        c.type = 1;
        c.adr = -1;
        p = equal(p, c);
        insymbol();
        return;
    }else if(sym == ADDOPSY ||sym == SUBOPSY){
        enum SYMBOL tmp = sym;
        insymbol();
        if(sym != NUMSY){
            error(13);
            while (sym != COMMASY && sym != RPARENTSY && sym != RBRACKETSY && sym != SEMISY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        if(tmp == ADDOPSY){
            struct table c;
            c = init(c);
            strcpy(c.name, cnum);
            c.type = 0;
            c.adr = -1;
            c.kind = 4;
            p = equal(p, c);
        }else{
            char str[100];
            memset(str, '\0', sizeof(str));
            str[0] = '-';
            strcat(str, cnum);
            struct table c;
            c = init(c);
            strcpy(c.name, str);
            c.kind = 4;
            c.type = 0;
            c.adr = -1;
            p = equal(p, c);
        }
        insymbol();
        return;
    }else if(sym == NUMSY){
        struct table c;
        c = init(c);
        strcpy(c.name, cnum);
        c.kind = 4;
        c.type = 0;
        c.adr = -1;
        p = equal(p, c);
        insymbol();
        return;
    }
}

//条件语句
void ifstatement(){
    struct table label1, label2, requirem;
    label1 = init(label1);
    label2 = init(label2);
    requirem = init(requirem);
    label1 = equal(label1, newtag());
    label2 = equal(label2, newtag());
    insymbol();
    if(sym != LPARENTSY){
        error(7);
        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    insymbol();
    require();
    if(sym != RPARENTSY){
        error(6);
        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    requirem = equal(requirem, p);
    emit("jbe", tab[0], tab[0], label1);                //为假时跳转
    insymbol();
    statement();
    emit("jmp", tab[0], tab[0], label2);
    emit("setlabel", tab[0], tab[0], label1);
    if(sym != ELSESY){
        error(23);
        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != LBRACKETSY){      //to do
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    insymbol();
    statement();
    emit("setlabel", tab[0], tab[0], label2);
    printf("This is a if-else statement.\n");
    return;
}

//条件
void require(){
    struct table place1, place2;
    place1 = init(place1);
    place2 = init(place2);
    expression();
    place1 = equal(place1, p);
    if(sym == BEQSY || sym == BNESY || sym == BGESY || sym == BGTSY || sym == BLESY || sym == BLTSY){
        if(sym == BEQSY){
            //== to do
            insymbol();
            expression();
            place2 = equal(place2, p);
            emit("==", place1, place2, tab[0]);
        }else if(sym == BNESY){
            //!= to do
            insymbol();
            expression();
            place2 = equal(place2, p);
            emit("!=", place1, place2, tab[0]);
        }else if(sym == BGESY){
            //>= to do
            insymbol();
            expression();
            place2 = equal(place2, p);
            emit(">=", place1, place2, tab[0]);
        }else if(sym == BGTSY){
            //> to do
            insymbol();
            expression();
            place2 = equal(place2, p);
            emit(">", place1, place2, tab[0]);
        }else if(sym == BLESY){
            // <= to do
            insymbol();
            expression();
            place2 = equal(place2, p);
            emit("<=", place1, place2, tab[0]);
        }else{
            // < to do
            insymbol();
            expression();
            place2 = equal(place2, p);
            emit("<", place1, place2, tab[0]);
        }
        //        printf("This is a condition statement.\n");
        return;
    }
    struct table c;
    c = init(c);
    strcpy(c.name, "0");
    c.kind = 4;
    c.type = 0;
    c.adr = -1;
    place2 = equal(place2, c);
    emit("!=", place1, place2, tab[0]);
    //    printf("This is a condition statement.\n");
    return;
}

//循环语句
void whilestatement(){
    struct table label1, label2;
    label1 = init(label1);
    label2 = init(label2);
    label1 = equal(label1, newtag());
    label2 = equal(label2, newtag());
    insymbol();
    if(sym != LPARENTSY){
        error(7);
        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    emit("setlabel", tab[0], tab[0], label1);
    insymbol();
    require();
    if(sym != RPARENTSY){
        error(6);
        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    emit("jbe", tab[0], tab[0], label2);                //为假时跳转
    insymbol();
    statement();
    emit("jmp", tab[0], tab[0], label1);
    emit("setlabel", tab[0], tab[0], label2);
    printf("This is a loop statement.\n");
    return;
}

//switch语句
void switchstatement(){
    struct table label1, label2, label3, place2, place3;
    label1 = init(label1);
    label2 = init(label2);
    label3 = init(label3);
    place2 = init(place2);
    place3 = init(place3);
    label3 = equal(label3, newtag());
    label1 = equal(label1, newtag());
    insymbol();
    if(sym != LPARENTSY){
        error(7);
        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    insymbol();
    expression();
    place2 = equal(place2, p);
    if(sym != RPARENTSY){
        error(6);
        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    insymbol();
    if(sym != LBRACESY){
        error(9);
        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    insymbol();
    label1 = casestatement(label1, label2, label3, place2, place3);
    if(sym == DEFAULTSY){
        defaultstatement(label1);
    }else{
        emit("setlabel", tab[0], tab[0], label1);
    }
    if(sym != RBRACESY){
        error(8);
        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY){      //to do
            if (ch == EOF){
                exit(0);
            }
            insymbol();
        }
        return;
    }
    printf("This is a switch-case statement.\n");
    insymbol();
    emit("setlabel", tab[0], tab[0], label3);
    return;
}

//情况表
struct table casestatement(struct table label1, struct table label2, struct table label3, struct table place2, struct table place3){
    if(sym == CASESY){
        while(sym == CASESY){
            emit("setlabel", tab[0], tab[0], label1);
            insymbol();
            if(sym == ADDOPSY || sym == SUBOPSY || sym == NUMSY || sym == CHAR){
                if(sym == ADDOPSY){
                    insymbol();
                    if(sym != NUMSY){
                        error(13);
                        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != CASESY){      //to do
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        return tab[0];
                    }
                    //to do
                    struct table c;
                    c = init(c);
                    strcpy(c.name, cnum);
                    c.kind = 4;
                    c.type = 0;
                    c.adr = -1;
                    place3 = equal(place3, c);
                }else if(sym == SUBOPSY){
                    insymbol();
                    if(sym != NUMSY){
                        error(13);
                        while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != CASESY){      //to do
                            if (ch == EOF){
                                exit(0);
                            }
                            insymbol();
                        }
                        return tab[0];
                    }
                    //to do
                    char str[100];
                    memset(str, '\0', sizeof(str));
                    str[0] = '-';
                    strcat(str, cnum);
                    struct table c;
                    c = init(c);
                    strcpy(c.name, str);
                    c.kind = 4;
                    c.type = 0;
                    c.adr = -1;
                    place3 = equal(place3, c);
                }else if(sym == NUMSY){
                    struct table c;
                    c = init(c);
                    strcpy(c.name, cnum);
                    c.type = 0;
                    c.kind = 4;
                    c.adr = -1;
                    place3 = equal(place3, c);
                }else{
                    struct table c;
                    c = init(c);
                    strcpy(c.name, cha);
                    c.kind = 4;
                    c.type = 1;
                    c.adr = -1;
                    place3 = equal(place3, c);
                }
                emit("!=", place2, place3, tab[0]);
                label1 = equal(label1, newtag());
                label2 = equal(label2, newtag());
                emit("jbe", tab[0], tab[0], label2);                //为假时跳转
                emit("jmp", tab[0], tab[0], label1);
                emit("setlabel", tab[0], tab[0], label2);
                insymbol();
                if(sym != COLONSY){
                    error(24);
                    while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != CASESY){      //to do
                        if (ch == EOF){
                            exit(0);
                        }
                        insymbol();
                    }
                    return tab[0];
                }
                insymbol();
                statement();
                emit("jmp", tab[0], tab[0], label3);
            }else{
                error(18);
                while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != CASESY){      //to do
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                return tab[0];
            }
        }
        return label1;
    }
    return tab[0];
}

//缺省
void defaultstatement(struct table label1){
    if(sym == DEFAULTSY){
        emit("setlabel", tab[0], tab[0], label1);
        insymbol();
        if(sym != COLONSY){
            error(24);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != CASESY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        insymbol();
        statement();
    }
}

//值参数表
int vParamList(int i){
    int j = 0;
    
    do{
        if(sym == COMMASY){
            insymbol();
        }
        if(sym == ADDOPSY || sym == SUBOPSY || sym == IDENSY || sym == LPARENTSY || sym == NUMSY || sym == CHAR){
            expression();
            struct table c;
            c = init(c);
            strcpy(c.name, tab[i].name);
            c.adr = j;
            c.lev = 1;
            emit("vpara", c, tab[0], p);
            j++;
        }else{
            error(25);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != CASESY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return -1;
        }
    }while(sym == COMMASY);
    //    printf("This is a value parameter table.\n");
    return j;
}

//读语句
void scanfstatement(){
    if(sym == SCANFSY){
        insymbol();
        if(sym != LPARENTSY){
            error(7);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        do{
            insymbol();
            if(sym != IDENSY){
                error(0);
                while (sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                return;
            }
            int k;
            k = locSym(id, funcNum-1);
            if(k == 0){
                error(28);
                while (sym != RPARENTSY){      //to do
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                break;
            }
            //to do
            emit("scanf", tab[0], tab[0], tab[k]);
            insymbol();
        }while(sym == COMMASY);
        if(sym != RPARENTSY){
            error(6);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        printf("This is a read statement.\n");
        insymbol();
        return;
    }
}

//写语句
void printfstatement(){
    struct table place1, place2, place3;
    place1 = init(place1);
    place2 = init(place2);
    place3 = init(place3);
    if(sym == PRINTFSY){
        insymbol();
        if(sym != LPARENTSY){
            error(7);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        insymbol();
        if(sym == STRINGSY){
            strcpy(stab[strNum++], string);
            char ss[10];
            char ss2[10];
            strcpy(ss, "str_");
            int q = strNum - 1;
            if(q < 10){
                ss2[0] = q + '0';
                ss2[1] = '\0';
            }else if(q >= 10 && q < 100){
                ss2[0] = q/10 + '0';
                ss2[1] = '0' + q%10;
                ss2[2] = '\0';
            }
            strcat(ss, ss2);
            strcpy(place1.name, ss);
            place1.adr = -1;
            insymbol();
            if(sym == COMMASY){
                insymbol();
                expression();
                place2 = equal(place2, p);
                
            }
        }else{
            expression();
            place2 = equal(place2, p);
        }
        if(sym != RPARENTSY){
            error(6);
            while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                if (ch == EOF){
                    exit(0);
                }
                insymbol();
            }
            return;
        }
        emit("printf", place1, place2, tab[0]);
        printf("This is a write statement.\n");
        insymbol();
        return;
    }
}

//返回语句
void returnstatement(){
    struct table place3;
    place3 = init(place3);
    func[funcNum-1].retFlag = 1;
    if(sym == RETURNSY){
        insymbol();
        if(sym == LPARENTSY){
            insymbol();
            expression();
            place3 = equal(place3, p);
            if(sym != RPARENTSY){
                error(6);
                while (sym != IDENSY && sym != IFSY && sym != WHILESY && sym != SWITCHSY && sym != RETURNSY && sym != PRINTFSY && sym != SCANFSY && sym != ELSESY && sym != LBRACKETSY){      //to do
                    if (ch == EOF){
                        exit(0);
                    }
                    insymbol();
                }
                return;
            }
            emit("return", tab[0], tab[0], place3);
            printf("This is a return statement.\n");
            insymbol();
            return;
        }
        emit("return", tab[0], tab[0], tab[0]);
        printf("This is a return statement.\n");
        return;
    }
}
