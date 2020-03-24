//
//  insymbol.h
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#ifndef insymbol_h
#define insymbol_h

#include <stdio.h>

extern char *key[13];
extern char *key_sym[13];
extern enum SYMBOL {CONSTSY, CHARSY, INTSY, VOIDSY, IFSY, ELSESY, WHILESY, RETURNSY, SWITCHSY, CASESY, DEFAULTSY, PRINTFSY, SCANFSY, IDENSY, NUMSY, OPSY, LPARENTSY, RPARENTSY, LBRACKETSY, RBRACKETSY, LBRACESY, RBRACESY, COMMASY, SEMISY, DOTSY, COLONSY, SQUOTESY, DQUOTESY, STRINGSY, CHAR, ADDOPSY, MULOPSY, SUBOPSY, DIVOPSY, EQUOPSY, MAINSY, BEQSY, BNESY, BLESY, BLTSY, BGESY, BGTSY};
extern char ch;        //字符
extern int no;     //单词个数i
extern int line;       //行数
extern enum SYMBOL sym;        //当前单词标识符
extern FILE *file;
extern char id[100];               //当前单词
extern int number;            //当前数字
extern char cnum[100];         //当前数字的字符串形式
extern char string[1000];          //当前字符串
extern char cha[1000];               //当前字符
extern int isLetter(char c);
extern int toDigit(char str[1000]);
extern int isDigit(char c);
extern char toLower(char c);
extern int isStr(char c);
extern void insymbol();


#endif /* insymbol_h */
