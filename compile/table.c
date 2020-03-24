//
//  table.c
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#include "table.h"

//在符号表中定位标识符
int locSym(char* id, int fn){
    int i;
    strcpy(tab[0].name, id);
    for(i = tab_num - 1; i >= 0; i--){
        if(fn == -1){
            if(strcmp(id, tab[i].name) == 0 && tab[i].lev == 0){
                break;
            }
        }else{
            if(strcmp(id, tab[i].name) == 0 && tab[i].lev == 1 && tab[i].func == fn){
                break;
            }
        }
    }
    if(fn != -1){
        if(i < 0){
            for(i = tab_num - 1; i >= 0; i--){
                if(strcmp(id, tab[i].name) == 0 && tab[i].lev == 0){
                    break;
                }
            }
        }
    }
    if(i < 0){
        return 0;
    }
    return i;
}

//登录符号表
int enter(char *name, int kind, int type, int adr, int normal, int size, int lev){
    int i;
    strcpy(tab[0].name, name);
    for(i = tab_num - 1; i >= 0; i--){
        if(strcmp(id, tab[i].name) == 0){
            break;
        }
    }
    if((i != 0 && tab[i].lev == 0 && lev == 0) || (i != 0 && tab[i].lev == 1 && lev == 1 && tab[i].adr == adr)){
        error(26);
        return 0;
    }
    strcpy(tab[tab_num].name, name);
    tab[tab_num].kind = kind;
    tab[tab_num].type = type;
    tab[tab_num].adr = adr;
    tab[tab_num].normal = normal;
    tab[tab_num].size = size;
    tab[tab_num].lev = lev;
    tab_num++;
    return tab_num - 1;
}
