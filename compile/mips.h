//
//  mips.h
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#ifndef mips_h
#define mips_h

#include <stdio.h>
extern char mipsIns[60][10];
extern char mipsReg[32][10];
extern enum Reg{reg_zero, reg_at,reg_v0, reg_v1,reg_a0, reg_a1, reg_a2, reg_a3,reg_t0, reg_t1, reg_t2, reg_t3, reg_t4, reg_t5, reg_t6, reg_t7,reg_s0, reg_s1, reg_s2, reg_s3, reg_s4, reg_s5, reg_s6, reg_s7,reg_t8, reg_t9,reg_k0, reg_k1,reg_gp, reg_sp, reg_fp, reg_ra};
extern enum Ins{ADD, ADDI, ADDIU, ADDU, SUB, SUBU,AND, ANDI, NOR, OR, ORI, XOR, XORI,SLL, SLLV, SRA, SRAV, SRL, SRLV,DIV, DIVU, MULT, MULTU, MFHI, MFLO,SLT,SLTI,SLTIU,SLTU,J, JAL, JALR, JR,BEQ, BGEZ, BGTZ, BLEZ, BLTZ, BNE,LB, LBU, LH, LHU, LUI, LW,SB, SH, SW,SYSCALL, NOP,LI, BLT, BLE, BGT, BGE, BEQZ, BNEZ, MUL};
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
extern struct table p;
extern char id[100];
extern struct table newreg(int kind, int type, int adr);
extern struct table newtag();
extern void emit(char op[], struct table a, struct table b, struct table res);
extern struct table equal(struct table a, struct table b);
extern int ifequal(struct table a, struct table b);
extern struct table init(struct table a);
extern int locSym(char* id, int fn);
extern void getMips();
extern void data();
extern void genJump(int ins, int rs, char *label);
extern void store(int a, int st, int base);
extern void load(int a, int ld, int base);
extern void loadgp(int a, char gp[1000]);
extern void storegp(int a, char gp[1000]);
extern void genconst(struct middlecode mcode);
extern void add_sub(int op, int a, int b, int res);
extern void add_sub_i(int op, int a, int imm, int res);
extern void mul_div(int op, int a, int b, int res);
extern void sll_srl(int op, int a, int b, int imm);
extern void moveto(int res, int a);
extern void compare(struct middlecode code, int a, int b, int f);
extern void condition_jump(int op, int a, int b, char lab[1000]);
extern void calculate(struct middlecode mcode, int f);
extern void call(struct middlecode mcode, int f);
extern void genreturn(struct middlecode mcode, int f);
extern void genscanf(struct middlecode mcode, int f);
extern void genprintf(struct middlecode mcode, int f);
extern void printcode();
extern void dag();
extern void color();
extern void activity();
extern struct table order[1000];
extern int orderNum;
#endif /* mips_h */
