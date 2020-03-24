//
//  Header.h
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#ifndef Header_h
#define Header_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *key[13] = {"const", "char", "int", "void", "if", "else", "while", "return", "switch", "case", "default", "printf", "scanf"};
char *key_sym[13] = {"CONSTSY", "CHARSY", "INTSY", "VOIDSY", "IFSY", "ELSESY", "WHILESY", "RETURNSY", "SWITCHSY", "CASESY", "DEFAULTSY", "PRINTFSY", "SCANFSY"};
enum SYMBOL {CONSTSY, CHARSY, INTSY, VOIDSY, IFSY, ELSESY, WHILESY, RETURNSY, SWITCHSY, CASESY, DEFAULTSY, PRINTFSY, SCANFSY, IDENSY, NUMSY, OPSY, LPARENTSY, RPARENTSY, LBRACKETSY, RBRACKETSY, LBRACESY, RBRACESY, COMMASY, SEMISY, DOTSY, COLONSY, SQUOTESY, DQUOTESY, STRINGSY, CHAR, ADDOPSY, MULOPSY, SUBOPSY, DIVOPSY, EQUOPSY, MAINSY, BEQSY, BNESY, BLESY, BLTSY, BGESY, BGTSY};
char mipsIns[60][10] = {"add", "addi", "addiu", "addu", "sub", "subu","and", "andi", "nor", "or", "ori", "xor", "xori","sll", "sllv", "sra", "srav", "srl", "srlv","div", "divu", "mult", "multu", "mfhi", "mflo","slt", "slti", "sltiu", "sltu","j", "jal", "jalr", "jr","beq", "bgez", "bgtz", "blez", "bltz", "bne","lb", "lbu", "lh", "lhu", "lui", "lw","sb", "sh", "sw","syscall", "nop","li", "blt", "ble", "bgt", "bge", "beqz", "bnez", "mul"};
char mipsReg[32][10] = {"$zero", "$at", "$v0", "$v1","$a0", "$a1", "$a2", "$a3","$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7","$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7","$t8", "$t9","$k0", "$k1","$gp", "$sp", "$fp", "$ra"};
enum Reg{reg_zero, reg_at,reg_v0, reg_v1,reg_a0, reg_a1, reg_a2, reg_a3,reg_t0, reg_t1, reg_t2, reg_t3, reg_t4, reg_t5, reg_t6, reg_t7,reg_s0, reg_s1, reg_s2, reg_s3, reg_s4, reg_s5, reg_s6, reg_s7,reg_t8, reg_t9,reg_k0, reg_k1,reg_gp, reg_sp, reg_fp, reg_ra};
enum Ins{ADD, ADDI, ADDIU, ADDU, SUB, SUBU,AND, ANDI, NOR, OR, ORI, XOR, XORI,SLL, SLLV, SRA, SRAV, SRL, SRLV,DIV, DIVU, MULT, MULTU, MFHI, MFLO,SLT,SLTI,SLTIU,SLTU,J, JAL, JALR, JR,BEQ, BGEZ, BGTZ, BLEZ, BLTZ, BNE,LB, LBU, LH, LHU, LUI, LW,SB, SH, SW,SYSCALL, NOP,LI, BLT, BLE, BGT, BGE, BEQZ, BNEZ, MUL};
char ch;        //字符
int no = 1;     //单词个数i
int line = 1;       //行数
enum SYMBOL sym;        //当前单词标识符
FILE *file;
int retFlag;
char id[100];               //当前单词
int number;            //当前数字
char cnum[100];         //当前数字的字符串形式
char string[1000];          //当前字符串
char cha[1000];               //当前字符
struct table{           //标识符表
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
struct ifRetFunc{           //函数表
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
struct arr{           //数组表
    int high;
    int size;
    int type;
    char arrname[100];
    char ref[1000];
};
struct middlecode{
    char op[10];
    struct table a;
    struct table b;
    struct table res;
};
struct middlecode code[10000];
char stab[1000][1000];           //字符串表
struct ifRetFunc func[1000];             //函数表
struct table tab[10000];
struct arr atab[1000];
int funcNum = 0;            //函数个数
int tab_num = 1;            //符号个数
int arrNum = 0;            //数组个数
int strNum = 0;            //字符串个数
int emitNum = 0;            //四元式个数
int regNum = 1;             //临时变量个数
int tagNum = 0;             //label个数
int m_emitNum = 0;
struct table p;
int map[10000][10000];
struct table actarr[10000];
int actNum = 0;
struct table actarr2[10000];
//函数声明
void error(int n);
int isLetter(char c);
int toDigit(char str[1000]);
int isDigit(char c);
char toLower(char c);
int isStr(char c);
void insymbol();
enum SYMBOL constDec(int lev);
void program();
void constDef(enum SYMBOL per, int lev);
void varDef(enum SYMBOL temp, int lev);
void defHead();
void paramList();
void compoundstatement();
void statementList();
void statement();
void expression();
void factor();
void term();
void ifstatement();
void require();
void whilestatement();
void switchstatement();
struct table casestatement(struct table label1, struct table label2, struct table label3, struct table place2, struct table place3);
void defaultstatement(struct table label1);
int vParamList(int i);
void printfstatement();
void scanfstatement();
void returnstatement();
int locSym(char* id, int fn);
int enter(char* name, int kind, int type, int adr, int normal, int size, int lev);
struct table newreg(int kind, int type, int adr);
struct table newtag();
void emit(char op[], struct table a, struct table b, struct table res);
struct table equal(struct table a, struct table b);
int ifequal(struct table a, struct table b);
void printcode();
struct table init(struct table a);
int clear();
void getMips();
void data();
void genJump(int ins, int rs, char *label);
void store(int a, int st, int base);
void load(int a, int ld, int base);
void loadgp(int a, char gp[1000]);
void storegp(int a, char gp[1000]);
void genconst(struct middlecode mcode);
void add_sub(int op, int a, int b, int res);
void add_sub_i(int op, int a, int imm, int res);
void mul_div(int op, int a, int b, int res);
void sll_srl(int op, int a, int b, int imm);
void moveto(int res, int a);
void compare(struct middlecode code, int a, int b, int f);
void condition_jump(int op, int a, int b, char lab[1000]);
void calculate(struct middlecode mcode, int f);
void call(struct middlecode mcode, int f);
void genreturn(struct middlecode mcode, int f);
void genscanf(struct middlecode mcode, int f);
void genprintf(struct middlecode mcode, int f);
void dag();
void activity();
void color();
struct table order[1000];
int orderNum = 0;
#endif /* Header_h */
