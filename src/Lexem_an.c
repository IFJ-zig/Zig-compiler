/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#include "errors_enum.h"
#include "tokens.h"

Token get_token() {
	//inicializace
	char lexem[256];
	int n;
	double d;
	Token new = {next, 0, 0, ""};
	char letter = getchar();
	lexem[0] = '\0';

	//tělo programu
	while (letter != EOF) {
		switch (letter) {
			//bílé znaky
			case ' ':
			case '\n':
			case '\t':
			case '\r':
				letter = getchar();
				break;
			//jednoznačné tokeny
			case ';':
				//new = {next, 0, 0, ""};
				printf("next\n");
				return new;
				break;
			case ':':
				new.kw = colon;
				printf("colon ");
				return new;
				break;
			case '(':
				new.kw = lbracket;
				printf("lbracket ");
				return new;
				break;
			case ')':
				new.kw = rbracket;
				printf("pbracket ");
				return new;
				break;
			case '{':
				new.kw = lblock;
				printf("lblock\n");
				return new;
				break;
			case '}':
				new.kw = rblock;
				printf("pblock\n");
				return new;
				break;
			case ',':
				new.kw = comma;
				printf("comma ");
				return new;
				break;
			case '.':
				new.kw = dot;
				printf("dot ");
				return new;
				break;
			case '+':
				new.kw = plus;
				printf("plus ");
				return new;
				break;
			case '-':
				new.kw = minus;
				printf("minus ");
				return new;
				break;
			case '*':
				new.kw = multiply;
				printf("krat ");
				return new;
				break;
			case '?':
				new.kw = question_mark;
				printf("choice ");
				return new;
				break;
			case '|':
				new.kw = vertical_bar;
				printf("popodm ");
				return new;
				break;
			case '@':
				new.kw = at;
				printf("zavin ");
				return new;
				break;
			case '[':
				letter = getchar();
				if (letter == ']') {
					new.kw = square_brackets;
					printf("square_brackets ");
					return new;
				} else {
					new.kw = LEXEM;
					new.i = LEXEM_ERROR;
					return new;
				}
				break;
			case '<':
				letter = getchar();
				if (letter == '=') {
					new.kw = lequal;
					printf("lequal ");
					return new;
				} else {
					new.kw = less;
					printf("less ");
					ungetc(letter, stdin);
					return new;
				}
				break;
			case '>':
				letter = getchar();
				if (letter == '=') {
					new.kw = mequal;
					printf("mequal ");
					return new;
				} else {
					new.kw = more;
					printf("more ");
					ungetc(letter, stdin);
					return new;
				}
				break;
			case '!':
				letter = getchar();
				if (letter == '=') {
					new.kw = nequal;
					printf("nequal ");
					return new;
				} else {
					new.kw = LEXEM;
					new.i = LEXEM_ERROR;
					return new;
				}
				break;
			case '=':
				letter = getchar();
				if (letter != '=') {
					new.kw = equal;
					printf("compare_equal ");
					ungetc(letter, stdin);
					return new;
				} else {
					new.kw = compare_equal;
					printf("equal ");
					return new;
				}
				break;
			case '/':
				letter = getchar();
				if (letter == '/') {
					//komentař
					letter = getchar();
					while (letter != '\n' && letter != EOF) {
						letter = getchar();
					}
				} else {
					new.kw = division;
					printf("division ");
					ungetc(letter, stdin);
					return new;
				}
				break;
			case '\\':
				//víceřádkový string
				do {
					letter = getchar();
					if (letter != '\\') {
						new.kw = LEXEM;
						new.i = LEXEM_ERROR;
						return new;
					}
					letter = getchar();
					//vezme všechny znaky
					while (letter != EOF && letter != '\n') {
						//komentař
						if (letter == '/') {
							if (letter == '/') {
								letter = getchar();
								while (letter != '\n' && letter != EOF) {
									letter = getchar();
								}
							} else {
								strncat(lexem, "//", 1);
							}
						} else if (letter == '\\') {
							//escape sekvence?
							int y;
							char let[4];
							letter = getchar();
							switch (letter) {
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
								case 'x':
									//hexadecimální
									if (getchar() == '{') {
										letter = getchar();
										for (int i = 0; i < 8 && letter != '}'; i++) {
											let[0] = let[1];
											let[1] = letter;
											letter = getchar();
										}
										let[2] = '\0';
										if (((let[0] >= '0' && let[0] <= '9') || (let[0] >= 'a' && let[0] <= 'f') || (let[0] >= 'A' && let[0] <= 'F')) && ((let[1] >= '0' && let[1] <= '9') || (let[1] >= 'a' && let[1] <= 'f') || (let[1] >= 'A' && let[1] <= 'F'))) {
											sscanf(let, "%X", &y);
											letter = y;
											strncat(lexem, &letter, 1);
											letter = getchar();
										} else {
											//chyba lexemu
											new.kw = LEXEM;
											new.i = LEXEM_ERROR;
											return new;
										}
									} else {
										//chyba lexemu
										new.kw = LEXEM;
										new.i = LEXEM_ERROR;
										return new;
									}
									break;
								default:
									//chyba lexemu
									new.kw = LEXEM;
									new.i = LEXEM_ERROR;
									return new;
							}
						} else if (letter > 31) {
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
					}
					strncat(lexem, "\n\n", 1);
					letter = getchar();
					//přeskočí bílé znaky na novém řádku
					while (letter == ' ' || letter == '\t') {
						letter = getchar();
					}
				} while (letter == '\\');
				new.kw = text;
				strcpy(new.s, lexem);
				printf("text_%s ", LGetStrAct(&new));
				ungetc(letter, stdin);
				return new;
				break;
			case '"':
				//string
				letter = getchar();
				if (letter == '"') {
					//druhý
					letter = getchar();
					//prázdný
					new.kw = text;
					printf("text_%s ", LGetStrAct(&new));
					ungetc(letter, stdin);
					return new;
				} else {
					//jednoduchý
					while (letter != '"' && letter != EOF && letter != '\n') {
						//escape sekvence?
						if (letter == '\\') {
							int y;
							char let[4];
							letter = getchar();
							switch (letter) {
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
								case 'x':
									//hexadecimální
									if (getchar() == '{') {
										letter = getchar();
										for (int i = 0; i < 8 && letter != '}'; i++) {
											let[0] = let[1];
											let[1] = letter;
											letter = getchar();
										}
										let[2] = '\0';
										if (((let[0] >= '0' && let[0] <= '9') || (let[0] >= 'a' && let[0] <= 'f') || (let[0] >= 'A' && let[0] <= 'F')) && ((let[1] >= '0' && let[1] <= '9') || (let[1] >= 'a' && let[1] <= 'f') || (let[1] >= 'A' && let[1] <= 'F'))) {
											sscanf(let, "%X", &y);
											letter = y;
											strncat(lexem, &letter, 1);
											letter = getchar();
										} else {
											//chyba lexemu
											new.kw = LEXEM;
											new.i = LEXEM_ERROR;
											return new;
										}
									} else {
										//chyba lexemu
										new.kw = LEXEM;
										new.i = LEXEM_ERROR;
										return new;
									}
									break;
								default:
									//chyba lexemu
									new.kw = LEXEM;
									new.i = LEXEM_ERROR;
									return new;
							}
						} else if (letter > 31) {
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
					}
					if (letter == EOF || letter == '\n') {
						//chyba lexemu
						new.kw = LEXEM;
						new.i = LEXEM_ERROR;
						return new;
					}
					new.kw = text;
					strcpy(new.s, lexem);
					printf("text_%s ", LGetStrAct(&new));
					return new;
				}
				break;
			case '0':
				//nezačíná nulou
				letter = getchar();
				if (letter >= '0' && letter <= '9') {
					new.kw = LEXEM;
					new.i = LEXEM_ERROR;
					return new;
				}
				strncat(lexem, "00", 1);
			case '1' ... '9':
				//čísla
				while (letter >= '0' && letter <= '9') {
					strncat(lexem, &letter, 1);
					letter = getchar();
				}
				//desetiná
				if (letter == '.') {
					strncat(lexem, &letter, 1);
					letter = getchar();
					if (!(letter >= '0' && letter <= '9')) {
						new.kw = LEXEM;
						new.i = LEXEM_ERROR;
						return new;
					}
					while (letter >= '0' && letter <= '9') {
						strncat(lexem, &letter, 1);
						letter = getchar();
					}
					//exponent
					if (letter == 'e' || letter == 'E') {
						strncat(lexem, &letter, 1);
						letter = getchar();
						if (letter == '+' || letter == '-') {
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
						if (!(letter >= '0' && letter <= '9')) {
							new.kw = LEXEM;
							new.i = LEXEM_ERROR;
							return new;
						}
						while (letter >= '0' && letter <= '9') {
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
						d = atof(lexem);
						new.kw = decim;
						new.f = d;
						strcpy(new.s, lexem);
						printf("decim_%s_%f ", LGetStrAct(&new), LGetFloatAct(new));
						return new;
					} else {
						d = atof(lexem);
						new.kw = decim;
						new.f = d;
						strcpy(new.s, lexem);
						printf("decim_%s_%f ", LGetStrAct(&new), LGetFloatAct(new));
						return new;
					}
				} else if (letter == 'e' || letter == 'E') {
					//exponent
					strncat(lexem, &letter, 1);
					letter = getchar();
					if (letter == '+' || letter == '-') {
						strncat(lexem, &letter, 1);
						letter = getchar();
					}
					if (!(letter >= '0' && letter <= '9')) {
						new.kw = LEXEM;
						new.i = LEXEM_ERROR;
						return new;
					}
					while (letter >= '0' && letter <= '9') {
						strncat(lexem, &letter, 1);
						letter = getchar();
					}
					d = atof(lexem);
					new.kw = decim;
					new.f = d;
					strcpy(new.s, lexem);
					printf("decim_%s_%f ", LGetStrAct(&new), LGetFloatAct(new));
					return new;
				} else {
					//celá
					n = atoi(lexem);
					new.kw = num;
					new.i = n;
					strcpy(new.s, lexem);
					printf("num_%s_%d ", LGetStrAct(&new), LGetNumAct(new));
					return new;
				}
				break;
			case 'a' ... 'z':
			case 'A' ... 'Z':
			case '_':
				lexem[0] = '\0';
				//samotné podtržítko
				if (letter == '_') {
					letter = getchar();
					if (!(letter >= 'a' && letter <= 'z') && !(letter >= 'A' && letter <= 'Z') && !(letter >= '0' && letter <= '9') && letter != '_') {
						new.kw = underscore;
						printf("underscore ");
						ungetc(letter, stdin);
						return new;
						break;
					} else {
						strcat(lexem, "_");
					}
				}
				//identifikátory
				while ((letter >= '0' && letter <= '9') || (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || letter == '_') {
					strncat(lexem, &letter, 1);
					letter = getchar();
				}
				switch (lexem[0]) {
					case 'o':
						//ord
						if (!strcmp(lexem, "ord")) {
							new.kw = inord;
							printf("inord ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'c':
						//const concat chr
						if (!strcmp(lexem, "chr")) {
							new.kw = inchr;
							printf("inchr ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "const")) {
							new.kw = constant;
							printf("konst ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "concat")) {
							new.kw = inccat;
							printf("inccat ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'v':
						//var void
						if (!strcmp(lexem, "var")) {
							new.kw = variable;
							printf("variable ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "void")) {
							new.kw = dtvoid;
							printf("dtvoid ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'l':
						//length
						if (!strcmp(lexem, "length")) {
							new.kw = inlen;
							printf("inlen ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'r':
						//return readstr readi32 readf64
						switch (lexem[4]) {
							case 's':
								if (!strcmp(lexem, "readstr")) {
									new.kw = inres;
									printf("inres ");
									ungetc(letter, stdin);
									return new;
								} else {
									//id
									new.kw = id;
									strcpy(new.s, lexem);
									printf("id_%s ", LGetStrAct(&new));
									ungetc(letter, stdin);
									return new;
								}
								break;
							case 'i':
								if (!strcmp(lexem, "readi32")) {
									new.kw = inrei;
									printf("inrei ");
									ungetc(letter, stdin);
									return new;
								} else {
									//id
									new.kw = id;
									strcpy(new.s, lexem);
									printf("id_%s ", LGetStrAct(&new));
									ungetc(letter, stdin);
									return new;
								}
								break;
							case 'f':
								if (!strcmp(lexem, "readf64")) {
									new.kw = inref;
									printf("inref ");
									ungetc(letter, stdin);
									return new;
								} else {
									//id
									new.kw = id;
									strcpy(new.s, lexem);
									printf("id_%s ", LGetStrAct(&new));
									ungetc(letter, stdin);
									return new;
								}
								break;
							case 'r':
								if (!strcmp(lexem, "return")) {
									new.kw = _return;
									printf("_return ");
									ungetc(letter, stdin);
									return new;
								} else {
									//id
									new.kw = id;
									strcpy(new.s, lexem);
									printf("id_%s ", LGetStrAct(&new));
									ungetc(letter, stdin);
									return new;
								}
								break;
							default:
								//id
								new.kw = id;
								strcpy(new.s, lexem);
								printf("id_%s ", LGetStrAct(&new));
								ungetc(letter, stdin);
								return new;
								break;
						}
						break;
					case 'p':
						//pub
						if (!strcmp(lexem, "pub")) {
							new.kw = _pub;
							printf("_pub ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'u':
						//u8
						if (!strcmp(lexem, "u8")) {
							new.kw = dtstr;
							printf("dtstr ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'm':
						//main
						if (!strcmp(lexem, "main")) {
							new.kw = _main;
							printf("_main ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'f':
						//fn f64 f2i
						if (!strcmp(lexem, "fn")) {
							new.kw = _fn;
							printf("_fn ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "f64")) {
							new.kw = dtflt;
							printf("dtflt ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "f2i")) {
							new.kw = inf2i;
							printf("inf2i ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'w':
						//while write
						if (!strcmp(lexem, "write")) {
							new.kw = inwrt;
							printf("inwrt ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "while")) {
							new.kw = _while;
							printf("_while ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'e':
						//else
						if (!strcmp(lexem, "else")) {
							new.kw = _else;
							printf("_else ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'i':
						//if i32 ifj import i2f
						if (!strcmp(lexem, "if")) {
							new.kw = _if;
							printf("_if ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "i32")) {
							new.kw = dtint;
							printf("dtint ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "ifj")) {
							new.kw = inbuild;
							printf("inbuild ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "import")) {
							new.kw = _import;
							printf("_import ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "i2f")) {
							new.kw = ini2f;
							printf("ini2f ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 's':
						//string substring strcmp
						if (!strcmp(lexem, "substring")) {
							new.kw = inssub;
							printf("inssub ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "string")) {
							new.kw = inu2s;
							printf("inu2s ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "strcmp")) {
							new.kw = inscmp;
							printf("inscmp ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'n':
						//null
						if (!strcmp(lexem, "null")) {
							new.kw = _null;
							printf("_null ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							new.kw = id;
							strcpy(new.s, lexem);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					default:
						//id
						new.kw = id;
						strcpy(new.s, lexem);
						printf("id_%s ", LGetStrAct(&new));
						ungetc(letter, stdin);
						return new;
				}
				break;
			default:
				//chyba lexemu
				new.kw = LEXEM;
				new.i = LEXEM_ERROR;
				return new;
		}
	}
	new.kw = end;
	return new;
}


/*
#include <stdio.h>
#include <string.h>

typedef struct SToken{
	//KeyWord kw;
	int i;
	double f;
	char s[256];
	//struct SToken *next;
} Token;

char *LGetStrAct(Token *T) {
	return T->s;
}

int main(){
    char a[256]="ggggggggggg";
    Token new = {10, 33.3, ""};
    strcpy(new.s, a);
    
    
    printf("Hello World %s\n", LGetStrAct(&new));
    char q= getchar();
    ungetc(q, stdin);
    printf("%c", getchar());

    return 0;
}*/