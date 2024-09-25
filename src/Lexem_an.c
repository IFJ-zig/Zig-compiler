/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#include "tokens.h"

int Lexem_analyzer(List *L){
    //inicializace
    char *lexem, *p;
    int *n;
    double *d;
    //pokud nealokuje, vrátí chybu
    if (!(lexem = (char*)malloc(sizeof(char)*256))){return 99;}
    char letter = getchar();
    lexem[0] = '\0';

    //tělo programu
    while(letter != EOF){
        switch (letter){
        //bílé znaky
        case ' ':
            letter = getchar();
            break;
        case '\t':
            letter = getchar();
            break;
        case ';':
        case '\n':
            LInsertLast(L, next, NULL, NULL, NULL);
            letter = getchar();
            break;
        //jednoznačné tokeny
        case ':':
            LInsertLast(L, dvojt, NULL, NULL, NULL);
            letter = getchar();
            break;
        case '(':
            LInsertLast(L, lzavor, NULL, NULL, NULL);
            letter = getchar();
            break;
        case ')':
            LInsertLast(L, pzavor, NULL, NULL, NULL);
            letter = getchar();
            break;
        case '{':
            LInsertLast(L, lblok, NULL, NULL, NULL);
            letter = getchar();
            break;
        case '}':
            LInsertLast(L, pblok, NULL, NULL, NULL);
            letter = getchar();
            break;
        case ',':
            LInsertLast(L, line, NULL, NULL, NULL);
            letter = getchar();
            break;
        case '+':
            LInsertLast(L, plus, NULL, NULL, NULL);
            letter = getchar();
            break;
        case '-':
            letter = getchar();
            if (letter == '>'){
                LInsertLast(L, rettyp, NULL, NULL, NULL);
                letter = getchar();
            } else {
                LInsertLast(L, minus, NULL, NULL, NULL);
            }
            break;
        case '*':
            LInsertLast(L, krat, NULL, NULL, NULL);
            letter = getchar();
            break;
        case '<':
            letter = getchar();
            if(letter == '='){
                LInsertLast(L, lequal, NULL, NULL, NULL);
                letter = getchar();
            } else {
                LInsertLast(L, less, NULL, NULL, NULL);
            }
            break;
        case '>':
            letter = getchar();
            if(letter == '='){
                LInsertLast(L, mequal, NULL, NULL, NULL);
                letter = getchar();
            } else {
                LInsertLast(L, more, NULL, NULL, NULL);
            }
            break;
        case '!':
            letter = getchar();
            if (letter == '='){
                LInsertLast(L, nequal, NULL, NULL, NULL);
                letter = getchar();
            } else {
                LInsertLast(L, nonnil, NULL, NULL, NULL);
            }
            break;
        case '=':
            letter = getchar();
            if (letter != '='){
                LInsertLast(L, rovno, NULL, NULL, NULL);
            } else {
                LInsertLast(L, equal, NULL, NULL, NULL);
                letter = getchar();
            }
            break;
        case '?':
            letter = getchar();
            if (letter == '?'){
                LInsertLast(L, choice, NULL, NULL, NULL);
                letter = getchar();
            } else {
                free(lexem);
                return 1;
            }
            break;
        case '/':
            letter = getchar();
            switch (letter){
                //jednořádkový komentář
                case '/':
                    letter = getchar();
                    while(letter != '\n' && letter != EOF){
                        letter = getchar();
                    }
                    break;
                //víceřádkový komentář
                case '*':
                    letter = getchar();
                    while(letter != EOF){
                        letter = getchar();
                        if (letter == '*'){
                            letter = getchar();
                            if (letter == '/'){break;}
                        }
                    }
                    letter = getchar();
                    break;
                //dělení
                default:
                    LInsertLast(L, deleno, NULL, NULL, NULL);
            }
            break;
        case '"':
            //string
            letter = getchar();
            if (letter == '"'){
                //druhý
                letter = getchar();
                if (letter == '"'){
                    //trojitý
                    if (getchar() == '\n'){
                        letter = getchar();
                        while(letter != EOF){
                            if (letter == '"'){
                                letter = getchar();
                                if (letter == '"'){
                                    letter = getchar();
                                    if (letter == '"'){
                                        break;
                                    } else {
                                        strcat(lexem, "\"\"");
                                    }
                                } else {
                                    strcat(lexem, "\"");
                                }
                            }
                            if (letter > 31 || letter == '\n' || letter == '\t' || letter == '\r'){
                                strncat(lexem, &letter, 1);
                                letter = getchar();
                            }
                        }
                        if (letter == EOF){
                            //chyba lexemu
                            free(lexem);
                            return 1;
                        }
                        if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                        strcpy(p, lexem);
                        LInsertLast(L, text, NULL, NULL, p);
                        lexem[0] = '\0';
                        letter = getchar();
                    } else {
                        //chyba lexemu
                        free(lexem);
                        return 1;
                    }
                } else {
                    //prázdný
                    if (!(p = (char*)malloc(sizeof(char)))){free(lexem);return 99;}
                    *p= '\0';
                    LInsertLast(L, text, NULL, NULL, p);
                }
            } else {
                //jednoduchý
                while(letter != '"' && letter != EOF  && letter != '\n'){
                    //escape sekvence?
                    if(letter == '\\'){
                        int y;
                        char let[4];
                        letter = getchar();
                        switch (letter){
                        case '"':
                            strncat(lexem, &letter, 1);
                            letter = getchar();
                            break;
                        case 'n':
                            letter = '\n';
                            strncat(lexem, &letter, 1);
                            letter = getchar();
                            break;
                        case 't':
                            letter = '\t';
                            strncat(lexem, &letter, 1);
                            letter = getchar();
                            break;
                        case 'r':
                            strncat(lexem, &letter, 1);
                            letter = getchar();
                            break;
                        case '\\':
                            strncat(lexem, &letter, 1);
                            letter = getchar();
                            break;
                        case 'u':
                            //hexadecimální
                            if(getchar() == '{'){
                                letter = getchar();
                                for (int i= 0; i < 8 && letter != '}'; i++){
                                    let[0] = let[1];
                                    let[1] = letter;
                                    letter = getchar();
                                }
                                let[2] = '\0';
                                if(((let[0] >= '0' && let[0] <= '9') || (let[0] >= 'a' && let[0] <= 'f') || (let[0] >= 'A' && let[0] <= 'F')) && 
                                    ((let[1] >= '0' && let[1] <= '9') || (let[1] >= 'a' && let[1] <= 'f') || (let[1] >= 'A' && let[1] <= 'F'))){
                                    sscanf(let, "%X", &y);
                                    letter = y;
                                    strncat(lexem, &letter, 1);
                                    letter = getchar();
                                } else {
                                    //chyba lexemu
                                    free(lexem);
                                    return 1;
                                }
                            } else {
                                //chyba lexemu
                                free(lexem);
                                return 1;
                            }
                            break;
                        default:
                            //chyba lexemu
                            free(lexem);
                            return 1;
                        }
                    } else if (letter > 31){
                        strncat(lexem, &letter, 1);
                        letter = getchar();
                    }
                }
                if (letter == EOF || letter == '\n'){
                    //chyba lexemu
                    free(lexem);
                    return 1;
                }
                if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                strcpy(p, lexem);
                LInsertLast(L, text, NULL, NULL, p);
                lexem[0] = '\0';
                letter = getchar();
            }
            break;
        case '0'...'9':
            //čísla
            while(letter >= '0' && letter <= '9'){
                strncat(lexem, &letter, 1);
                letter = getchar();
            }
            //desetiná
            if(letter == '.'){
                strncat(lexem, &letter, 1);
                letter = getchar();
                if (!(letter >= '0' && letter <= '9')){free(lexem);return 1;}
                while(letter >= '0' && letter <= '9'){
                    strncat(lexem, &letter, 1);
                    letter = getchar();
                }
                //exponent
                if(letter == 'e' || letter == 'E'){
                    strncat(lexem, &letter, 1);
                    letter = getchar();
                    if (letter == '+' || letter == '-'){
                        strncat(lexem, &letter, 1);
                        letter = getchar();
                    }
                    if (!(letter >= '0' && letter <= '9')){free(lexem);return 1;}
                    while(letter >= '0' && letter <= '9'){
                        strncat(lexem, &letter, 1);
                        letter = getchar();
                    }
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    if (!(d = (double*)malloc(sizeof(double)))){free(lexem);return 99;}
                    *d = atof(lexem);
                    LInsertLast(L, decim, NULL, d, p);
                    lexem[0] = '\0';
                } else {
                    //jen tečka
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    if (!(d = (double*)malloc(sizeof(double)))){free(lexem);return 99;}
                    *d = atof(lexem);
                    LInsertLast(L, decim, NULL, d, p);
                    lexem[0] = '\0';
                }
            } else if(letter == 'e' || letter == 'E'){
                //exponent
                strncat(lexem, &letter, 1);
                letter = getchar();
                if (letter == '+' || letter == '-'){
                    strncat(lexem, &letter, 1);
                    letter = getchar();
                }
                if (!(letter >= '0' && letter <= '9')){free(lexem);return 1;}
                while(letter >= '0' && letter <= '9'){
                    strncat(lexem, &letter, 1);
                    letter = getchar();
                }
                if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                strcpy(p, lexem);
                if (!(d = (double*)malloc(sizeof(double)))){free(lexem);return 99;}
                *d = atof(lexem);
                LInsertLast(L, decim, NULL, d, p);
                lexem[0] = '\0';
            } else {
            //celá
                if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                strcpy(p, lexem);
                if (!(n = (int*)malloc(sizeof(int)))){free(lexem);return 99;}
                *n = atoi(lexem);
                LInsertLast(L, num, n, NULL, p);
                lexem[0] = '\0';
            }
            break;
        case 'a'...'z':
        case 'A'...'Z':
        case '_':
            lexem[0] = '\0';
            //samotné podtržítko
            if (letter == '_'){
                letter = getchar();
                if (!(letter >= 'a' && letter <= 'z') && !(letter >= 'A' && letter <= 'Z') && !(letter >= '0' && letter <= '9') && letter != '_'){
                    LInsertLast(L, pass, NULL, NULL, NULL);
                    break;
                } else {
                    strcat(lexem, "_");
                }
            }
            //identifikátory
            while((letter >= '0' && letter <= '9') || (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || letter == '_'){
                strncat(lexem, &letter, 1);
                letter = getchar();
            }
            switch (lexem[0]){
            case 'o':
                if(!strcmp(lexem,"ord")){
                    LInsertLast(L, inord, NULL, NULL, NULL);
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'c':
                if(!strcmp(lexem,"chr")){
                    LInsertLast(L, inchr, NULL, NULL, NULL);
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'v':
                if(!strcmp(lexem,"var")){
                    LInsertLast(L, prom, NULL, NULL, NULL);
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'l':
                if(!strcmp(lexem,"let")){
                    LInsertLast(L, konst, NULL, NULL, NULL);
                } else if(!strcmp(lexem,"length")){
                    LInsertLast(L, inlen, NULL, NULL, NULL);
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'r':
                switch (lexem[4]){
                case 'S':
                    if(!strcmp(lexem,"readString")){
                        LInsertLast(L, inres, NULL, NULL, NULL);
                    } else {
                        //id
                        if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                        strcpy(p, lexem);
                        LInsertLast(L, id, NULL, NULL, p);
                    }
                    break;
                case 'I':
                    if(!strcmp(lexem,"readInt")){
                        LInsertLast(L, inrei, NULL, NULL, NULL);
                    } else {
                        //id
                        if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                        strcpy(p, lexem);
                        LInsertLast(L, id, NULL, NULL, p);
                    }
                    break;
                case 'D':
                    if(!strcmp(lexem,"readDouble")){
                        LInsertLast(L, inred, NULL, NULL, NULL);
                    } else {
                        //id
                        if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                        strcpy(p, lexem);
                        LInsertLast(L, id, NULL, NULL, p);
                    }
                    break;
                case 'r':
                    if(!strcmp(lexem,"return")){
                        LInsertLast(L, back, NULL, NULL, NULL);
                    } else {
                        //id
                        if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                        strcpy(p, lexem);
                        LInsertLast(L, id, NULL, NULL, p);
                    }
                    break;
                default:
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                    break;
                }
                break;
            case 'f':
                if(!strcmp(lexem,"func")){
                    LInsertLast(L, funk, NULL, NULL, NULL);
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'w':
                if(!strcmp(lexem,"write")){
                    LInsertLast(L, inwrt, NULL, NULL, NULL);
                } else if(!strcmp(lexem,"while")){
                    LInsertLast(L, loop, NULL, NULL, NULL);
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'e':
                if(!strcmp(lexem,"else")){
                    LInsertLast(L, jinak, NULL, NULL, NULL);
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'i':
                if(!strcmp(lexem,"if")){
                    LInsertLast(L, kdyz, NULL, NULL, NULL);
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'n':
                if(!strcmp(lexem,"nil")){
                    LInsertLast(L, dtnic, NULL, NULL, NULL);
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'I':
                if(!strcmp(lexem,"Int2Double")){
                    LInsertLast(L, ini2d, NULL, NULL, NULL);
                } else if(!strcmp(lexem,"Int")){
                    if(letter == '?'){
                        LInsertLast(L, dtintn, NULL, NULL, NULL);
                        letter = getchar();
                    } else {
                        LInsertLast(L, dtint, NULL, NULL, NULL);
                    }
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'D':
                if(!strcmp(lexem,"Double2Int")){
                    LInsertLast(L, ind2i, NULL, NULL, NULL);
                } else if(!strcmp(lexem,"Double")){
                    if(letter == '?'){
                        LInsertLast(L, dtdbln, NULL, NULL, NULL);
                        letter = getchar();
                    } else {
                        LInsertLast(L, dtdbl, NULL, NULL, NULL);
                    }
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 's':
                if(!strcmp(lexem,"substring")){
                    LInsertLast(L, inssub, NULL, NULL, NULL);
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            case 'S':
                if(!strcmp(lexem,"String")){
                    if(letter == '?'){
                        LInsertLast(L, dtstrn, NULL, NULL, NULL);
                        letter = getchar();
                    } else {
                        LInsertLast(L, dtstr, NULL, NULL, NULL);
                    }
                } else {
                    //id
                    if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                    strcpy(p, lexem);
                    LInsertLast(L, id, NULL, NULL, p);
                }
                break;
            default:
                //id
                if (!(p = (char*)malloc(sizeof(char)*(1 + strlen(lexem))))){free(lexem);return 99;}
                strcpy(p, lexem);
                LInsertLast(L, id, NULL, NULL, p);
                break;
            }
            lexem[0] = '\0';
            break;
        default:
            //chyba lexemu
            free(lexem);
            return 1;
        }
    }
    //*p, *n, *d neuvolňuji, přešli do tokenu
    LInsertLast(L, end, NULL, NULL, NULL);
    free(lexem);
    return 0;
}
