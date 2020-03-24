//
//  midcode.c
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#include "midcode.h"
//生成临时变量
struct table newreg(int kind, int type, int adr){
    struct table reg;
    reg = init(reg);
    reg.kind = kind;
    reg.type = type;
    reg.adr = regNum;
    reg.ref = funcNum - 1;
    reg.name[0] = '$';
    if(regNum < 10){
        reg.name[1] = regNum + '0';
        reg.name[2] = '\0';
    }else if(regNum >= 10 && regNum < 100){
        reg.name[1] = regNum/10 + '0';
        reg.name[2] = '0' + regNum%10;
        reg.name[3] = '\0';
    }
    regNum++;
    return reg;
}

//生成新的label
struct table newtag(){
    struct table tag;
    tag = init(tag);
    tag.adr = tagNum;
    tag.name[0] = 'l';
    tag.name[1] = 'a';
    tag.name[2] = 'b';
    if(tagNum < 10){
        tag.name[3] = tagNum + '0';
        tag.name[4] = '\0';
    }else if(tagNum >= 10 && tagNum < 100){
        tag.name[3] = tagNum/10 + '0';
        tag.name[4] = '0' + tagNum%10;
        tag.name[5] = '\0';
    }
    tagNum++;
    return tag;
}

//四元式
void emit(char op[], struct table a, struct table b, struct table res){
    strcpy(code[emitNum].op, op);
    if(ifequal(b, tab[0]) == 0){
        code[emitNum].b = equal(code[emitNum].b, b);
    }else{
        strcpy(code[emitNum].b.name, "null");
    }
    if(ifequal(res, tab[0]) == 0){
        code[emitNum].res = equal(code[emitNum].res, res);
    }else{
        strcpy(code[emitNum].res.name, "null");
    }
    if(ifequal(a, tab[0]) == 0){
        code[emitNum].a = equal(code[emitNum].a, a);
    }else{
        strcpy(code[emitNum].a.name, "null");
    }
    emitNum++;
}

struct table equal(struct table a, struct table b){
    strcpy(a.name, b.name);
    strcpy(a.arr, b.arr);
    a.adr = b.adr;
    a.kind = b.kind;
    a.lev = b.lev;
    a.normal = b.normal;
    a.ref = b.ref;
    a.size = b.size;
    a.type = b.type;
    a.func = b.func;
    a.reg = b.reg;
    return a;
}

int ifequal(struct table a, struct table b){
    if(strcmp(a.name, b.name)==0 && a.adr == b.adr && a.kind == b.kind && a.lev == b.lev && a.normal == b.normal && a.ref == b.ref && a.size == b.size && a.type == b.type && strcmp(a.arr, b.arr)==0 && a.func == b.func){
        return 1;
    }
    return 0;
}

void printcode(){
    int i;
    for(i = 0; i < emitNum; i++){
        printf("%s %s %s %s\n", code[i].op, code[i].a.name, code[i].b.name, code[i].res.name);
    }
}

struct table init(struct table a){
    memset(a.name, '\0', sizeof(a.name));
    memset(a.arr, '\0', sizeof(a.arr));
    a.adr = 0;
    a.kind = 0;
    a.lev = 0;
    a.normal = 0;
    a.ref = 0;
    a.size = 0;
    a.type = 0;
    a.func = 0;
    a.reg = 0;
    return a;
}

//清空符号表
int clear(){
    int size = 0;
    int x = tab_num;
    int y = locSym(func[funcNum-1].funcname, -1);
    int z = y + 1;
    int ref = 1;
    if(x != y){
        while(x != z){
            tab[z].func = funcNum - 1;
            if(tab[z].kind == 1 || tab[z].kind == 0){
                size += 4;
                tab[z].ref = ref;
                ref++;
            }else if(tab[z].kind == 3){
                size += 4*atab[tab[z].adr].size;
                tab[z].ref = ref;
                ref += atab[tab[z].adr].size;
            }
            z++;
        }
    }
    func[funcNum-1].last = ref - 1;
    return size;
}

void dag(){
    int i,j,k;
    for(i = 0; i < emitNum; i++){
        if(strcmp(code[i].op, "start") == 0){
            break;
        }
    }
    while(strcmp(code[i].op, "start") == 0){
        i++;
        while(strcmp(code[i].op, "end") != 0){
            if(strcmp(code[i].op, "+") == 0 || strcmp(code[i].op, "-") == 0 || strcmp(code[i].op, "*") == 0 || strcmp(code[i].op, "/") == 0 || strcmp(code[i].op, "neq") == 0){
                j = i + 1;
                while(strcmp(code[j].op, "end") != 0){
                    if(strcmp(code[j].op, "=") == 0 && (ifequal(code[j].res, code[i].a) == 1 || ifequal(code[j].res, code[i].b) == 1)){
                        break;
                    }else if(strcmp(code[i].op, code[j].op) == 0 &&((ifequal(code[i].a, code[j].a) == 1 && ifequal(code[i].b, code[j].b) == 1)||(ifequal(code[i].a, code[j].b) == 1 && ifequal(code[i].b, code[j].a) == 1))){
                        k = j + 1;
                        while(strcmp(code[k].op, "end") != 0){
                            if(ifequal(code[k].a, code[j].res)){
                                code[k].a = equal(code[k].a, code[i].res);
                            }
                            if(ifequal(code[k].b, code[j].res)){
                                code[k].b = equal(code[k].b, code[i].res);
                            }
                            strcpy(code[j].op, "null");
                            k++;
                        }
                    }
                    j++;
                }
                i++;
            }else{
                i++;
                continue;
            }
        }
        i++;
    }
}

void activity(){
    struct table temp[1000];
    int tempNum = 0;
    int labref;
    int i, j, k;
    int small, big;
    int flag = 0;
    for(i = 0; i < 10000; i++){
        for(j = 0; j < 10000; j++){
            map[i][j] = 0;
        }
    }
    for(i = emitNum - 1; i >= 0; i--){
        if(strcmp(code[i].op, "setlabel") == 0){
            for(j = 0; j < emitNum; j++){
                if(strcmp(code[j].res.name, code[i].res.name) && strcmp(code[j].op, "setlabel") != 0){
                    labref = j;
                    break;
                }
            }
            if(i > labref){
                small = labref;
                big = i;
            }else{
                small = i;
                big = labref;
            }
            for(k = big; k > small; k--){
                if(strcmp(code[k].op, "+") == 0 || strcmp(code[k].op, "-") == 0 || strcmp(code[k].op, "*") == 0 || strcmp(code[k].op, "/") == 0){
                    for(j = 0; j < tempNum; j++){
                        if(ifequal(code[k].res, temp[j]) == 1){
                            strcpy(temp[j].name, "null");
                            break;
                        }
                    }
                    if(code[k].a.kind != 4){
                        temp[tempNum] = equal(temp[tempNum], code[k].a);
                        tempNum++;
                    }
                    if(code[k].b.kind != 4){
                        temp[tempNum] = equal(temp[tempNum], code[k].b);
                        tempNum++;
                    }
                }else if(strcmp(code[k].op, ">") == 0 || strcmp(code[k].op, "<") == 0 || strcmp(code[k].op, "==") == 0 || strcmp(code[k].op, ">=") == 0 || strcmp(code[k].op, "<=") == 0 || strcmp(code[k].op, "!=") == 0){
                    if(code[k].a.kind != 4){
                        temp[tempNum] = equal(temp[tempNum], code[k].a);
                        tempNum++;
                    }
                    if(code[k].b.kind != 4){
                        temp[tempNum] = equal(temp[tempNum], code[k].b);
                        tempNum++;
                    }
                }else if(strcmp(code[k].op, "neq") == 0 || strcmp(code[k].op, "=") == 0){
                    for(j = 0; j < tempNum; j++){
                        if(ifequal(code[k].res, temp[j]) == 1){
                            strcpy(temp[j].name, "null");
                            break;
                        }
                    }
                    if(code[k].a.kind != 4){
                        temp[tempNum] = equal(temp[tempNum], code[k].a);
                        tempNum++;
                    }
                }
            }
            for(j = 0; j < tempNum; j++){
                for(k = 0 ; k < actNum; k++){
                    if(ifequal(temp[j], actarr2[k]) == 1){
                        actarr[k] = equal(actarr[k], actarr2[k]);
                        flag = 1;
                        break;
                    }
                }
                if(flag != 1){
                    actarr[actNum] = equal(actarr[actNum], temp[j]);
                    actarr2[actNum] = equal(actarr2[actNum], temp[j]);
                    actNum++;
                    for(j = 0; j < actNum-1; j++){
                        if(strcmp(actarr[j].name, "null") != 0){
                            map[j][actNum-1] = 1;
                            map[actNum-1][j] = 1;
                        }
                    }
                }
                flag = 0;
            }
            i = small;
            tempNum = 0;
        }else if(strcmp(code[i].op, "+") == 0 || strcmp(code[i].op, "-") == 0 || strcmp(code[i].op, "*") == 0 || strcmp(code[i].op, "/") == 0){
            for(j = 0; j < actNum; j++){
                if(ifequal(code[i].res, actarr[j]) == 1){
                    strcpy(actarr[j].name, "null");
                    break;
                }
            }
            if(code[i].a.kind != 4){
                for(j = 0; j < actNum; j++){
                    if(ifequal(actarr2[j], code[i].a) == 1){
                        actarr[j] = equal(actarr[j], actarr2[j]);
                        flag = 1;
                        break;
                    }
                }
                if(flag != 1){
                    actarr[actNum] = equal(actarr[actNum], code[i].a);
                    actarr2[actNum] = equal(actarr2[actNum], code[i].a);
                    actNum++;
                    for(j = 0; j < actNum - 1; j++){
                        if(strcmp(actarr[j].name, "null") != 0){
                            map[j][actNum-1] = 1;
                            map[actNum-1][j] = 1;
                        }
                    }
                }
                flag = 0;
            }
            if(code[i].b.kind != 4){
                for(j = 0; j < actNum; j++){
                    if(ifequal(actarr2[j], code[i].b) == 1){
                        actarr[j] = equal(actarr[j], actarr2[j]);
                        flag = 1;
                        break;
                    }
                }
                if(flag != 1){
                    actarr[actNum] = equal(actarr[actNum], code[i].b);
                    actarr2[actNum] = equal(actarr2[actNum], code[i].b);
                    actNum++;
                    for(j = 0; j < actNum - 1; j++){
                        if(strcmp(actarr[j].name, "null") != 0){
                            map[j][actNum-1] = 1;
                            map[actNum-1][j] = 1;
                        }
                    }
                }
                flag = 0;
            }
        }else if(strcmp(code[i].op, ">") == 0 || strcmp(code[i].op, "<") == 0 || strcmp(code[i].op, "==") == 0 || strcmp(code[i].op, ">=") == 0 || strcmp(code[i].op, "<=") == 0 || strcmp(code[i].op, "!=") == 0){
            if(code[i].a.kind != 4){
                for(j = 0; j < actNum; j++){
                    if(ifequal(actarr2[j], code[i].a) == 1){
                        actarr[j] = equal(actarr[j], actarr2[j]);
                        flag = 1;
                        break;
                    }
                }
                if(flag != 1){
                    actarr[actNum] = equal(actarr[actNum], code[i].a);
                    actarr2[actNum] = equal(actarr2[actNum], code[i].a);
                    actNum++;
                    for(j = 0; j < actNum - 1; j++){
                        if(strcmp(actarr[j].name, "null") != 0){
                            map[j][actNum-1] = 1;
                            map[actNum-1][j] = 1;
                        }
                    }
                }
                flag = 0;
            }
            if(code[i].b.kind != 4){
                for(j = 0; j < actNum; j++){
                    if(ifequal(actarr2[j], code[i].b) == 1){
                        actarr[j] = equal(actarr[j], actarr2[j]);
                        flag = 1;
                        break;
                    }
                }
                if(flag != 1){
                    actarr[actNum] = equal(actarr[actNum], code[i].b);
                    actarr2[actNum] = equal(actarr2[actNum], code[i].b);
                    actNum++;
                    for(j = 0; j < actNum - 1; j++){
                        if(strcmp(actarr[j].name, "null") != 0){
                            map[j][actNum-1] = 1;
                            map[actNum-1][j] = 1;
                        }
                    }
                }
                flag = 0;
            }
        }else if(strcmp(code[i].op, "neq") == 0 || strcmp(code[i].op, "=") == 0){
            for(j = 0; j < actNum; j++){
                if(ifequal(code[i].res, actarr[j]) == 1){
                    strcpy(actarr[j].name, "null");
                    break;
                }
            }
            if(code[i].a.kind != 4){
                for(j = 0; j < actNum; j++){
                    if(ifequal(actarr2[j], code[i].a) == 1){
                        actarr[j] = equal(actarr[j], actarr2[j]);
                        flag = 1;
                        break;
                    }
                }
                if(flag != 1){
                    actarr[actNum] = equal(actarr[actNum], code[i].a);
                    actarr2[actNum] = equal(actarr2[actNum], code[i].a);
                    actNum++;
                    for(j = 0; j < actNum - 1; j++){
                        if(strcmp(actarr[j].name, "null") != 0){
                            map[j][actNum-1] = 1;
                            map[actNum-1][j] = 1;
                        }
                    }
                }
                flag = 0;
            }
        }
    }
//    for(i = 0; i < actNum; i++){
//        printf("%s ", actarr2[i].name);
//    }
//    printf("\n");
}

void color(){
    activity();
    int counter[actNum];
    int i, j, q;
    int number = 12;    //t8,t9,s0-s7,k0,k1
    int flag = 0;
    int now;
    for(i = 0; i < actNum; i++){
        counter[i] = 0;
    }
    for(i = 0; i < actNum; i++){
        for(j = 0; j < actNum; j++){
            if(map[i][j] == 1){
                counter[i]++;
            }
        }
    }
    for(q = 0; q < actNum; q++){
        for(i = 0; i < actNum; i++){
            if(counter[i] < number && strcmp(actarr2[i].name, "null") != 0){
                counter[i] = 0;
                order[orderNum] = equal(order[orderNum], actarr2[i]);
                orderNum++;
                strcpy(actarr2[i].name, "null");
                flag = 1;
                for(j = 0; j < actNum; j++){
                    if(strcmp(actarr2[j].name, "null") != 0 && map[j][i] == 1){
                        counter[j]--;
                    }
                }
                break;
            }
        }
        if(flag != 1){
            for(i = 0; i < actNum; i++){
                if(strcmp(actarr2[i].name, "null") != 0){
                    counter[i] = 0;
                    strcpy(actarr2[i].name, "null");
                    for(j = 0; j < actNum; j++){
                        if(strcmp(actarr2[j].name, "null") != 0 && map[j][i] == 1){
                            counter[j]--;
                        }
                    }
                    break;
                }
            }
        }
        flag = 0;
    }
//    for(i = 0; i < orderNum; i++){
//        printf("%s ", order[i].name);
//    }
//    printf("\n");
    order[orderNum-1].reg = 16;
    now = 16;
    for(i = orderNum-2; i>=0; i--){
        for(j = i+1; j < orderNum; j++){
            if(map[i][j] == 0){
                order[i].reg = order[j].reg;
                flag = 1;
                break;
            }
        }
        if(flag != 1 && now < 27){
            order[i].reg = ++now;
        }
        flag = 0;
    }
//    for(i = 0; i < orderNum; i++){
//        printf("%d ", order[i].reg);
//    }
//    printf("\n");
}
