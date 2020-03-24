//
//  insymbol.c
//  lrparser
//
//  Created by 胡梓义 on 2017/12/15.
//  Copyright © 2017年 胡梓义. All rights reserved.
//

#include "insymbol.h"
int isLetter(char c){
    if(((c<='z')&&(c>='a')) || ((c<='Z')&&(c>='A')) || (c == '_')){
        return 1;
    }
    return 0;
}

int toDigit(char str[1000]){
    int j = 0;
    number = 0;
    int flag = 1;
    if(str[j] == '-'){
        flag = -1;
        j++;
    }
    if(str[j] == '$'){
        j++;
    }
    while(str[j] != '\0'){
        number = number * 10 + (str[j] - '0');
        j++;
    }
    return flag*number;
}

int isDigit(char c){
    if((c>='0')&&(c<='9')){
        return 1;
    }
    return 0;
}

char toLower(char c){
    if((c<='Z')&&(c>='A')){
        return (c + 32);
    }
    return c;
}

int isStr(char c){
    if((c == 32) || (c == 33) || ((c>34) && (c<127))){
        return 1;
    }
    return 0;
}

void insymbol(){
    char str[100]="";
    int flag;
    int i = 0;
    int k;
    while((ch = fgetc(file)) != EOF){
        flag = 0;
        i = 0;
        memset(str,0,sizeof(str));
        if(ch == ' ' || ch == '\t' || ch == '\n'){
            if(ch == '\n'){
                line++;
            }
        }else if(isLetter(ch)){
            while(isLetter(ch) || isDigit(ch)){
                str[i++] = toLower(ch);
                ch = fgetc(file);
            }
            fseek(file,-1L,SEEK_CUR);
            for(k = 0; k < 13; k++){
                if(strcmp(str, key[k]) == 0){
                    flag = 1;
                    //                    printf("%d %s %s\n", no++, key_sym[k], str);
                    sym = k;
                    return;
                }
            }
            if(flag == 0){
                strcpy(id, str);
                if(strcmp(str, "main") == 0){
                    sym = MAINSY;
                }else{
                    sym = IDENSY;
                }
                //                printf("%d IDEN %s\n", no++, str);
                return;
            }
        }else if(isDigit(ch)){
            while(isDigit(ch)){
                str[i++] = ch;
                ch = fgetc(file);
            }
            int j = 0;
            number = 0;
            while(j < i){
                number = number * 10 + (str[j] - '0');
                j++;
            }
            if(isLetter(ch)){
                error(0);
                while(isLetter(ch) || isDigit(ch)){
                    ch = fgetc(file);
                }
                ch = fgetc(file);
            }
            fseek(file,-1L,SEEK_CUR);
            sym = NUMSY;
            strcpy(cnum, str);
            //            printf("%d NUM %s\n", no++, str);
            return;
        }else{
            switch (ch) {
                case '+' :
                    sym = ADDOPSY;
                    //                    printf("%d OP %c\n", no++, ch);
                    break;
                case '-' :
                    sym = SUBOPSY;
                    //                    printf("%d OP %c\n", no++, ch);
                    break;
                case '*' :
                    sym = MULOPSY;
                    //                    printf("%d OP %c\n", no++, ch);
                    break;
                case '/' :
                    sym = DIVOPSY;
                    //                    printf("%d OP %c\n", no++, ch);
                    break;
                case '=' :
                    ch = fgetc(file);
                    if(ch == '='){
                        sym = BEQSY;
                        //                        printf("%d OP ==\n", no++);
                    }else{
                        sym = EQUOPSY;
                        //                        printf("%d OP =\n", no++);
                        fseek(file,-1L,SEEK_CUR);
                    }
                    break;
                case '(' :
                    sym = LPARENTSY;
                    //                    printf("%d LPARENT %c\n", no++, ch);
                    break;
                case ')' :
                    sym = RPARENTSY;
                    //                    printf("%d RPARENT %c\n", no++, ch);
                    break;
                case '[' :
                    sym = LBRACKETSY;
                    //                    printf("%d LBRACKET %c\n", no++, ch);
                    break;
                case ']' :
                    sym = RBRACKETSY;
                    //                    printf("%d RBRACKET %c\n", no++, ch);
                    break;
                case '{' :
                    sym = LBRACESY;
                    //                    printf("%d LBRACE %c\n", no++, ch);
                    break;
                case '}' :
                    sym = RBRACESY;
                    //                    printf("%d RBRACE %c\n", no++, ch);
                    break;
                case ',' :
                    sym = COMMASY;
                    //                    printf("%d COMMA %c\n", no++, ch);
                    break;
                case ';' :
                    sym = SEMISY;
                    //                    printf("%d SEMI %c\n", no++, ch);
                    break;
                case '.' :
                    sym = DOTSY;
                    //                    printf("%d DOT %c\n", no++, ch);
                    break;
                case ':' :
                    sym = COLONSY;
                    //                    printf("%d COLON %c\n", no++, ch);
                    break;
                case '!' :
                    ch = fgetc(file);
                    if(ch == '='){
                        sym = BNESY;
                        //                        printf("%d OP !=\n", no++);
                    }else{
                        error(12);
                        fseek(file,-1L,SEEK_CUR);
                    }
                    break;
                case '>' :
                    ch = fgetc(file);
                    if(ch == '='){
                        sym = BGESY;
                        //                        printf("%d OP >=\n", no++);
                    }else{
                        sym = BGTSY;
                        //                        printf("%dOP >\n", no++);
                        fseek(file,-1L,SEEK_CUR);
                    }
                    break;
                case '<' :
                    ch = fgetc(file);
                    if(ch == '='){
                        sym = BLESY;
                        //                        printf("%d OP <=\n", no++);
                    }else{
                        sym = BLTSY;
                        //                        printf("%d OP <\n", no++);
                        fseek(file,-1L,SEEK_CUR);
                    }
                    break;
                case '\'' :
                    str[i++] = ch;
                    ch = fgetc(file);
                    if((ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || isDigit(ch) || isLetter(ch)){
                        str[i++] = ch;
                        ch = fgetc(file);
                        if(ch != '\''){
                            error(3);
                            fseek(file,-1L,SEEK_CUR);
                        }else{
                            str[i++] = ch;
                            sym = CHAR;
                            strcpy(cha, str);
                            //                            printf("%d SQUOTE '\n", no++);
                            //                            printf("%d CHAR %s\n", no++, str);
                        }
                    }else{
                        error(1);
                        ch = fgetc(file);
                        if(ch != '\''){
                            error(3);
                            fseek(file,-1L,SEEK_CUR);
                        }
                    }
                    break;
                case '\"' :
                    str[i++] = ch;
                    ch = fgetc(file);
                    if(isStr(ch)){
                        while(isStr(ch)){
                            str[i++] = ch;
                            ch = fgetc(file);
                        }
                        if(ch != '\"'){
                            error(4);
                            fseek(file,-1L,SEEK_CUR);
                        }else{
                            str[i++] = ch;
                            sym = STRINGSY;
                            strcpy(string, str);
                            //                            printf("%d DQUOTE %c\n", no++, ch);
                            //                            printf("%d STRING %s\n", no++, str);
                        }
                    }else{
                        error(2);
                        while(ch != '\"'){
                            if(ch != EOF){
                                ch = fgetc(file);
                            }else{
                                error(4);
                                exit(0);
                            }
                        }
                        //                        printf("%d DQUOTE %c\n", no++, ch);
                    }
                    break;
                default:
                    error(5);
                    break;
            }
            return;
        }
    }
}
