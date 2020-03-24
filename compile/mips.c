//
//  mips.c
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mips.h"

FILE *fp;
FILE *fp2;
void getMips2();
void data2();
void genJump2(int ins, int rs, char *label);
void store2(int a, int st, int base);
void load2(int a, int ld, int base);
void loadgp2(int a, char gp[1000]);
void storegp2(int a, char gp[1000]);
void genconst2(struct middlecode mcode);
void add_sub2(int op, int a, int b, int res);
void add_sub_i2(int op, int a, int imm, int res);
void mul_div2(int op, int a, int b, int res);
void sll_srl2(int op, int a, int b, int imm);
void moveto2(int res, int a);
void compare2(struct middlecode code, int a, int b, int f);
void condition_jump2(int op, int a, int b, char lab[1000]);
void calculate2(struct middlecode mcode, int f);
void call2(struct middlecode mcode, int f);
void genreturn2(struct middlecode mcode, int f);
void genscanf2(struct middlecode mcode, int f);
void genprintf2(struct middlecode mcode, int f);

void data(){
//    printf(".data\n");
    fprintf(fp, ".data\n");
    while(strcmp(code[m_emitNum].op, "start") != 0){
        if(strcmp(code[m_emitNum].op, "const") == 0){
            if(strcmp(code[m_emitNum].a.name, "char") == 0){
//                printf("%s:\t.word %s\n", code[m_emitNum].res.name, code[m_emitNum].b.name);
                fprintf(fp, "%s:\t.word %s\n", code[m_emitNum].res.name, code[m_emitNum].b.name);
            }else{
//                printf("%s:\t.word %s\n", code[m_emitNum].res.name, code[m_emitNum].b.name);
                fprintf(fp, "%s:\t.word %s\n", code[m_emitNum].res.name, code[m_emitNum].b.name);
            }
        }else if(strcmp(code[m_emitNum].op, "int") == 0){
            if(code[m_emitNum].res.kind == 3){
                int k;
                k = locSym(code[m_emitNum].res.name, -1);
//                printf("%s:\t.space %d\n", code[m_emitNum].res.name, 4 * atab[tab[k].adr].size);
                fprintf(fp, "%s:\t.space %d\n", code[m_emitNum].res.name, 4 * atab[tab[k].adr].size);
            }else{
//                printf("%s:\t.space 4\n", code[m_emitNum].res.name);
                fprintf(fp, "%s:\t.space 4\n", code[m_emitNum].res.name);
            }
        }else if(strcmp(code[m_emitNum].op, "char") == 0){
            if(code[m_emitNum].res.kind == 3){
                int k;
                k = locSym(code[m_emitNum].res.name, -1);
//                printf("%s:\t.space %d\n", code[m_emitNum].res.name, 4 * atab[tab[k].adr].size);
                fprintf(fp, "%s:\t.space %d\n", code[m_emitNum].res.name, 4 * atab[tab[k].adr].size);
            }else{
//                printf("%s:\t.space 4\n", code[m_emitNum].res.name);
                fprintf(fp, "%s:\t.space 4\n", code[m_emitNum].res.name);
            }
        }
        m_emitNum++;
    }
    int i;
    for(i = 0; i < strNum; i++){
//        printf("str_%d:\t", i);
        fprintf(fp, "str_%d:\t", i);
//        printf(".asciiz ");
        fprintf(fp, ".asciiz ");
//        printf("%s\n", stab[i]);
        fprintf(fp, "%s\n", stab[i]);
    }
}

void genJump(int ins, int rs, char *label){
    if(ins == JAL || ins == J){
//        printf("\t%s %s\n", mipsIns[ins], label);
        fprintf(fp, "\t%s %s\n", mipsIns[ins], label);
    }else if(ins == JR){
//        printf("\t%s %s\n", mipsIns[ins], mipsReg[rs]);
        fprintf(fp, "\t%s %s\n", mipsIns[ins], mipsReg[rs]);
    }else{
//        printf("Invalid jump instruction\n");
        fprintf(fp, "Invalid jump instruction\n");
    }
//        printf("\t%s\n", mipsIns[NOP]);
    fprintf(fp, "\t%s\n", mipsIns[NOP]);
}

void store(int a, int st, int base){
//    printf("\tsw %s, %d(%s)\n", mipsReg[a], base*4, mipsReg[st]);
    fprintf(fp, "\tsw %s, %d(%s)\n", mipsReg[a], base*4, mipsReg[st]);
}

void load(int a, int ld, int base){
//    printf("\tlw %s, %d(%s)\n", mipsReg[a], base*4, mipsReg[ld]);
    fprintf(fp, "\tlw %s, %d(%s)\n", mipsReg[a], base*4, mipsReg[ld]);
}

void loadgp(int a, char gp[1000]){
//    printf("\tlw %s, %s\n", mipsReg[a], gp);
    fprintf(fp, "\tlw %s, %s\n", mipsReg[a], gp);
}

void storegp(int a, char gp[1000]){
//    printf("\tsw %s, %s\n", mipsReg[a], gp);
    fprintf(fp, "\tsw %s, %s\n", mipsReg[a], gp);
}

void genconst(struct middlecode mcode){
    if(strcmp(mcode.a.name, "char")==0){
        int c = mcode.b.name[1];
//        printf("\tli %s, %d\n", mipsReg[reg_t1], c);
        fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t1], c);
        store(reg_t1, reg_fp, -1*tab[mcode.res.adr].ref);
    }else if(strcmp(mcode.a.name, "int")==0){
        int i = toDigit(mcode.b.name);
//        printf("\tli %s, %d\n", mipsReg[reg_t1], i);
        fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t1], i);
        store(reg_t1, reg_fp, -1*tab[mcode.res.adr].ref);
    }
}

void add_sub(int op, int a, int b, int res){
    if(op == ADD){
//        printf("\tadd %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
        fprintf(fp, "\tadd %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
    }else{
//        printf("\tsub %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
        fprintf(fp, "\tsub %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
    }
}

void add_sub_i(int op, int a, int imm, int res){
    if(op == ADDI){
//        printf("\tadd %s, %s, %d\n", mipsReg[res], mipsReg[a], imm);
        fprintf(fp, "\tadd %s, %s, %d\n", mipsReg[res], mipsReg[a], imm);
    }else{
//        printf("\tsub %s, %s, %d\n", mipsReg[res], mipsReg[a], imm);
        fprintf(fp, "\tsub %s, %s, %d\n", mipsReg[res], mipsReg[a], imm);
    }
}

void mul_div(int op, int a, int b, int res){
    if(op == MUL){
//        printf("\tmul %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
        fprintf(fp, "\tmul %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
    }else{
//        printf("\tdiv %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
        fprintf(fp, "\tdiv %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
    }
}

void sll_srl(int op, int a, int b, int imm){
    if(op == SLL){
//        printf("\tsll %s, %s, %d\n", mipsReg[a], mipsReg[b], imm);
        fprintf(fp, "\tsll %s, %s, %d\n", mipsReg[a], mipsReg[b], imm);
    }
}

void moveto(int res, int a){
//    printf("\tmove %s, %s\n", mipsReg[res], mipsReg[a]);
    fprintf(fp, "\tmove %s, %s\n", mipsReg[res], mipsReg[a]);
}

void compare(struct middlecode mcode, int a, int b, int f){
    int a1, b1;
    if(strcmp(mcode.a.name, "null") == 0){
        
    }else if(mcode.a.name[0] == '$'){
        load(reg_t1, reg_fp, (mcode.a.adr+func[f].last)*(-1));
        a1 = reg_t1;
    }else if(mcode.a.kind == 4){
        if(mcode.a.type == 0){
            int n = toDigit(mcode.a.name);
//            printf("\tli %s, %d\n", mipsReg[reg_t1], n);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t1], n);
            a1 = reg_t1;
        }else{
            int c = mcode.a.name[1];
//            printf("\tli %s, %d\n", mipsReg[reg_t1], c);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t1], c);
            a1 = reg_t1;
        }
    }else if(mcode.a.kind == 3){
        if(mcode.a.lev == 0){
//            printf("\tla %s, %s\n", mipsReg[reg_t3], mcode.a.name);
            fprintf(fp, "\tla %s, %s\n", mipsReg[reg_t3], mcode.a.name);
            if(mcode.a.arr[0] == '$'){
                int i = toDigit(mcode.a.arr);
                load(reg_t1, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t1, reg_t1, 2);
                add_sub(ADD, reg_t3, reg_t1, reg_t3);
                load(reg_t1, reg_t3, 0);
                a1 = reg_t1;
            }else if(locSym(mcode.a.arr, f) == 0){
                int n = toDigit(mcode.a.arr);
                load(reg_t1, reg_t3, n);
                a1 = reg_t1;
            }else{
                int k = locSym(mcode.a.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t1, mcode.a.arr);
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(ADD, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(ADD, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t1, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(ADD, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }
            }
        }else{
            int n = mcode.a.ref*4;
            moveto(reg_t3, reg_fp);
            add_sub_i(ADDI, reg_t3, -1*n, reg_t3);
            if(mcode.a.arr[0] == '$'){
                int i = toDigit(mcode.a.arr);
                load(reg_t1, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t1, reg_t1, 2);
                add_sub(SUB, reg_t3, reg_t1, reg_t3);
                load(reg_t1, reg_t3, 0);
                a1 = reg_t1;
            }else if(locSym(mcode.a.arr, f) == 0){
                int n = toDigit(mcode.a.arr);
                load(reg_t1, reg_t3, -1*n);
                a1 = reg_t1;
            }else{
                int k = locSym(mcode.a.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t1, mcode.a.arr);
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(SUB, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(SUB, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t1, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(SUB, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }
            }
        }
    }else if(mcode.a.lev == 0){
        loadgp(reg_t1, mcode.a.name);
        a1 = reg_t1;
    }else if(mcode.a.lev == 1 && mcode.a.normal == 1){
        load(reg_t1, reg_fp, func[f].parnum-mcode.a.ref+3);     //func[f].parnum - ref + 1 + 2
        a1 = reg_t1;
    }else if(mcode.a.lev == 1 && mcode.a.normal == 0){
        load(reg_t1, reg_fp, (-1)*mcode.a.ref);
        a1 = reg_t1;
    }
    if(strcmp(mcode.b.name, "null") == 0){
        
    }else if(mcode.b.name[0] == '$'){
        load(reg_t2, reg_fp, (mcode.b.adr+func[f].last)*(-1));
        b1 = reg_t2;
    }else if(mcode.b.kind == 4){
        if(mcode.b.type == 0){
            int n = toDigit(mcode.b.name);
//            printf("\tli %s, %d\n", mipsReg[reg_t2], n);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t2], n);
            b1 = reg_t2;
        }else{
            int c = mcode.b.name[1];
//            printf("\tli %s, %d\n", mipsReg[reg_t2], c);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t2], c);
            b1 = reg_t2;
        }
    }else if(mcode.b.kind == 3){
        if(mcode.b.lev == 0){
//            printf("\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
            fprintf(fp, "\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
            if(mcode.b.arr[0] == '$'){
                int i = toDigit(mcode.b.arr);
                load(reg_t2, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t2, reg_t2, 2);
                add_sub(ADD, reg_t3, reg_t2, reg_t3);
                load(reg_t2, reg_t3, 0);
                b1 = reg_t2;
            }else if(locSym(mcode.b.arr, f) == 0){
                int n = toDigit(mcode.b.arr);
                load(reg_t2, reg_t3, n);
                b1 = reg_t2;
            }else{
                int k = locSym(mcode.b.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t2, mcode.b.arr);
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(ADD, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(ADD, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t2, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(ADD, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }
            }
        }else{
            int n = mcode.b.ref*4;
            moveto(reg_t3, reg_fp);
            add_sub_i(ADDI, reg_t3, -1*n, reg_t3);
            if(mcode.b.arr[0] == '$'){
                int i = toDigit(mcode.b.arr);
                load(reg_t2, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t2, reg_t2, 2);
                add_sub(SUB, reg_t3, reg_t2, reg_t3);
                load(reg_t2, reg_t3, 0);
                b1 = reg_t2;
            }else if(locSym(mcode.b.arr, f) == 0){
                int n = toDigit(mcode.b.arr);
                load(reg_t2, reg_t3, -1*n);
                b1 = reg_t2;
            }else{
                int k = locSym(mcode.b.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t2, mcode.b.arr);
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(SUB, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(SUB, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t2, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(SUB, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }
            }
        }
    }else if(mcode.b.lev == 0){
        loadgp(reg_t2, mcode.b.name);
        b1 = reg_t2;
    }else if(mcode.b.lev == 1 && mcode.b.normal == 1){
        load(reg_t2, reg_fp, func[f].parnum-mcode.b.ref+3);     //func[f].parnum - ref + 1 + 2
        b1 = reg_t2;
    }else if(mcode.b.lev == 1 && mcode.b.normal == 0){
        load(reg_t2, reg_fp, (-1)*mcode.b.ref);
        b1 = reg_t2;
    }
    if(strcmp(mcode.op, ">=") == 0){
        m_emitNum++;
        condition_jump(BLT, a1, b1, code[m_emitNum].res.name);
    }else if(strcmp(mcode.op, ">") == 0){
        m_emitNum++;
        condition_jump(BLE, a1, b1, code[m_emitNum].res.name);
    }else if(strcmp(mcode.op, "<=") == 0){
        m_emitNum++;
        condition_jump(BGT, a1, b1, code[m_emitNum].res.name);
    }else if(strcmp(mcode.op, "<") == 0){
        m_emitNum++;
        condition_jump(BGE, a1, b1, code[m_emitNum].res.name);
    }else if(strcmp(mcode.op, "==") == 0){
        m_emitNum++;
        condition_jump(BNE, a1, b1, code[m_emitNum].res.name);
    }else if(strcmp(mcode.op, "!=") == 0){
        m_emitNum++;
        condition_jump(BEQ, a1, b1, code[m_emitNum].res.name);
    }
}

void condition_jump(int op, int a, int b, char lab[1000]){
    if(op == BEQ){
//        printf("\t%s %s, %s, %s\n", mipsIns[BEQ], mipsReg[a], mipsReg[b], lab);
        fprintf(fp, "\t%s %s, %s, %s\n", mipsIns[BEQ], mipsReg[a], mipsReg[b], lab);
    }else if(op == BLT){
//        printf("\t%s %s, %s, %s\n", mipsIns[BLT], mipsReg[a], mipsReg[b], lab);
        fprintf(fp, "\t%s %s, %s, %s\n", mipsIns[BLT], mipsReg[a], mipsReg[b], lab);
    }else if(op == BLE){
//        printf("\t%s %s, %s, %s\n", mipsIns[BLE], mipsReg[a], mipsReg[b], lab);
        fprintf(fp, "\t%s %s, %s, %s\n", mipsIns[BLE], mipsReg[a], mipsReg[b], lab);
    }else if(op == BGT){
//        printf("\t%s %s, %s, %s\n", mipsIns[BGT], mipsReg[a], mipsReg[b], lab);
        fprintf(fp, "\t%s %s, %s, %s\n", mipsIns[BGT], mipsReg[a], mipsReg[b], lab);
    }else if(op == BGE){
//        printf("\t%s %s, %s, %s\n", mipsIns[BGE], mipsReg[a], mipsReg[b], lab);
        fprintf(fp, "\t%s %s, %s, %s\n", mipsIns[BGE], mipsReg[a], mipsReg[b], lab);
    }else if(op == BNE){
//        printf("\t%s %s, %s, %s\n", mipsIns[BNE], mipsReg[a], mipsReg[b], lab);
        fprintf(fp, "\t%s %s, %s, %s\n", mipsIns[BNE], mipsReg[a], mipsReg[b], lab);
    }else if(op == BEQZ){
//        printf("\t%s %s, %s\n", mipsIns[BEQZ], mipsReg[a], lab);
        fprintf(fp, "\t%s %s, %s\n", mipsIns[BEQZ], mipsReg[a], lab);
    }else if(op == BNEZ){
//        printf("\t%s %s, %s\n", mipsIns[BNEZ], mipsReg[a], lab);
        fprintf(fp, "\t%s %s, %s\n", mipsIns[BNEZ], mipsReg[a], lab);
    }else if(op == BGEZ){
//        printf("\t%s %s, %s\n", mipsIns[BGEZ], mipsReg[a], lab);
        fprintf(fp, "\t%s %s, %s\n", mipsIns[BGEZ], mipsReg[a], lab);
    }else if(op == BGTZ){
//        printf("\t%s %s, %s\n", mipsIns[BGTZ], mipsReg[a], lab);
        fprintf(fp, "\t%s %s, %s\n", mipsIns[BGTZ], mipsReg[a], lab);
    }else if(op == BLEZ){
//        printf("\t%s %s, %s\n", mipsIns[BLEZ], mipsReg[a], lab);
        fprintf(fp, "\t%s %s, %s\n", mipsIns[BLEZ], mipsReg[a], lab);
    }else if(op == BLTZ){
//        printf("\t%s %s, %s\n", mipsIns[BLTZ], mipsReg[a], lab);
        fprintf(fp, "\t%s %s, %s\n", mipsIns[BLTZ], mipsReg[a], lab);
    }
}

void calculate(struct middlecode mcode, int f){
    int a1, b1, res1 = 0;
    int flag = 0;
    if(strcmp(mcode.a.name, "null") == 0){
        
    }else if(mcode.a.name[0] == '$'){
        load(reg_t1, reg_fp, (mcode.a.adr+func[f].last)*(-1));
        a1 = reg_t1;
    }else if(mcode.a.kind == 4){
        if(mcode.a.type == 0){
            int n = toDigit(mcode.a.name);
//            printf("\tli %s, %d\n", mipsReg[reg_t1], n);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t1], n);
            a1 = reg_t1;
        }else{
            int c = mcode.a.name[1];
//            printf("\tli %s, %d\n", mipsReg[reg_t1], c);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t1], c);
            a1 = reg_t1;
        }
    }else if(mcode.a.kind == 3){
        if(mcode.a.lev == 0){
//            printf("\tla %s, %s\n", mipsReg[reg_t3], mcode.a.name);
            fprintf(fp, "\tla %s, %s\n", mipsReg[reg_t3], mcode.a.name);
            if(mcode.a.arr[0] == '$'){
                int i = toDigit(mcode.a.arr);
                load(reg_t1, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t1, reg_t1, 2);
                add_sub(ADD, reg_t3, reg_t1, reg_t3);
                load(reg_t1, reg_t3, 0);
                a1 = reg_t1;
            }else if(locSym(mcode.a.arr, f) == 0){
                int n = toDigit(mcode.a.arr);
                load(reg_t1, reg_t3, n);
                a1 = reg_t1;
            }else{
                int k = locSym(mcode.a.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t1, mcode.a.arr);
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(ADD, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(ADD, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t1, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(ADD, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }
            }
        }else{
            int n = mcode.a.ref*4;
            moveto(reg_t3, reg_fp);
            add_sub_i(ADDI, reg_t3, -1*n, reg_t3);
            if(mcode.a.arr[0] == '$'){
                int i = toDigit(mcode.a.arr);
                load(reg_t1, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t1, reg_t1, 2);
                add_sub(SUB, reg_t3, reg_t1, reg_t3);
                load(reg_t1, reg_t3, 0);
                a1 = reg_t1;
            }else if(locSym(mcode.a.arr, f) == 0){
                int n = toDigit(mcode.a.arr);
                load(reg_t1, reg_t3, -1*n);
                a1 = reg_t1;
            }else{
                int k = locSym(mcode.a.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t1, mcode.a.arr);
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(SUB, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(SUB, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t1, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t1, reg_t1, 2);
                    add_sub(SUB, reg_t3, reg_t1, reg_t3);
                    load(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }
            }
        }
    }else if(mcode.a.lev == 0){
        loadgp(reg_t1, mcode.a.name);
        a1 = reg_t1;
    }else if(mcode.a.lev == 1 && mcode.a.normal == 1){
        load(reg_t1, reg_fp, func[f].parnum-mcode.a.ref+3);     //func[f].parnum - ref + 1 + 2
        a1 = reg_t1;
    }else if(mcode.a.lev == 1 && mcode.a.normal == 0){
        load(reg_t1, reg_fp, (-1)*mcode.a.ref);
        a1 = reg_t1;
    }
    if(strcmp(mcode.b.name, "null") == 0){
        
    }else if(mcode.b.name[0] == '$'){
        load(reg_t2, reg_fp, (mcode.b.adr+func[f].last)*(-1));
        b1 = reg_t2;
    }else if(mcode.b.kind == 4){
        if(mcode.b.type == 0){
            int n = toDigit(mcode.b.name);
//            printf("\tli %s, %d\n", mipsReg[reg_t2], n);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t2], n);
            b1 = reg_t2;
        }else{
            int c = mcode.b.name[1];
//            printf("\tli %s, %d\n", mipsReg[reg_t2], c);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t2], c);
            b1 = reg_t2;
        }
    }else if(mcode.b.kind == 3){
        if(mcode.b.lev == 0){
//            printf("\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
            fprintf(fp, "\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
            if(mcode.b.arr[0] == '$'){
                int i = toDigit(mcode.b.arr);
                load(reg_t2, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t2, reg_t2, 2);
                add_sub(ADD, reg_t3, reg_t2, reg_t3);
                load(reg_t2, reg_t3, 0);
                b1 = reg_t2;
            }else if(locSym(mcode.b.arr, f) == 0){
                int n = toDigit(mcode.b.arr);
                load(reg_t2, reg_t3, n);
                b1 = reg_t2;
            }else{
                int k = locSym(mcode.b.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t2, mcode.b.arr);
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(ADD, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(ADD, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t2, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(ADD, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }
            }
        }else{
            int n = mcode.b.ref*4;
            moveto(reg_t3, reg_fp);
            add_sub_i(ADDI, reg_t3, -1*n, reg_t3);
            if(mcode.b.arr[0] == '$'){
                int i = toDigit(mcode.b.arr);
                load(reg_t2, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t2, reg_t2, 2);
                add_sub(SUB, reg_t3, reg_t2, reg_t3);
                load(reg_t2, reg_t3, 0);
                b1 = reg_t2;
            }else if(locSym(mcode.b.arr, f) == 0){
                int n = toDigit(mcode.b.arr);
                load(reg_t2, reg_t3, -1*n);
                b1 = reg_t2;
            }else{
                int k = locSym(mcode.b.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t2, mcode.b.arr);
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(SUB, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(SUB, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t2, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(SUB, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }
            }
        }
    }else if(mcode.b.lev == 0){
        loadgp(reg_t2, mcode.b.name);
        b1 = reg_t2;
    }else if(mcode.b.lev == 1 && mcode.b.normal == 1){
        load(reg_t2, reg_fp, func[f].parnum-mcode.b.ref+3);     //func[f].parnum - ref + 1 + 2
        b1 = reg_t2;
    }else if(mcode.b.lev == 1 && mcode.b.normal == 0){
        load(reg_t2, reg_fp, (-1)*mcode.b.ref);
        b1 = reg_t2;
    }
    if(strcmp(mcode.res.name, "null") == 0){
        
    }else if(mcode.res.name[0] == '$'){
        res1 = (mcode.res.adr+func[f].last)*(-1);
    }else if(mcode.res.kind == 3){
        flag = 1;
        if(mcode.res.lev == 0){
//            printf("\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            fprintf(fp, "\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t4, reg_t4, 2);
                add_sub(ADD, reg_t4, reg_t5, reg_t4);
                res1 = reg_t4;
            }else if(locSym(mcode.res.arr, f) == 0){
                int n = toDigit(mcode.res.arr);
                fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t4], n);
                sll_srl(SLL, reg_t4, reg_t4, 2);
                add_sub(ADD, reg_t4, reg_t5, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t4, mcode.res.arr);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }
            }
        }else{
            int n = mcode.res.ref*4;
            moveto(reg_t5, reg_fp);
            add_sub_i(ADDI, reg_t5, -1*n, reg_t5);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t4, reg_t4, 2);
                add_sub(SUB, reg_t5, reg_t4, reg_t4);
                res1 = reg_t4;
            }else if(locSym(mcode.res.arr, f) == 0){
                int n = toDigit(mcode.res.arr);
                fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t4], n);
                sll_srl(SLL, reg_t4, reg_t4, 2);
                add_sub(SUB, reg_t5, reg_t4, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t4, mcode.res.arr);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }
            }
        }
    }else if(mcode.res.lev == 0){
        flag = -1;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 1){
        //func[f].parnum - ref + 1 + 2
        res1 = func[f].parnum-mcode.res.ref+3;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 0){
        res1 = (-1)*mcode.res.ref;
    }
    if(strcmp(mcode.op, "+") == 0){
        add_sub(ADD, reg_t1, reg_t2, reg_t0);
    }else if(strcmp(mcode.op, "-") == 0){
        add_sub(SUB, reg_t1, reg_t2, reg_t0);
    }else if(strcmp(mcode.op, "*") == 0){
        mul_div(MUL, reg_t1, reg_t2, reg_t0);
    }else if(strcmp(mcode.op, "/") == 0){
        mul_div(DIV, reg_t1, reg_t2, reg_t0);
    }else if(strcmp(code[m_emitNum].op, "=") == 0){
        moveto(reg_t0, reg_t1);
    }else if(strcmp(code[m_emitNum].op, "neq") == 0){
        add_sub(SUB, reg_zero, reg_t1, reg_t0);
    }
    if(flag == 1){
        store(reg_t0, res1, 0);
    }else if(flag != -1){
        store(reg_t0, reg_fp, res1);
    }else{
        storegp(reg_t0, mcode.res.name);
    }
}

void call(struct middlecode mcode, int f){
    int adr = m_emitNum - 5;
    int fn = mcode.a.adr;
    int parnum = func[fn].parnum;
    struct table b;
    b = init(b);
    int b1;
    int res1;
    int flag = 0;
    if(parnum > 0){
        int j;
        int d = 0;
        //        if(strcmp(mcode.res.name, "null") == 0){
        for(j = adr; j < m_emitNum; j++){
            if(strcmp(code[j].op, "vpara") == 0 && strcmp(code[j].a.name, func[fn].funcname) == 0){
                b = equal(b, code[j].res);
                strcpy(code[j].op, "novpara");
                if(b.name[0] == '$'){
                    load(reg_t1, reg_fp, (b.adr+func[f].last)*(-1));
                    b1 = reg_t1;
                }else if(b.kind == 4){
                    if(b.type == 0){
                        int n = toDigit(b.name);
//                        printf("\tli %s, %d\n", mipsReg[reg_t1], n);
                        fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t1], n);
                        b1 = reg_t1;
                    }else{
                        int c = b.name[1];
//                        printf("\tli %s, %d\n", mipsReg[reg_t1], c);
                        fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t1], c);
                        b1 = reg_t1;
                    }
                }else if(b.kind == 3){
                    if(b.lev == 0){
//                        printf("\tla %s, %s\n", mipsReg[reg_t3], b.name);
                        fprintf(fp, "\tla %s, %s\n", mipsReg[reg_t3], b.name);
                        if(b.arr[0] == '$'){
                            int i = toDigit(b.arr);
                            load(reg_t1, reg_fp, (i+func[f].last)*(-1));
                            sll_srl(SLL, reg_t1, reg_t1, 2);
                            add_sub(ADD, reg_t3, reg_t1, reg_t3);
                            load(reg_t1, reg_t3, 0);
                            b1 = reg_t1;
                        }else if(locSym(b.arr, f) == 0){
                            int n = toDigit(b.arr);
                            load(reg_t1, reg_t3, n);
                            b1 = reg_t1;
                        }else{
                            int k = locSym(b.arr, f);
                            if(tab[k].lev == 0){
                                loadgp(reg_t1, b.arr);
                                sll_srl(SLL, reg_t1, reg_t1, 2);
                                add_sub(ADD, reg_t3, reg_t1, reg_t3);
                                load(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }else if(tab[k].lev == 1 && tab[k].normal == 1){
                                load(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                                sll_srl(SLL, reg_t1, reg_t1, 2);
                                add_sub(ADD, reg_t3, reg_t1, reg_t3);
                                load(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }else if(tab[k].lev == 1 && tab[k].normal == 0){
                                load(reg_t1, reg_fp, (-1)*tab[k].ref);
                                sll_srl(SLL, reg_t1, reg_t1, 2);
                                add_sub(ADD, reg_t3, reg_t1, reg_t3);
                                load(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }
                        }
                    }else{
                        int n = b.ref*4;
                        moveto(reg_t3, reg_fp);
                        add_sub_i(ADDI, reg_t3, -1*n, reg_t3);
                        if(b.arr[0] == '$'){
                            int i = toDigit(b.arr);
                            load(reg_t1, reg_fp, (i+func[f].last)*(-1));
                            sll_srl(SLL, reg_t1, reg_t1, 2);
                            add_sub(SUB, reg_t3, reg_t1, reg_t3);
                            load(reg_t1, reg_t3, 0);
                            b1 = reg_t1;
                        }else if(locSym(b.arr, f) == 0){
                            int n = toDigit(b.arr);
                            load(reg_t1, reg_t3, -1*n);
                            b1 = reg_t1;
                        }else{
                            int k = locSym(b.arr, f);
                            if(tab[k].lev == 0){
                                loadgp(reg_t1, b.arr);
                                sll_srl(SLL, reg_t1, reg_t1, 2);
                                add_sub(SUB, reg_t3, reg_t1, reg_t3);
                                load(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }else if(tab[k].lev == 1 && tab[k].normal == 1){
                                load(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                                sll_srl(SLL, reg_t1, reg_t1, 2);
                                add_sub(SUB, reg_t3, reg_t1, reg_t3);
                                load(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }else if(tab[k].lev == 1 && tab[k].normal == 0){
                                load(reg_t1, reg_fp, (-1)*tab[k].ref);
                                sll_srl(SLL, reg_t1, reg_t1, 2);
                                add_sub(SUB, reg_t3, reg_t1, reg_t3);
                                load(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }
                        }
                    }
                }else if(b.lev == 0){
                    loadgp(reg_t1, b.name);
                    b1 = reg_t1;
                }else if(b.lev == 1 && b.normal == 1){
                    load(reg_t1, reg_fp, func[f].parnum-b.ref+3);     //func[f].parnum - ref + 1 + 2
                    b1 = reg_t1;
                }else if(b.lev == 1 && b.normal == 0){
                    load(reg_t1, reg_fp, (-1)*b.ref);
                    b1 = reg_t1;
                }
                store(reg_t1, reg_sp, 0);
                add_sub_i(ADDI, reg_sp, -4, reg_sp);
                d++;
                if(d == parnum){
                    break;
                }
            }
        }
       
        
    }
    //    moveto(reg_s0, reg_sp);
    //    moveto(reg_s1, reg_fp);
    store(reg_sp, reg_sp, 0);
    add_sub_i(ADDI, reg_sp, -4, reg_sp);
    store(reg_fp, reg_sp, 0);
    add_sub_i(ADDI, reg_sp, -4, reg_sp);
    moveto(reg_fp, reg_sp);
    genJump(JAL, 0, mcode.a.name);
    //    moveto(reg_sp, reg_s0);
    //    moveto(reg_fp, reg_s1);
    load(reg_sp, reg_fp, 2);
    load(reg_fp, reg_fp, 1);
    if(strcmp(mcode.res.name, "null") == 0){
        return;
    }else if(mcode.res.name[0] == '$'){
        res1 = (mcode.res.adr+func[f].last)*(-1);
    }else if(mcode.res.kind == 3){
        flag = 1;
        if(mcode.res.lev == 0){
//            printf("\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            fprintf(fp, "\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t4, reg_t4, 2);
                add_sub(ADD, reg_t4, reg_t5, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t4, mcode.res.arr);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }
            }
        }else{
            int n = mcode.res.ref*4;
            moveto(reg_t5, reg_fp);
            add_sub_i(ADDI, reg_t5, -1*n, reg_t5);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t4, reg_t4, 2);
                add_sub(SUB, reg_t5, reg_t4, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t4, mcode.res.arr);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }
            }
        }
    }else if(mcode.res.lev == 0){
        flag = -1;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 1){
        //func[f].parnum - ref + 1 + 2
        res1 = func[f].parnum-mcode.res.ref+3;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 0){
        res1 = (-1)*mcode.res.ref;
    }
    if(flag == 1){
        store(reg_v1, res1, 0);
    }else if(flag != -1){
        store(reg_v1, reg_fp, res1);
    }else{
        storegp(reg_v1, mcode.res.name);
    }
}

void genreturn(struct middlecode mcode, int f){
    int res1;
    int flag = 0;
    if(strcmp(mcode.res.name, "null") == 0){
        load(reg_ra, reg_fp, 0);
        genJump(JR, reg_ra, "");
        return;
    }else if(mcode.res.name[0] == '$'){
        res1 = (mcode.res.adr+func[f].last)*(-1);
    }else if(mcode.res.kind == 4){
        if(mcode.res.type == 0){
            int n = toDigit(mcode.res.name);
//            printf("\tli %s, %d\n", mipsReg[reg_t7], n);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t7], n);
            moveto(reg_v1, reg_t7);
        }else{
            int c = mcode.res.name[1];
//            printf("\tli %s, %d\n", mipsReg[reg_t7], c);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t7], c);
            moveto(reg_v1, reg_t7);
        }
        load(reg_ra, reg_fp, 0);
        genJump(JR, reg_ra, "");
        return;
    }else if(mcode.res.kind == 3){
        flag = 1;
        if(mcode.res.lev == 0){
//            printf("\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            fprintf(fp, "\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t4, reg_t4, 2);
                add_sub(ADD, reg_t4, reg_t5, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t4, mcode.res.arr);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }
            }
        }else{
            int n = mcode.res.ref*4;
            moveto(reg_t5, reg_fp);
            add_sub_i(ADDI, reg_t5, -1*n, reg_t5);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t4, reg_t4, 2);
                add_sub(SUB, reg_t5, reg_t4, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t4, mcode.res.arr);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }
            }
        }
    }else if(mcode.res.lev == 0){
        flag = -1;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 1){
        //func[f].parnum - ref + 1 + 2
        res1 = func[f].parnum-mcode.res.ref+3;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 0){
        res1 = (-1)*mcode.res.ref;
    }
    if(flag == 1){
        load(reg_v1, res1, 0);
    }else if(flag != -1){
        load(reg_v1, reg_fp, res1);
    }else{
        loadgp(reg_v1, mcode.res.name);
    }
    load(reg_ra, reg_fp, 0);
    genJump(JR, reg_ra, "");
}

void genscanf(struct middlecode mcode, int f){
    int res1;
    int flag = 0;
    if(strcmp(mcode.res.name, "null") == 0){
        
    }else if(mcode.res.type == 0){
//        printf("\tli %s, %d\n", mipsReg[reg_v0], 5);
        fprintf(fp, "\tli %s, %d\n", mipsReg[reg_v0], 5);
//        printf("\tsyscall\n");
        fprintf(fp, "\tsyscall\n");
    }else if(mcode.res.type == 1){
//        printf("\tli %s, %d\n", mipsReg[reg_v0], 12);
        fprintf(fp, "\tli %s, %d\n", mipsReg[reg_v0], 12);
//        printf("\tsyscall\n");
        fprintf(fp, "\tsyscall\n");
    }
    if(mcode.res.name[0] == '$'){
        res1 = (mcode.res.adr+func[f].last)*(-1);
    }else if(mcode.res.kind == 3){
        flag = 1;
        if(mcode.res.lev == 0){
//            printf("\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            fprintf(fp, "\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t4, reg_t4, 2);
                add_sub(ADD, reg_t4, reg_t5, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t4, mcode.res.arr);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }
            }
        }else{
            int n = mcode.res.ref*4;
            moveto(reg_t5, reg_fp);
            add_sub_i(ADDI, reg_t5, -1*n, reg_t5);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl(SLL, reg_t4, reg_t4, 2);
                add_sub(SUB, reg_t5, reg_t4, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp(reg_t4, mcode.res.arr);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl(SLL, reg_t4, reg_t4, 2);
                    add_sub(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }
            }
        }
    }else if(mcode.res.lev == 0){
        flag = -1;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 1){
        //func[f].parnum - ref + 1 + 2
        res1 = func[f].parnum-mcode.res.ref+3;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 0){
        res1 = (-1)*mcode.res.ref;
    }
    if(flag == 1){
        store(reg_v0, res1, 0);
    }else if(flag != -1){
        store(reg_v0, reg_fp, res1);
    }else{
        storegp(reg_v0, mcode.res.name);
    }
}

void genprintf(struct middlecode mcode, int f){
    int b1;
    if(strcmp(mcode.a.name, "null") != 0 && strcmp(mcode.a.name, "") != 0){
//        printf("\tli %s, %d\n", mipsReg[reg_v0], 4);
        fprintf(fp, "\tli %s, %d\n", mipsReg[reg_v0], 4);
//        printf("\tla %s, %s\n", mipsReg[reg_a0], mcode.a.name);
        fprintf(fp, "\tla %s, %s\n", mipsReg[reg_a0], mcode.a.name);
//        printf("\tsyscall\n");
        fprintf(fp, "\tsyscall\n");
    }
    if(strcmp(mcode.b.name, "null") != 0 && strcmp(mcode.b.name, "") != 0){
        if(mcode.b.name[0] == '$'){
            load(reg_t2, reg_fp, (mcode.b.adr+func[f].last)*(-1));
            b1 = reg_t2;
        }else if(mcode.b.kind == 4){
            if(mcode.b.type == 0){
                int n = toDigit(mcode.b.name);
//                printf("\tli %s, %d\n", mipsReg[reg_t2], n);
                fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t2], n);
                b1 = reg_t2;
            }else{
                int c = mcode.b.name[1];
//                printf("\tli %s, %d\n", mipsReg[reg_t2], c);
                fprintf(fp, "\tli %s, %d\n", mipsReg[reg_t2], c);
                b1 = reg_t2;
            }
        }else if(mcode.b.kind == 3){
            if(mcode.b.lev == 0){
//                printf("\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
                fprintf(fp, "\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
                if(mcode.b.arr[0] == '$'){
                    int i = toDigit(mcode.b.arr);
                    load(reg_t2, reg_fp, (i+func[f].last)*(-1));
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(ADD, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(locSym(mcode.b.arr, f) == 0){
                    int n = toDigit(mcode.b.arr);
                    load(reg_t2, reg_t3, n);
                    b1 = reg_t2;
                }else{
                    int k = locSym(mcode.b.arr, f);
                    if(tab[k].lev == 0){
                        loadgp(reg_t2, mcode.b.arr);
                        sll_srl(SLL, reg_t2, reg_t2, 2);
                        add_sub(ADD, reg_t3, reg_t2, reg_t3);
                        load(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }else if(tab[k].lev == 1 && tab[k].normal == 1){
                        load(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                        sll_srl(SLL, reg_t2, reg_t2, 2);
                        add_sub(ADD, reg_t3, reg_t2, reg_t3);
                        load(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }else if(tab[k].lev == 1 && tab[k].normal == 0){
                        load(reg_t2, reg_fp, (-1)*tab[k].ref);
                        sll_srl(SLL, reg_t2, reg_t2, 2);
                        add_sub(ADD, reg_t3, reg_t2, reg_t3);
                        load(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }
                }
            }else{
                int n = mcode.b.ref*4;
                moveto(reg_t3, reg_fp);
                add_sub_i(ADDI, reg_t3, -1*n, reg_t3);
                if(mcode.b.arr[0] == '$'){
                    int i = toDigit(mcode.b.arr);
                    load(reg_t2, reg_fp, (i+func[f].last)*(-1));
                    sll_srl(SLL, reg_t2, reg_t2, 2);
                    add_sub(SUB, reg_t3, reg_t2, reg_t3);
                    load(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(locSym(mcode.b.arr, f) == 0){
                    int n = toDigit(mcode.b.arr);
                    load(reg_t2, reg_t3, -1*n);
                    b1 = reg_t2;
                }else{
                    int k = locSym(mcode.b.arr, f);
                    if(tab[k].lev == 0){
                        loadgp(reg_t2, mcode.b.arr);
                        sll_srl(SLL, reg_t2, reg_t2, 2);
                        add_sub(SUB, reg_t3, reg_t2, reg_t3);
                        load(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }else if(tab[k].lev == 1 && tab[k].normal == 1){
                        load(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                        sll_srl(SLL, reg_t2, reg_t2, 2);
                        add_sub(SUB, reg_t3, reg_t2, reg_t3);
                        load(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }else if(tab[k].lev == 1 && tab[k].normal == 0){
                        load(reg_t2, reg_fp, (-1)*tab[k].ref);
                        sll_srl(SLL, reg_t2, reg_t2, 2);
                        add_sub(SUB, reg_t3, reg_t2, reg_t3);
                        load(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }
                }
            }
        }else if(mcode.b.lev == 0){
            loadgp(reg_t2, mcode.b.name);
            b1 = reg_t2;
        }else if(mcode.b.lev == 1 && mcode.b.normal == 1){
            load(reg_t2, reg_fp, func[f].parnum-mcode.b.ref+3);     //func[f].parnum - ref + 1 + 2
            b1 = reg_t2;
        }else if(mcode.b.lev == 1 && mcode.b.normal == 0){
            load(reg_t2, reg_fp, (-1)*mcode.b.ref);
            b1 = reg_t2;
        }
        if(mcode.b.type == 0){
//            printf("\tli %s, %d\n", mipsReg[reg_v0], 1);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_v0], 1);
            moveto(reg_a0, b1);
//            printf("\tsyscall\n");
            fprintf(fp, "\tsyscall\n");
        }else if(mcode.b.type == 1){
//            printf("\tli %s, %d\n", mipsReg[reg_v0], 11);
            fprintf(fp, "\tli %s, %d\n", mipsReg[reg_v0], 11);
            moveto(reg_a0, b1);
//            printf("\tsyscall\n");
            fprintf(fp, "\tsyscall\n");
        }
    }
}

void getMips(){
//    printcode();
//    dag();
//    printf("***************\n");
//    printcode();
//    printf("***************\n");
//    color();
    fp = fopen("/Users/huziyi/code/insymbol/insymbol/result.txt", "w");
    data();
//    printf(".text\n");
    fputs(".text\n", fp);
    moveto(reg_fp, reg_sp);
    genJump(JAL, 0, "main");
//    printf("\tli, %s, %d\n", mipsReg[reg_v0], 10);
    fprintf(fp,"\tli, %s, %d\n", mipsReg[reg_v0], 10);
//    printf("\tsyscall\n");
    fprintf(fp,"\tsyscall\n");
    while(strcmp(code[m_emitNum].op, "start") == 0){
        int fn = locSym(code[m_emitNum].res.name, -1);
        int fref = tab[fn].adr;
//        printf("%s:\n", code[m_emitNum].res.name);
        fprintf(fp,"%s:\n", code[m_emitNum].res.name);
        store(reg_ra, reg_sp, 0);
        add_sub_i(ADDI, reg_sp, -4, reg_sp);
        add_sub_i(ADDI, reg_sp, -1*(func[tab[fn].adr].size + 4 * func[tab[fn].adr].regnum), reg_sp);
        m_emitNum++;
        while(strcmp(code[m_emitNum].op, "end") != 0){
            if(strcmp(code[m_emitNum].op, "int") == 0 || strcmp(code[m_emitNum].op, "char") == 0){
                
            }else if(strcmp(code[m_emitNum].op, "const") == 0){
                genconst(code[m_emitNum]);
            }else if(strcmp(code[m_emitNum].op, ">=") == 0 || strcmp(code[m_emitNum].op, ">") == 0 || strcmp(code[m_emitNum].op, "<=") == 0 || strcmp(code[m_emitNum].op, "<") == 0 || strcmp(code[m_emitNum].op, "==") == 0 || strcmp(code[m_emitNum].op, "!=") == 0){
                compare(code[m_emitNum], 0, 0, fref);
            }else if(strcmp(code[m_emitNum].op, "setlabel") == 0){
//                printf("%s:\n", code[m_emitNum].res.name);
                fprintf(fp,"%s:\n", code[m_emitNum].res.name);
            }else if(strcmp(code[m_emitNum].op, "jmp") == 0){
                genJump(J, 0, code[m_emitNum].res.name);
            }else if(strcmp(code[m_emitNum].op, "+") == 0 || strcmp(code[m_emitNum].op, "-") == 0 || strcmp(code[m_emitNum].op, "*") == 0 || strcmp(code[m_emitNum].op, "/") == 0 || strcmp(code[m_emitNum].op, "=") == 0 || strcmp(code[m_emitNum].op, "neq") == 0){
                calculate(code[m_emitNum], fref);
            }else if(strcmp(code[m_emitNum].op, "call") == 0){
                call(code[m_emitNum], fref);
            }else if(strcmp(code[m_emitNum].op, "return") == 0){
                int ff = locSym(code[m_emitNum+1].res.name, -1);
                int fff = tab[ff].adr;
                genreturn(code[m_emitNum], fref);
            }else if(strcmp(code[m_emitNum].op, "scanf") == 0){
                genscanf(code[m_emitNum], fref);
            }else if(strcmp(code[m_emitNum].op, "printf") == 0){
                genprintf(code[m_emitNum], fref);
            }
            m_emitNum++;
        }
        if(func[code[m_emitNum].res.adr].flag == 0){
            load(reg_ra, reg_fp, 0);
            genJump(JR, reg_ra, "");
        }
        m_emitNum++;
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void data2(){
    //    printf(".data\n");
    fprintf(fp2, ".data\n");
    while(strcmp(code[m_emitNum].op, "start") != 0){
        if(strcmp(code[m_emitNum].op, "const") == 0){
            if(strcmp(code[m_emitNum].a.name, "char") == 0){
                //                printf("%s:\t.word %s\n", code[m_emitNum].res.name, code[m_emitNum].b.name);
                fprintf(fp2, "%s:\t.word %s\n", code[m_emitNum].res.name, code[m_emitNum].b.name);
            }else{
                //                printf("%s:\t.word %s\n", code[m_emitNum].res.name, code[m_emitNum].b.name);
                fprintf(fp2, "%s:\t.word %s\n", code[m_emitNum].res.name, code[m_emitNum].b.name);
            }
        }else if(strcmp(code[m_emitNum].op, "int") == 0){
            if(code[m_emitNum].res.kind == 3){
                int k;
                k = locSym(code[m_emitNum].res.name, -1);
                //                printf("%s:\t.space %d\n", code[m_emitNum].res.name, 4 * atab[tab[k].adr].size);
                fprintf(fp2, "%s:\t.space %d\n", code[m_emitNum].res.name, 4 * atab[tab[k].adr].size);
            }else{
                //                printf("%s:\t.space 4\n", code[m_emitNum].res.name);
                fprintf(fp2, "%s:\t.space 4\n", code[m_emitNum].res.name);
            }
        }else if(strcmp(code[m_emitNum].op, "char") == 0){
            if(code[m_emitNum].res.kind == 3){
                int k;
                k = locSym(code[m_emitNum].res.name, -1);
                //                printf("%s:\t.space %d\n", code[m_emitNum].res.name, 4 * atab[tab[k].adr].size);
                fprintf(fp2, "%s:\t.space %d\n", code[m_emitNum].res.name, 4 * atab[tab[k].adr].size);
            }else{
                //                printf("%s:\t.space 4\n", code[m_emitNum].res.name);
                fprintf(fp2, "%s:\t.space 4\n", code[m_emitNum].res.name);
            }
        }
        m_emitNum++;
    }
    int i;
    for(i = 0; i < strNum; i++){
        //        printf("str_%d:\t", i);
        fprintf(fp2, "str_%d:\t", i);
        //        printf(".asciiz ");
        fprintf(fp2, ".asciiz ");
        //        printf("%s\n", stab[i]);
        fprintf(fp2, "%s\n", stab[i]);
    }
}

void genJump2(int ins, int rs, char *label){
    if(ins == JAL || ins == J){
        //        printf("\t%s %s\n", mipsIns[ins], label);
        fprintf(fp2, "\t%s %s\n", mipsIns[ins], label);
    }else if(ins == JR){
        //        printf("\t%s %s\n", mipsIns[ins], mipsReg[rs]);
        fprintf(fp2, "\t%s %s\n", mipsIns[ins], mipsReg[rs]);
    }else{
        //        printf("Invalid jump instruction\n");
        fprintf(fp2, "Invalid jump instruction\n");
    }
    //        printf("\t%s\n", mipsIns[NOP]);
    fprintf(fp2, "\t%s\n", mipsIns[NOP]);
}

void store2(int a, int st, int base){
    //    printf("\tsw %s, %d(%s)\n", mipsReg[a], base*4, mipsReg[st]);
    fprintf(fp2, "\tsw %s, %d(%s)\n", mipsReg[a], base*4, mipsReg[st]);
}

void load2(int a, int ld, int base){
    //    printf("\tlw %s, %d(%s)\n", mipsReg[a], base*4, mipsReg[ld]);
    fprintf(fp2, "\tlw %s, %d(%s)\n", mipsReg[a], base*4, mipsReg[ld]);
}

void loadgp2(int a, char gp[1000]){
    //    printf("\tlw %s, %s\n", mipsReg[a], gp);
    fprintf(fp2, "\tlw %s, %s\n", mipsReg[a], gp);
}

void storegp2(int a, char gp[1000]){
    //    printf("\tsw %s, %s\n", mipsReg[a], gp);
    fprintf(fp2, "\tsw %s, %s\n", mipsReg[a], gp);
}

void genconst2(struct middlecode mcode){
    if(strcmp(mcode.a.name, "char")==0){
        int c = mcode.b.name[1];
        //        printf("\tli %s, %d\n", mipsReg[reg_t1], c);
        fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t1], c);
        store2(reg_t1, reg_fp, -1*tab[mcode.res.adr].ref);
    }else if(strcmp(mcode.a.name, "int")==0){
        int i = toDigit(mcode.b.name);
        //        printf("\tli %s, %d\n", mipsReg[reg_t1], i);
        fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t1], i);
        store2(reg_t1, reg_fp, -1*tab[mcode.res.adr].ref);
    }
}

void add_sub2(int op, int a, int b, int res){
    if(op == ADD){
        //        printf("\tadd %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
        fprintf(fp2, "\tadd %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
    }else{
        //        printf("\tsub %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
        fprintf(fp2, "\tsub %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
    }
}

void add_sub_i2(int op, int a, int imm, int res){
    if(op == ADDI){
        //        printf("\tadd %s, %s, %d\n", mipsReg[res], mipsReg[a], imm);
        fprintf(fp2, "\tadd %s, %s, %d\n", mipsReg[res], mipsReg[a], imm);
    }else{
        //        printf("\tsub %s, %s, %d\n", mipsReg[res], mipsReg[a], imm);
        fprintf(fp2, "\tsub %s, %s, %d\n", mipsReg[res], mipsReg[a], imm);
    }
}

void mul_div2(int op, int a, int b, int res){
    if(op == MUL){
        //        printf("\tmul %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
        fprintf(fp2, "\tmul %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
    }else{
        //        printf("\tdiv %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
        fprintf(fp2, "\tdiv %s, %s, %s\n", mipsReg[res], mipsReg[a], mipsReg[b]);
    }
}

void sll_srl2(int op, int a, int b, int imm){
    if(op == SLL){
        //        printf("\tsll %s, %s, %d\n", mipsReg[a], mipsReg[b], imm);
        fprintf(fp2, "\tsll %s, %s, %d\n", mipsReg[a], mipsReg[b], imm);
    }
}

void moveto2(int res, int a){
    //    printf("\tmove %s, %s\n", mipsReg[res], mipsReg[a]);
    fprintf(fp2, "\tmove %s, %s\n", mipsReg[res], mipsReg[a]);
}

void compare2(struct middlecode mcode, int a, int b, int f){
    int a1, b1, i, flag = 0;
    for(i = 0; i < orderNum; i++){
        if(ifequal(mcode.a, order[i]) == 1 && order[i].reg != 0){
            flag = 1;
            break;
        }
    }
    if(strcmp(mcode.a.name, "null") == 0){
        
    }else if(flag == 1 && mcode.a.kind != 3 && mcode.a.normal != 1){
        moveto2(reg_t1, order[i].reg);
        a1 = reg_t1;
        flag = 0;
    }else if(mcode.a.name[0] == '$'){
        load2(reg_t1, reg_fp, (mcode.a.adr+func[f].last)*(-1));
        a1 = reg_t1;
    }else if(mcode.a.kind == 4){
        if(mcode.a.type == 0){
            int n = toDigit(mcode.a.name);
            //            printf("\tli %s, %d\n", mipsReg[reg_t1], n);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t1], n);
            a1 = reg_t1;
        }else{
            int c = mcode.a.name[1];
            //            printf("\tli %s, %d\n", mipsReg[reg_t1], c);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t1], c);
            a1 = reg_t1;
        }
    }else if(mcode.a.kind == 3){
        if(mcode.a.lev == 0){
            //            printf("\tla %s, %s\n", mipsReg[reg_t3], mcode.a.name);
            fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_t3], mcode.a.name);
            if(mcode.a.arr[0] == '$'){
                int i = toDigit(mcode.a.arr);
                load2(reg_t1, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t1, reg_t1, 2);
                add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                load2(reg_t1, reg_t3, 0);
                a1 = reg_t1;
            }else if(locSym(mcode.a.arr, f) == 0){
                int n = toDigit(mcode.a.arr);
                load2(reg_t1, reg_t3, n);
                a1 = reg_t1;
            }else{
                int k = locSym(mcode.a.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t1, mcode.a.arr);
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t1, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }
            }
        }else{
            int n = mcode.a.ref*4;
            moveto2(reg_t3, reg_fp);
            add_sub_i2(ADDI, reg_t3, -1*n, reg_t3);
            if(mcode.a.arr[0] == '$'){
                int i = toDigit(mcode.a.arr);
                load2(reg_t1, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t1, reg_t1, 2);
                add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                load2(reg_t1, reg_t3, 0);
                a1 = reg_t1;
            }else if(locSym(mcode.a.arr, f) == 0){
                int n = toDigit(mcode.a.arr);
                load2(reg_t1, reg_t3, -1*n);
                a1 = reg_t1;
            }else{
                int k = locSym(mcode.a.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t1, mcode.a.arr);
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t1, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }
            }
        }
    }else if(mcode.a.lev == 0){
        loadgp2(reg_t1, mcode.a.name);
        a1 = reg_t1;
    }else if(mcode.a.lev == 1 && mcode.a.normal == 1){
        load2(reg_t1, reg_fp, func[f].parnum-mcode.a.ref+3);     //func[f].parnum - ref + 1 + 2
        a1 = reg_t1;
    }else if(mcode.a.lev == 1 && mcode.a.normal == 0){
        load2(reg_t1, reg_fp, (-1)*mcode.a.ref);
        a1 = reg_t1;
    }
    for(i = 0; i < orderNum; i++){
        if(ifequal(mcode.b, order[i]) == 1 && order[i].reg != 0){
            flag = 1;
            break;
        }
    }
    if(strcmp(mcode.b.name, "null") == 0){
        
    }else if(flag == 1 && mcode.b.kind != 3 && mcode.b.normal != 1){
        moveto2(reg_t2, order[i].reg);
        b1 = reg_t2;
        flag = 0;
    }else if(mcode.b.name[0] == '$'){
        load2(reg_t2, reg_fp, (mcode.b.adr+func[f].last)*(-1));
        b1 = reg_t2;
    }else if(mcode.b.kind == 4){
        if(mcode.b.type == 0){
            int n = toDigit(mcode.b.name);
            //            printf("\tli %s, %d\n", mipsReg[reg_t2], n);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t2], n);
            b1 = reg_t2;
        }else{
            int c = mcode.b.name[1];
            //            printf("\tli %s, %d\n", mipsReg[reg_t2], c);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t2], c);
            b1 = reg_t2;
        }
    }else if(mcode.b.kind == 3){
        if(mcode.b.lev == 0){
            //            printf("\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
            fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
            if(mcode.b.arr[0] == '$'){
                int i = toDigit(mcode.b.arr);
                load2(reg_t2, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t2, reg_t2, 2);
                add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                load2(reg_t2, reg_t3, 0);
                b1 = reg_t2;
            }else if(locSym(mcode.b.arr, f) == 0){
                int n = toDigit(mcode.b.arr);
                load2(reg_t2, reg_t3, n);
                b1 = reg_t2;
            }else{
                int k = locSym(mcode.b.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t2, mcode.b.arr);
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t2, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }
            }
        }else{
            int n = mcode.b.ref*4;
            moveto2(reg_t3, reg_fp);
            add_sub_i2(ADDI, reg_t3, -1*n, reg_t3);
            if(mcode.b.arr[0] == '$'){
                int i = toDigit(mcode.b.arr);
                load2(reg_t2, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t2, reg_t2, 2);
                add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                load2(reg_t2, reg_t3, 0);
                b1 = reg_t2;
            }else if(locSym(mcode.b.arr, f) == 0){
                int n = toDigit(mcode.b.arr);
                load2(reg_t2, reg_t3, -1*n);
                b1 = reg_t2;
            }else{
                int k = locSym(mcode.b.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t2, mcode.b.arr);
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t2, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }
            }
        }
    }else if(mcode.b.lev == 0){
        loadgp2(reg_t2, mcode.b.name);
        b1 = reg_t2;
    }else if(mcode.b.lev == 1 && mcode.b.normal == 1){
        load2(reg_t2, reg_fp, func[f].parnum-mcode.b.ref+3);     //func[f].parnum - ref + 1 + 2
        b1 = reg_t2;
    }else if(mcode.b.lev == 1 && mcode.b.normal == 0){
        load2(reg_t2, reg_fp, (-1)*mcode.b.ref);
        b1 = reg_t2;
    }
    if(strcmp(mcode.op, ">=") == 0){
        m_emitNum++;
        condition_jump2(BLT, a1, b1, code[m_emitNum].res.name);
    }else if(strcmp(mcode.op, ">") == 0){
        m_emitNum++;
        condition_jump2(BLE, a1, b1, code[m_emitNum].res.name);
    }else if(strcmp(mcode.op, "<=") == 0){
        m_emitNum++;
        condition_jump2(BGT, a1, b1, code[m_emitNum].res.name);
    }else if(strcmp(mcode.op, "<") == 0){
        m_emitNum++;
        condition_jump2(BGE, a1, b1, code[m_emitNum].res.name);
    }else if(strcmp(mcode.op, "==") == 0){
        m_emitNum++;
        condition_jump2(BNE, a1, b1, code[m_emitNum].res.name);
    }else if(strcmp(mcode.op, "!=") == 0){
        m_emitNum++;
        condition_jump2(BEQ, a1, b1, code[m_emitNum].res.name);
    }
}

void condition_jump2(int op, int a, int b, char lab[1000]){
    if(op == BEQ){
        //        printf("\t%s %s, %s, %s\n", mipsIns[BEQ], mipsReg[a], mipsReg[b], lab);
        fprintf(fp2, "\t%s %s, %s, %s\n", mipsIns[BEQ], mipsReg[a], mipsReg[b], lab);
    }else if(op == BLT){
        //        printf("\t%s %s, %s, %s\n", mipsIns[BLT], mipsReg[a], mipsReg[b], lab);
        fprintf(fp2, "\t%s %s, %s, %s\n", mipsIns[BLT], mipsReg[a], mipsReg[b], lab);
    }else if(op == BLE){
        //        printf("\t%s %s, %s, %s\n", mipsIns[BLE], mipsReg[a], mipsReg[b], lab);
        fprintf(fp2, "\t%s %s, %s, %s\n", mipsIns[BLE], mipsReg[a], mipsReg[b], lab);
    }else if(op == BGT){
        //        printf("\t%s %s, %s, %s\n", mipsIns[BGT], mipsReg[a], mipsReg[b], lab);
        fprintf(fp2, "\t%s %s, %s, %s\n", mipsIns[BGT], mipsReg[a], mipsReg[b], lab);
    }else if(op == BGE){
        //        printf("\t%s %s, %s, %s\n", mipsIns[BGE], mipsReg[a], mipsReg[b], lab);
        fprintf(fp2, "\t%s %s, %s, %s\n", mipsIns[BGE], mipsReg[a], mipsReg[b], lab);
    }else if(op == BNE){
        //        printf("\t%s %s, %s, %s\n", mipsIns[BNE], mipsReg[a], mipsReg[b], lab);
        fprintf(fp2, "\t%s %s, %s, %s\n", mipsIns[BNE], mipsReg[a], mipsReg[b], lab);
    }else if(op == BEQZ){
        //        printf("\t%s %s, %s\n", mipsIns[BEQZ], mipsReg[a], lab);
        fprintf(fp2, "\t%s %s, %s\n", mipsIns[BEQZ], mipsReg[a], lab);
    }else if(op == BNEZ){
        //        printf("\t%s %s, %s\n", mipsIns[BNEZ], mipsReg[a], lab);
        fprintf(fp2, "\t%s %s, %s\n", mipsIns[BNEZ], mipsReg[a], lab);
    }else if(op == BGEZ){
        //        printf("\t%s %s, %s\n", mipsIns[BGEZ], mipsReg[a], lab);
        fprintf(fp2, "\t%s %s, %s\n", mipsIns[BGEZ], mipsReg[a], lab);
    }else if(op == BGTZ){
        //        printf("\t%s %s, %s\n", mipsIns[BGTZ], mipsReg[a], lab);
        fprintf(fp2, "\t%s %s, %s\n", mipsIns[BGTZ], mipsReg[a], lab);
    }else if(op == BLEZ){
        //        printf("\t%s %s, %s\n", mipsIns[BLEZ], mipsReg[a], lab);
        fprintf(fp2, "\t%s %s, %s\n", mipsIns[BLEZ], mipsReg[a], lab);
    }else if(op == BLTZ){
        //        printf("\t%s %s, %s\n", mipsIns[BLTZ], mipsReg[a], lab);
        fprintf(fp2, "\t%s %s, %s\n", mipsIns[BLTZ], mipsReg[a], lab);
    }
}

void calculate2(struct middlecode mcode, int f){
    int a1, b1, res1 = 0, i;
    int flag = 0;
    int flag2 = 0;
    for(i = 0; i < orderNum; i++){
        if(ifequal(mcode.a, order[i]) == 1 && order[i].reg != 0){
            flag2 = 1;
            break;
        }
    }
    if(strcmp(mcode.a.name, "null") == 0){
        
    }else if(flag2 == 1 && mcode.a.kind != 3 && mcode.a.normal != 1){
        moveto2(reg_t1, order[i].reg);
        a1 = reg_t1;
        flag2 = 0;
    }else if(mcode.a.name[0] == '$'){
        load2(reg_t1, reg_fp, (mcode.a.adr+func[f].last)*(-1));
        a1 = reg_t1;
    }else if(mcode.a.kind == 4){
        if(mcode.a.type == 0){
            int n = toDigit(mcode.a.name);
            //            printf("\tli %s, %d\n", mipsReg[reg_t1], n);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t1], n);
            a1 = reg_t1;
        }else{
            int c = mcode.a.name[1];
            //            printf("\tli %s, %d\n", mipsReg[reg_t1], c);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t1], c);
            a1 = reg_t1;
        }
    }else if(mcode.a.kind == 3){
        if(mcode.a.lev == 0){
            //            printf("\tla %s, %s\n", mipsReg[reg_t3], mcode.a.name);
            fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_t3], mcode.a.name);
            if(mcode.a.arr[0] == '$'){
                int i = toDigit(mcode.a.arr);
                load2(reg_t1, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t1, reg_t1, 2);
                add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                load2(reg_t1, reg_t3, 0);
                a1 = reg_t1;
            }else if(locSym(mcode.a.arr, f) == 0){
                int n = toDigit(mcode.a.arr);
                load2(reg_t1, reg_t3, n);
                a1 = reg_t1;
            }else{
                int k = locSym(mcode.a.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t1, mcode.a.arr);
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t1, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }
            }
        }else{
            int n = mcode.a.ref*4;
            moveto2(reg_t3, reg_fp);
            add_sub_i2(ADDI, reg_t3, -1*n, reg_t3);
            if(mcode.a.arr[0] == '$'){
                int i = toDigit(mcode.a.arr);
                load2(reg_t1, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t1, reg_t1, 2);
                add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                load2(reg_t1, reg_t3, 0);
                a1 = reg_t1;
            }else if(locSym(mcode.a.arr, f) == 0){
                int n = toDigit(mcode.a.arr);
                load2(reg_t1, reg_t3, -1*n);
                a1 = reg_t1;
            }else{
                int k = locSym(mcode.a.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t1, mcode.a.arr);
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t1, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t1, reg_t1, 2);
                    add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                    load2(reg_t1, reg_t3, 0);
                    a1 = reg_t1;
                }
            }
        }
    }else if(mcode.a.lev == 0){
        loadgp2(reg_t1, mcode.a.name);
        a1 = reg_t1;
    }else if(mcode.a.lev == 1 && mcode.a.normal == 1){
        load2(reg_t1, reg_fp, func[f].parnum-mcode.a.ref+3);     //func[f].parnum - ref + 1 + 2
        a1 = reg_t1;
    }else if(mcode.a.lev == 1 && mcode.a.normal == 0){
        load2(reg_t1, reg_fp, (-1)*mcode.a.ref);
        a1 = reg_t1;
    }
    for(i = 0; i < orderNum; i++){
        if(ifequal(mcode.b, order[i]) == 1 && order[i].reg != 0){
            flag2 = 1;
            break;
        }
    }
    if(strcmp(mcode.b.name, "null") == 0){
        
    }else if(flag2 == 1 && mcode.b.kind != 3 && mcode.b.normal != 1){
        moveto2(reg_t2, order[i].reg);
        b1 = reg_t2;
        flag2 == 0;
    }else if(mcode.b.name[0] == '$'){
        load2(reg_t2, reg_fp, (mcode.b.adr+func[f].last)*(-1));
        b1 = reg_t2;
    }else if(mcode.b.kind == 4){
        if(mcode.b.type == 0){
            int n = toDigit(mcode.b.name);
            //            printf("\tli %s, %d\n", mipsReg[reg_t2], n);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t2], n);
            b1 = reg_t2;
        }else{
            int c = mcode.b.name[1];
            //            printf("\tli %s, %d\n", mipsReg[reg_t2], c);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t2], c);
            b1 = reg_t2;
        }
    }else if(mcode.b.kind == 3){
        if(mcode.b.lev == 0){
            //            printf("\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
            fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
            if(mcode.b.arr[0] == '$'){
                int i = toDigit(mcode.b.arr);
                load2(reg_t2, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t2, reg_t2, 2);
                add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                load2(reg_t2, reg_t3, 0);
                b1 = reg_t2;
            }else if(locSym(mcode.b.arr, f) == 0){
                int n = toDigit(mcode.b.arr);
                load2(reg_t2, reg_t3, n);
                b1 = reg_t2;
            }else{
                int k = locSym(mcode.b.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t2, mcode.b.arr);
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t2, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }
            }
        }else{
            int n = mcode.b.ref*4;
            moveto2(reg_t3, reg_fp);
            add_sub_i2(ADDI, reg_t3, -1*n, reg_t3);
            if(mcode.b.arr[0] == '$'){
                int i = toDigit(mcode.b.arr);
                load2(reg_t2, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t2, reg_t2, 2);
                add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                load2(reg_t2, reg_t3, 0);
                b1 = reg_t2;
            }else if(locSym(mcode.b.arr, f) == 0){
                int n = toDigit(mcode.b.arr);
                load2(reg_t2, reg_t3, -1*n);
                b1 = reg_t2;
            }else{
                int k = locSym(mcode.b.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t2, mcode.b.arr);
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t2, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }
            }
        }
    }else if(mcode.b.lev == 0){
        loadgp2(reg_t2, mcode.b.name);
        b1 = reg_t2;
    }else if(mcode.b.lev == 1 && mcode.b.normal == 1){
        load2(reg_t2, reg_fp, func[f].parnum-mcode.b.ref+3);     //func[f].parnum - ref + 1 + 2
        b1 = reg_t2;
    }else if(mcode.b.lev == 1 && mcode.b.normal == 0){
        load2(reg_t2, reg_fp, (-1)*mcode.b.ref);
        b1 = reg_t2;
    }
    for(i = 0; i < orderNum; i++){
        if(ifequal(mcode.res, order[i]) == 1 && order[i].reg != 0){
            flag2 = 1;
            break;
        }
    }
    if(strcmp(mcode.res.name, "null") == 0){
        
    }else if(flag2 == 1 && mcode.res.kind != 3 && mcode.res.normal != 1){
        
    }else if(mcode.res.name[0] == '$'){
        res1 = (mcode.res.adr+func[f].last)*(-1);
    }else if(mcode.res.kind == 3){
        flag = 1;
        if(mcode.res.lev == 0){
            //            printf("\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load2(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t4, reg_t4, 2);
                add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                res1 = reg_t4;
            }else if(locSym(mcode.res.arr, f) == 0){
                int n = toDigit(mcode.res.arr);
                fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t4], n);
                sll_srl2(SLL, reg_t4, reg_t4, 2);
                add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t4, mcode.res.arr);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }
            }
        }else{
            int n = mcode.res.ref*4;
            moveto2(reg_t5, reg_fp);
            add_sub_i2(ADDI, reg_t5, -1*n, reg_t5);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load2(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t4, reg_t4, 2);
                add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                res1 = reg_t4;
            }else if(locSym(mcode.res.arr, f) == 0){
                int n = toDigit(mcode.res.arr);
                fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t4], n);
                sll_srl2(SLL, reg_t4, reg_t4, 2);
                add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t4, mcode.res.arr);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }
            }
        }
    }else if(mcode.res.lev == 0){
        flag = -1;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 1){
        //func[f].parnum - ref + 1 + 2
        res1 = func[f].parnum-mcode.res.ref+3;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 0){
        res1 = (-1)*mcode.res.ref;
    }
    
    if(strcmp(mcode.op, "+") == 0){
        add_sub2(ADD, reg_t1, reg_t2, reg_t0);
    }else if(strcmp(mcode.op, "-") == 0){
        add_sub2(SUB, reg_t1, reg_t2, reg_t0);
    }else if(strcmp(mcode.op, "*") == 0){
        mul_div2(MUL, reg_t1, reg_t2, reg_t0);
    }else if(strcmp(mcode.op, "/") == 0){
        mul_div2(DIV, reg_t1, reg_t2, reg_t0);
    }else if(strcmp(code[m_emitNum].op, "=") == 0){
        moveto2(reg_t0, reg_t1);
    }else if(strcmp(code[m_emitNum].op, "neq") == 0){
        add_sub2(SUB, reg_zero, reg_t1, reg_t0);
    }
    if(flag2 == 1 && mcode.res.kind != 3 && mcode.res.normal != 1){
        moveto2(order[i].reg, reg_t0);
        flag2 = 0;
    }else if(flag == 1){
        store2(reg_t0, res1, 0);
    }else if(flag != -1){
        store2(reg_t0, reg_fp, res1);
    }else{
        storegp2(reg_t0, mcode.res.name);
    }
}

void call2(struct middlecode mcode, int f){
    int adr = m_emitNum - 5;
    int fn = mcode.a.adr;
    int parnum = func[fn].parnum;
    struct table b;
    b = init(b);
    int b1;
    int res1;
    int flag = 0;
    if(parnum > 0){
        int j;
        int d = 0;
        //        if(strcmp(mcode.res.name, "null") == 0){
        for(j = adr; j < m_emitNum; j++){
            if(strcmp(code[j].op, "vpara") == 0 && strcmp(code[j].a.name, func[fn].funcname) == 0){
                b = equal(b, code[j].res);
                strcpy(code[j].op, "novpara");
                if(b.name[0] == '$'){
                    load2(reg_t1, reg_fp, (b.adr+func[f].last)*(-1));
                    b1 = reg_t1;
                }else if(b.kind == 4){
                    if(b.type == 0){
                        int n = toDigit(b.name);
                        //                        printf("\tli %s, %d\n", mipsReg[reg_t1], n);
                        fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t1], n);
                        b1 = reg_t1;
                    }else{
                        int c = b.name[1];
                        //                        printf("\tli %s, %d\n", mipsReg[reg_t1], c);
                        fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t1], c);
                        b1 = reg_t1;
                    }
                }else if(b.kind == 3){
                    if(b.lev == 0){
                        //                        printf("\tla %s, %s\n", mipsReg[reg_t3], b.name);
                        fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_t3], b.name);
                        if(b.arr[0] == '$'){
                            int i = toDigit(b.arr);
                            load2(reg_t1, reg_fp, (i+func[f].last)*(-1));
                            sll_srl2(SLL, reg_t1, reg_t1, 2);
                            add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                            load2(reg_t1, reg_t3, 0);
                            b1 = reg_t1;
                        }else if(locSym(b.arr, f) == 0){
                            int n = toDigit(b.arr);
                            load2(reg_t1, reg_t3, n);
                            b1 = reg_t1;
                        }else{
                            int k = locSym(b.arr, f);
                            if(tab[k].lev == 0){
                                loadgp2(reg_t1, b.arr);
                                sll_srl2(SLL, reg_t1, reg_t1, 2);
                                add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                                load2(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }else if(tab[k].lev == 1 && tab[k].normal == 1){
                                load2(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                                sll_srl2(SLL, reg_t1, reg_t1, 2);
                                add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                                load2(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }else if(tab[k].lev == 1 && tab[k].normal == 0){
                                load2(reg_t1, reg_fp, (-1)*tab[k].ref);
                                sll_srl2(SLL, reg_t1, reg_t1, 2);
                                add_sub2(ADD, reg_t3, reg_t1, reg_t3);
                                load2(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }
                        }
                    }else{
                        int n = b.ref*4;
                        moveto2(reg_t3, reg_fp);
                        add_sub_i2(ADDI, reg_t3, -1*n, reg_t3);
                        if(b.arr[0] == '$'){
                            int i = toDigit(b.arr);
                            load2(reg_t1, reg_fp, (i+func[f].last)*(-1));
                            sll_srl2(SLL, reg_t1, reg_t1, 2);
                            add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                            load2(reg_t1, reg_t3, 0);
                            b1 = reg_t1;
                        }else if(locSym(b.arr, f) == 0){
                            int n = toDigit(b.arr);
                            load2(reg_t1, reg_t3, -1*n);
                            b1 = reg_t1;
                        }else{
                            int k = locSym(b.arr, f);
                            if(tab[k].lev == 0){
                                loadgp2(reg_t1, b.arr);
                                sll_srl2(SLL, reg_t1, reg_t1, 2);
                                add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                                load2(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }else if(tab[k].lev == 1 && tab[k].normal == 1){
                                load2(reg_t1, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                                sll_srl2(SLL, reg_t1, reg_t1, 2);
                                add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                                load2(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }else if(tab[k].lev == 1 && tab[k].normal == 0){
                                load2(reg_t1, reg_fp, (-1)*tab[k].ref);
                                sll_srl2(SLL, reg_t1, reg_t1, 2);
                                add_sub2(SUB, reg_t3, reg_t1, reg_t3);
                                load2(reg_t1, reg_t3, 0);
                                b1 = reg_t1;
                            }
                        }
                    }
                }else if(b.lev == 0){
                    loadgp2(reg_t1, b.name);
                    b1 = reg_t1;
                }else if(b.lev == 1 && b.normal == 1){
                    load2(reg_t1, reg_fp, func[f].parnum-b.ref+3);     //func[f].parnum - ref + 1 + 2
                    b1 = reg_t1;
                }else if(b.lev == 1 && b.normal == 0){
                    load2(reg_t1, reg_fp, (-1)*b.ref);
                    b1 = reg_t1;
                }
                store2(reg_t1, reg_sp, 0);
                add_sub_i2(ADDI, reg_sp, -4, reg_sp);
                d++;
                if(d == parnum){
                    break;
                }
            }
        }
        
        
    }
    //    moveto(reg_s0, reg_sp);
    //    moveto(reg_s1, reg_fp);
    store2(reg_sp, reg_sp, 0);
    add_sub_i2(ADDI, reg_sp, -4, reg_sp);
    store2(reg_fp, reg_sp, 0);
    add_sub_i2(ADDI, reg_sp, -4, reg_sp);
    moveto2(reg_fp, reg_sp);
    genJump2(JAL, 0, mcode.a.name);
    //    moveto(reg_sp, reg_s0);
    //    moveto(reg_fp, reg_s1);
    load2(reg_sp, reg_fp, 2);
    load2(reg_fp, reg_fp, 1);
    if(strcmp(mcode.res.name, "null") == 0){
        return;
    }else if(mcode.res.name[0] == '$'){
        res1 = (mcode.res.adr+func[f].last)*(-1);
    }else if(mcode.res.kind == 3){
        flag = 1;
        if(mcode.res.lev == 0){
            //            printf("\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load2(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t4, reg_t4, 2);
                add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t4, mcode.res.arr);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }
            }
        }else{
            int n = mcode.res.ref*4;
            moveto2(reg_t5, reg_fp);
            add_sub_i2(ADDI, reg_t5, -1*n, reg_t5);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load2(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t4, reg_t4, 2);
                add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t4, mcode.res.arr);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }
            }
        }
    }else if(mcode.res.lev == 0){
        flag = -1;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 1){
        //func[f].parnum - ref + 1 + 2
        res1 = func[f].parnum-mcode.res.ref+3;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 0){
        res1 = (-1)*mcode.res.ref;
    }
    if(flag == 1){
        store2(reg_v1, res1, 0);
    }else if(flag != -1){
        store2(reg_v1, reg_fp, res1);
    }else{
        storegp2(reg_v1, mcode.res.name);
    }
}

void genreturn2(struct middlecode mcode, int f){
    int res1;
    int flag = 0;
    if(strcmp(mcode.res.name, "null") == 0){
        load2(reg_ra, reg_fp, 0);
        genJump2(JR, reg_ra, "");
        return;
    }else if(mcode.res.name[0] == '$'){
        res1 = (mcode.res.adr+func[f].last)*(-1);
    }else if(mcode.res.kind == 4){
        if(mcode.res.type == 0){
            int n = toDigit(mcode.res.name);
            //            printf("\tli %s, %d\n", mipsReg[reg_t7], n);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t7], n);
            moveto2(reg_v1, reg_t7);
        }else{
            int c = mcode.res.name[1];
            //            printf("\tli %s, %d\n", mipsReg[reg_t7], c);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t7], c);
            moveto2(reg_v1, reg_t7);
        }
        load2(reg_ra, reg_fp, 0);
        genJump2(JR, reg_ra, "");
        return;
    }else if(mcode.res.kind == 3){
        flag = 1;
        if(mcode.res.lev == 0){
            //            printf("\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load2(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t4, reg_t4, 2);
                add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t4, mcode.res.arr);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }
            }
        }else{
            int n = mcode.res.ref*4;
            moveto2(reg_t5, reg_fp);
            add_sub_i2(ADDI, reg_t5, -1*n, reg_t5);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load2(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t4, reg_t4, 2);
                add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t4, mcode.res.arr);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }
            }
        }
    }else if(mcode.res.lev == 0){
        flag = -1;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 1){
        //func[f].parnum - ref + 1 + 2
        res1 = func[f].parnum-mcode.res.ref+3;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 0){
        res1 = (-1)*mcode.res.ref;
    }
    if(flag == 1){
        load2(reg_v1, res1, 0);
    }else if(flag != -1){
        load2(reg_v1, reg_fp, res1);
    }else{
        loadgp2(reg_v1, mcode.res.name);
    }
    load2(reg_ra, reg_fp, 0);
    genJump2(JR, reg_ra, "");
}

void genscanf2(struct middlecode mcode, int f){
    int res1;
    int i;
    int flag = 0;
    int flag2 = 0;
    for(i = 0; i < orderNum; i++){
        if(ifequal(mcode.res, order[i]) == 1 && order[i].reg != 0){
            flag2 = 1;
            break;
        }
    }
    if(strcmp(mcode.res.name, "null") == 0){
        
    }else if(mcode.res.type == 0){
        //        printf("\tli %s, %d\n", mipsReg[reg_v0], 5);
        fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_v0], 5);
        //        printf("\tsyscall\n");
        fprintf(fp2, "\tsyscall\n");
    }else if(mcode.res.type == 1){
        //        printf("\tli %s, %d\n", mipsReg[reg_v0], 12);
        fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_v0], 12);
        //        printf("\tsyscall\n");
        fprintf(fp2, "\tsyscall\n");
    }
    
    if(flag2 == 1 && mcode.res.kind != 3 && mcode.res.normal != 1){
        
    }else if(mcode.res.name[0] == '$'){
        res1 = (mcode.res.adr+func[f].last)*(-1);
    }else if(mcode.res.kind == 3){
        flag = 1;
        if(mcode.res.lev == 0){
            //            printf("\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_t5], mcode.res.name);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load2(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t4, reg_t4, 2);
                add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t4, mcode.res.arr);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(ADD, reg_t4, reg_t5, reg_t4);
                    res1 = reg_t4;
                }
            }
        }else{
            int n = mcode.res.ref*4;
            moveto2(reg_t5, reg_fp);
            add_sub_i2(ADDI, reg_t5, -1*n, reg_t5);
            if(mcode.res.arr[0] == '$'){
                int i = toDigit(mcode.res.arr);
                load2(reg_t4, reg_fp, (i+func[f].last)*(-1));
                sll_srl2(SLL, reg_t4, reg_t4, 2);
                add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                res1 = reg_t4;
            }else{
                int k = locSym(mcode.res.arr, f);
                if(tab[k].lev == 0){
                    loadgp2(reg_t4, mcode.res.arr);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 1){
                    load2(reg_t4, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }else if(tab[k].lev == 1 && tab[k].normal == 0){
                    load2(reg_t4, reg_fp, (-1)*tab[k].ref);
                    sll_srl2(SLL, reg_t4, reg_t4, 2);
                    add_sub2(SUB, reg_t5, reg_t4, reg_t4);
                    res1 = reg_t4;
                }
            }
        }
    }else if(mcode.res.lev == 0){
        flag = -1;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 1){
        //func[f].parnum - ref + 1 + 2
        res1 = func[f].parnum-mcode.res.ref+3;
    }else if(mcode.res.lev == 1 && mcode.res.normal == 0){
        res1 = (-1)*mcode.res.ref;
    }
    if(flag2 == 1 && mcode.res.kind != 3 && mcode.res.normal != 1){
        moveto2(order[i].reg, reg_v0);
        flag2 = 0;
    }else if(flag == 1){
        store2(reg_v0, res1, 0);
    }else if(flag != -1){
        store2(reg_v0, reg_fp, res1);
    }else{
        storegp2(reg_v0, mcode.res.name);
    }
}

void genprintf2(struct middlecode mcode, int f){
    int b1;
    int i;
    int flag = 0;
    if(strcmp(mcode.a.name, "null") != 0 && strcmp(mcode.a.name, "") != 0){
        //        printf("\tli %s, %d\n", mipsReg[reg_v0], 4);
        fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_v0], 4);
        //        printf("\tla %s, %s\n", mipsReg[reg_a0], mcode.a.name);
        fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_a0], mcode.a.name);
        //        printf("\tsyscall\n");
        fprintf(fp2, "\tsyscall\n");
    }
    if(strcmp(mcode.b.name, "null") != 0 && strcmp(mcode.b.name, "") != 0){
        for(i = 0; i < orderNum; i++){
            if(ifequal(mcode.b, order[i]) == 1 && order[i].reg != 0){
                flag = 1;
                break;
            }
        }
        if(flag == 1 && mcode.b.kind != 3 && mcode.b.normal != 1){
            moveto2(reg_t2, order[i].reg);
            b1 = reg_t2;
            flag = 0;
        }else if(mcode.b.name[0] == '$'){
            load2(reg_t2, reg_fp, (mcode.b.adr+func[f].last)*(-1));
            b1 = reg_t2;
        }else if(mcode.b.kind == 4){
            if(mcode.b.type == 0){
                int n = toDigit(mcode.b.name);
                //                printf("\tli %s, %d\n", mipsReg[reg_t2], n);
                fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t2], n);
                b1 = reg_t2;
            }else{
                int c = mcode.b.name[1];
                //                printf("\tli %s, %d\n", mipsReg[reg_t2], c);
                fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_t2], c);
                b1 = reg_t2;
            }
        }else if(mcode.b.kind == 3){
            if(mcode.b.lev == 0){
                //                printf("\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
                fprintf(fp2, "\tla %s, %s\n", mipsReg[reg_t3], mcode.b.name);
                if(mcode.b.arr[0] == '$'){
                    int i = toDigit(mcode.b.arr);
                    load2(reg_t2, reg_fp, (i+func[f].last)*(-1));
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(locSym(mcode.b.arr, f) == 0){
                    int n = toDigit(mcode.b.arr);
                    load2(reg_t2, reg_t3, n);
                    b1 = reg_t2;
                }else{
                    int k = locSym(mcode.b.arr, f);
                    if(tab[k].lev == 0){
                        loadgp2(reg_t2, mcode.b.arr);
                        sll_srl2(SLL, reg_t2, reg_t2, 2);
                        add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                        load2(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }else if(tab[k].lev == 1 && tab[k].normal == 1){
                        load2(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                        sll_srl2(SLL, reg_t2, reg_t2, 2);
                        add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                        load2(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }else if(tab[k].lev == 1 && tab[k].normal == 0){
                        load2(reg_t2, reg_fp, (-1)*tab[k].ref);
                        sll_srl2(SLL, reg_t2, reg_t2, 2);
                        add_sub2(ADD, reg_t3, reg_t2, reg_t3);
                        load2(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }
                }
            }else{
                int n = mcode.b.ref*4;
                moveto2(reg_t3, reg_fp);
                add_sub_i2(ADDI, reg_t3, -1*n, reg_t3);
                if(mcode.b.arr[0] == '$'){
                    int i = toDigit(mcode.b.arr);
                    load2(reg_t2, reg_fp, (i+func[f].last)*(-1));
                    sll_srl2(SLL, reg_t2, reg_t2, 2);
                    add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                    load2(reg_t2, reg_t3, 0);
                    b1 = reg_t2;
                }else if(locSym(mcode.b.arr, f) == 0){
                    int n = toDigit(mcode.b.arr);
                    load2(reg_t2, reg_t3, -1*n);
                    b1 = reg_t2;
                }else{
                    int k = locSym(mcode.b.arr, f);
                    if(tab[k].lev == 0){
                        loadgp2(reg_t2, mcode.b.arr);
                        sll_srl2(SLL, reg_t2, reg_t2, 2);
                        add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                        load2(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }else if(tab[k].lev == 1 && tab[k].normal == 1){
                        load2(reg_t2, reg_fp, func[f].parnum-tab[k].ref+3);     //func[f].parnum - ref + 1 + 2
                        sll_srl2(SLL, reg_t2, reg_t2, 2);
                        add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                        load2(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }else if(tab[k].lev == 1 && tab[k].normal == 0){
                        load2(reg_t2, reg_fp, (-1)*tab[k].ref);
                        sll_srl2(SLL, reg_t2, reg_t2, 2);
                        add_sub2(SUB, reg_t3, reg_t2, reg_t3);
                        load2(reg_t2, reg_t3, 0);
                        b1 = reg_t2;
                    }
                }
            }
        }else if(mcode.b.lev == 0){
            loadgp2(reg_t2, mcode.b.name);
            b1 = reg_t2;
        }else if(mcode.b.lev == 1 && mcode.b.normal == 1){
            load2(reg_t2, reg_fp, func[f].parnum-mcode.b.ref+3);     //func[f].parnum - ref + 1 + 2
            b1 = reg_t2;
        }else if(mcode.b.lev == 1 && mcode.b.normal == 0){
            load2(reg_t2, reg_fp, (-1)*mcode.b.ref);
            b1 = reg_t2;
        }
        if(mcode.b.type == 0){
            //            printf("\tli %s, %d\n", mipsReg[reg_v0], 1);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_v0], 1);
            moveto2(reg_a0, b1);
            //            printf("\tsyscall\n");
            fprintf(fp2, "\tsyscall\n");
        }else if(mcode.b.type == 1){
            //            printf("\tli %s, %d\n", mipsReg[reg_v0], 11);
            fprintf(fp2, "\tli %s, %d\n", mipsReg[reg_v0], 11);
            moveto2(reg_a0, b1);
            //            printf("\tsyscall\n");
            fprintf(fp2, "\tsyscall\n");
        }
    }
}

void getMips2(){
    dag();
    color();
    fp2 = fopen("/Users/huziyi/code/insymbol/insymbol/result2.txt", "w");
    m_emitNum = 0;
    data2();
    //    printf(".text\n");
    fputs(".text\n", fp2);
    moveto2(reg_fp, reg_sp);
    genJump2(JAL, 0, "main");
    //    printf("\tli, %s, %d\n", mipsReg[reg_v0], 10);
    fprintf(fp2,"\tli, %s, %d\n", mipsReg[reg_v0], 10);
    //    printf("\tsyscall\n");
    fprintf(fp2,"\tsyscall\n");
    while(strcmp(code[m_emitNum].op, "start") == 0){
        int fn = locSym(code[m_emitNum].res.name, -1);
        int fref = tab[fn].adr;
        //        printf("%s:\n", code[m_emitNum].res.name);
        fprintf(fp2,"%s:\n", code[m_emitNum].res.name);
        store2(reg_ra, reg_sp, 0);
        add_sub_i2(ADDI, reg_sp, -4, reg_sp);
        add_sub_i2(ADDI, reg_sp, -1*(func[tab[fn].adr].size + 4 * func[tab[fn].adr].regnum), reg_sp);
        m_emitNum++;
        while(strcmp(code[m_emitNum].op, "end") != 0){
            if(strcmp(code[m_emitNum].op, "int") == 0 || strcmp(code[m_emitNum].op, "char") == 0){
                
            }else if(strcmp(code[m_emitNum].op, "const") == 0){
                genconst2(code[m_emitNum]);
            }else if(strcmp(code[m_emitNum].op, ">=") == 0 || strcmp(code[m_emitNum].op, ">") == 0 || strcmp(code[m_emitNum].op, "<=") == 0 || strcmp(code[m_emitNum].op, "<") == 0 || strcmp(code[m_emitNum].op, "==") == 0 || strcmp(code[m_emitNum].op, "!=") == 0){
                compare2(code[m_emitNum], 0, 0, fref);
            }else if(strcmp(code[m_emitNum].op, "setlabel") == 0){
                //                printf("%s:\n", code[m_emitNum].res.name);
                fprintf(fp2,"%s:\n", code[m_emitNum].res.name);
            }else if(strcmp(code[m_emitNum].op, "jmp") == 0){
                genJump2(J, 0, code[m_emitNum].res.name);
            }else if(strcmp(code[m_emitNum].op, "+") == 0 || strcmp(code[m_emitNum].op, "-") == 0 || strcmp(code[m_emitNum].op, "*") == 0 || strcmp(code[m_emitNum].op, "/") == 0 || strcmp(code[m_emitNum].op, "=") == 0 || strcmp(code[m_emitNum].op, "neq") == 0){
                calculate2(code[m_emitNum], fref);
            }else if(strcmp(code[m_emitNum].op, "call") == 0){
                call2(code[m_emitNum], fref);
            }else if(strcmp(code[m_emitNum].op, "return") == 0){
                int ff = locSym(code[m_emitNum+1].res.name, -1);
                int fff = tab[ff].adr;
                genreturn2(code[m_emitNum], fref);
            }else if(strcmp(code[m_emitNum].op, "scanf") == 0){
                genscanf2(code[m_emitNum], fref);
            }else if(strcmp(code[m_emitNum].op, "printf") == 0){
                genprintf2(code[m_emitNum], fref);
            }
            m_emitNum++;
        }
        if(func[code[m_emitNum].res.adr].flag == 0){
            load2(reg_ra, reg_fp, 0);
            genJump2(JR, reg_ra, "");
        }
        m_emitNum++;
    }
}
