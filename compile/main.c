//
//  main.c
//  lrparser
//
//  Created by 胡梓义 on 2017/11/28.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

//begin

////在符号表中定位标识符
//int locSym(char* id){
//    int i;
//    strcpy(tab[0].name, id);
//    for(i = tab_num - 1; i >= 0; i--){
//        if(strcmp(id, tab[i].name) == 0){
//            break;
//        }
//    }
//    if(i == 0){
//        return 0;
//    }
//    return i;
//}



int main() {
    char fname[100];
    int i;
    printf("plz input the path of target file\n");
    scanf("%s", fname);
    
    for(i = 0; i < 10000; i++){
        tab[i] = init(tab[i]);
    }
    tab[0].adr = -2;
    tab[0].lev = -2;
    file=fopen(fname,"r");
//    while(ch != EOF){
//        insymbol();
//    }
    program();
    getMips();
//    getMips2();
    return 0;
}

