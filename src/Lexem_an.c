/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#include "errors_enum.h"
#include "tokens.h"

int Lexem_analyzer(List *L) {
	//inicializace
	char *lexem, *p;
	int *n;
	double *d;
	//pokud nealokuje, vrátí chybu
	if (!(lexem = (char *)malloc(sizeof(char) * 256))) {
		return INTERNAL_COMPILER_ERROR;
	}
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
				LInsertLast(L, next, NULL, NULL, NULL);
				printf("next\n");
				letter = getchar();
				break;
			case ':':
				LInsertLast(L, colon, NULL, NULL, NULL);
				printf("colon ");
				letter = getchar();
				break;
			case '(':
				LInsertLast(L, lbracket, NULL, NULL, NULL);
				printf("lbracket ");
				letter = getchar();
				break;
			case ')':
				LInsertLast(L, rbracket, NULL, NULL, NULL);
				printf("pbracket ");
				letter = getchar();
				break;
			case '{':
				LInsertLast(L, lblock, NULL, NULL, NULL);
				printf("lblock\n");
				letter = getchar();
				break;
			case '}':
				LInsertLast(L, rblock, NULL, NULL, NULL);
				printf("pblock\n");
				letter = getchar();
				break;
			case ',':
				LInsertLast(L, comma, NULL, NULL, NULL);
				printf("comma ");
				letter = getchar();
				break;
			case '.':
				LInsertLast(L, dot, NULL, NULL, NULL);
				printf("dot ");
				letter = getchar();
				break;
			case '+':
				LInsertLast(L, plus, NULL, NULL, NULL);
				printf("plus ");
				letter = getchar();
				break;
			case '-':
				LInsertLast(L, minus, NULL, NULL, NULL);
				printf("minus ");
				letter = getchar();
				break;
			case '*':
				LInsertLast(L, multiply, NULL, NULL, NULL);
				printf("krat ");
				letter = getchar();
				break;
			case '?':
				LInsertLast(L, question_mark, NULL, NULL, NULL);
				printf("optional ");
				letter = getchar();
				break;
			case '|':
				LInsertLast(L, vertical_bar, NULL, NULL, NULL);
				printf("popodm ");
				letter = getchar();
				break;
			case '@':
				LInsertLast(L, at, NULL, NULL, NULL);
				printf("zavin ");
				letter = getchar();
				break;
			case '[':
				letter = getchar();
				if (letter == ']') {
					LInsertLast(L, square_brackets, NULL, NULL, NULL);
					printf("square_brackets ");
					letter = getchar();
				} else {
					free(lexem);
					return LEXEM_ERROR;
				}
				break;
			case '<':
				letter = getchar();
				if (letter == '=') {
					LInsertLast(L, lequal, NULL, NULL, NULL);
					printf("lequal ");
					letter = getchar();
				} else {
					LInsertLast(L, less, NULL, NULL, NULL);
					printf("less ");
				}
				break;
			case '>':
				letter = getchar();
				if (letter == '=') {
					LInsertLast(L, mequal, NULL, NULL, NULL);
					printf("mequal ");
					letter = getchar();
				} else {
					LInsertLast(L, more, NULL, NULL, NULL);
					printf("more ");
				}
				break;
			case '!':
				letter = getchar();
				if (letter == '=') {
					LInsertLast(L, nequal, NULL, NULL, NULL);
					printf("nequal ");
					letter = getchar();
				} else {
					free(lexem);
					return LEXEM_ERROR;
				}
				break;
			case '=':
				letter = getchar();
				if (letter != '=') {
					LInsertLast(L, equal, NULL, NULL, NULL);
					printf("compare_equal ");
				} else {
					LInsertLast(L, compare_equal, NULL, NULL, NULL);
					printf("equal ");
					letter = getchar();
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
					LInsertLast(L, division, NULL, NULL, NULL);
					printf("division ");
				}
				break;
			case '\\':
				//víceřádkový string
				do {
					letter = getchar();
					if (letter != '\\') {
						free(lexem);
						return LEXEM_ERROR;
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
											free(lexem);
											return LEXEM_ERROR;
										}
									} else {
										//chyba lexemu
										free(lexem);
										return LEXEM_ERROR;
									}
									break;
								default:
									//chyba lexemu
									free(lexem);
									return LEXEM_ERROR;
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
				if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
					free(lexem);
					return INTERNAL_COMPILER_ERROR;
				}
				strcpy(p, lexem);
				LInsertLast(L, text, NULL, NULL, p);
				printf("text_%s ", p);
				lexem[0] = '\0';
				break;
			case '"':
				//string
				letter = getchar();
				if (letter == '"') {
					//druhý
					letter = getchar();
					//prázdný
					if (!(p = (char *)malloc(sizeof(char)))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}
					*p = '\0';
					LInsertLast(L, text, NULL, NULL, p);
					printf("text_%s ", p);
					//}
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
											free(lexem);
											return LEXEM_ERROR;
										}
									} else {
										//chyba lexemu
										free(lexem);
										return LEXEM_ERROR;
									}
									break;
								default:
									//chyba lexemu
									free(lexem);
									return LEXEM_ERROR;
							}
						} else if (letter > 31) {
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
					}
					if (letter == EOF || letter == '\n') {
						//chyba lexemu
						free(lexem);
						return LEXEM_ERROR;
					}
					if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}
					strcpy(p, lexem);
					LInsertLast(L, text, NULL, NULL, p);
					printf("text_%s ", p);
					lexem[0] = '\0';
					letter = getchar();
				}
				break;
			case '0':
				//nezačíná nulou
				letter = getchar();
				if (letter >= '0' && letter <= '9') {
					free(lexem);
					return LEXEM_ERROR;
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
						free(lexem);
						return LEXEM_ERROR;
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
							free(lexem);
							return LEXEM_ERROR;
						}
						while (letter >= '0' && letter <= '9') {
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
						if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
							free(lexem);
							return INTERNAL_COMPILER_ERROR;
						}
						strcpy(p, lexem);
						if (!(d = (double *)malloc(sizeof(double)))) {
							free(lexem);
							return INTERNAL_COMPILER_ERROR;
						}
						*d = atof(lexem);
						LInsertLast(L, decim, NULL, d, p);
						printf("decim_%s_%f ", p, *d);
						lexem[0] = '\0';
					} else {
						//jen tečka
						if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
							free(lexem);
							return INTERNAL_COMPILER_ERROR;
						}
						strcpy(p, lexem);
						if (!(d = (double *)malloc(sizeof(double)))) {
							free(lexem);
							return INTERNAL_COMPILER_ERROR;
						}
						*d = atof(lexem);
						LInsertLast(L, decim, NULL, d, p);
						printf("decim_%s_%f ", p, *d);
						lexem[0] = '\0';
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
						free(lexem);
						return LEXEM_ERROR;
					}
					while (letter >= '0' && letter <= '9') {
						strncat(lexem, &letter, 1);
						letter = getchar();
					}
					if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}
					strcpy(p, lexem);
					if (!(d = (double *)malloc(sizeof(double)))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}
					*d = atof(lexem);
					LInsertLast(L, decim, NULL, d, p);
					printf("decim_%s_%f ", p, *d);
					lexem[0] = '\0';
				} else {
					//celá
					if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}
					strcpy(p, lexem);
					if (!(n = (int *)malloc(sizeof(int)))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}
					*n = atoi(lexem);
					LInsertLast(L, num, n, NULL, p);
					printf("num_%s_%d ", p, *n);
					lexem[0] = '\0';
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
						LInsertLast(L, underscore, NULL, NULL, NULL);
						printf("underscore ");
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
							LInsertLast(L, inord, NULL, NULL, NULL);
							printf("inord ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'c':
						//const concat chr
						if (!strcmp(lexem, "chr")) {
							LInsertLast(L, inchr, NULL, NULL, NULL);
							printf("inchr ");
						} else if (!strcmp(lexem, "const")) {
							LInsertLast(L, constant, NULL, NULL, NULL);
							printf("konst ");
						} else if (!strcmp(lexem, "concat")) {
							LInsertLast(L, inccat, NULL, NULL, NULL);
							printf("inccat ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'v':
						//var void
						if (!strcmp(lexem, "var")) {
							LInsertLast(L, variable, NULL, NULL, NULL);
							printf("variable ");
						} else if (!strcmp(lexem, "void")) {
							LInsertLast(L, dtvoid, NULL, NULL, NULL);
							printf("dtvoid ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'l':
						//length
						if (!strcmp(lexem, "length")) {
							LInsertLast(L, inlen, NULL, NULL, NULL);
							printf("inlen ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'r':
						//return readstr readi32 readf64
						switch (lexem[4]) {
							case 's':
								if (!strcmp(lexem, "readstr")) {
									LInsertLast(L, inres, NULL, NULL, NULL);
									printf("inres ");
								} else {
									//id
									if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
										free(lexem);
										return INTERNAL_COMPILER_ERROR;
									}
									strcpy(p, lexem);
									LInsertLast(L, id, NULL, NULL, p);
									printf("id_%s ", p);
								}
								break;
							case 'i':
								if (!strcmp(lexem, "readi32")) {
									LInsertLast(L, inrei, NULL, NULL, NULL);
									printf("inrei ");
								} else {
									//id
									if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
										free(lexem);
										return INTERNAL_COMPILER_ERROR;
									}
									strcpy(p, lexem);
									LInsertLast(L, id, NULL, NULL, p);
									printf("id_%s ", p);
								}
								break;
							case 'f':
								if (!strcmp(lexem, "readf64")) {
									LInsertLast(L, inref, NULL, NULL, NULL);
									printf("inref ");
								} else {
									//id
									if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
										free(lexem);
										return INTERNAL_COMPILER_ERROR;
									}
									strcpy(p, lexem);
									LInsertLast(L, id, NULL, NULL, p);
									printf("id_%s ", p);
								}
								break;
							case 'r':
								if (!strcmp(lexem, "return")) {
									LInsertLast(L, _return, NULL, NULL, NULL);
									printf("return ");
								} else {
									//id
									if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
										free(lexem);
										return INTERNAL_COMPILER_ERROR;
									}
									strcpy(p, lexem);
									LInsertLast(L, id, NULL, NULL, p);
									printf("id_%s ", p);
								}
								break;
							default:
								//id
								if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
									free(lexem);
									return INTERNAL_COMPILER_ERROR;
								}
								strcpy(p, lexem);
								LInsertLast(L, id, NULL, NULL, p);
								printf("id_%s ", p);
								break;
						}
						break;
					case 'p':
						//pub
						if (!strcmp(lexem, "pub")) {
							LInsertLast(L, _pub, NULL, NULL, NULL);
							printf("_pub");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'u':
						//u8
						if (!strcmp(lexem, "u8")) {
							LInsertLast(L, dtstr, NULL, NULL, NULL);
							printf("dtstr ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'm':
						//main
						if (!strcmp(lexem, "main")) {
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, _main, NULL, NULL, p);
							printf("_main ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'f':
						//fn f64 f2i
						if (!strcmp(lexem, "fn")) {
							LInsertLast(L, _fn, NULL, NULL, NULL);
							printf("_fn ");
						} else if (!strcmp(lexem, "f64")) {
							LInsertLast(L, dtflt, NULL, NULL, NULL);
							printf("dtflt ");
						} else if (!strcmp(lexem, "f2i")) {
							LInsertLast(L, inf2i, NULL, NULL, NULL);
							printf("inf2i ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'w':
						//while write
						if (!strcmp(lexem, "write")) {
							LInsertLast(L, inwrt, NULL, NULL, NULL);
							printf("inwrt ");
						} else if (!strcmp(lexem, "while")) {
							LInsertLast(L, _while, NULL, NULL, NULL);
							printf("_while ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'e':
						//else
						if (!strcmp(lexem, "else")) {
							LInsertLast(L, _else, NULL, NULL, NULL);
							printf("_else ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'i':
						//if i32 ifj import i2f
						if (!strcmp(lexem, "if")) {
							LInsertLast(L, _if, NULL, NULL, NULL);
							printf("_if ");
						} else if (!strcmp(lexem, "i32")) {
							LInsertLast(L, dtint, NULL, NULL, NULL);
							printf("dtint ");
						} else if (!strcmp(lexem, "ifj")) {
							LInsertLast(L, inbuild, NULL, NULL, NULL);
							printf("inbuild ");
						} else if (!strcmp(lexem, "import")) {
							LInsertLast(L, _import, NULL, NULL, NULL);
							printf("import ");
						} else if (!strcmp(lexem, "i2f")) {
							LInsertLast(L, ini2f, NULL, NULL, NULL);
							printf("ini2f ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 's':
						//string substring strcmp
						if (!strcmp(lexem, "substring")) {
							LInsertLast(L, inssub, NULL, NULL, NULL);
							printf("inssub ");
						} else if (!strcmp(lexem, "string")) {
							LInsertLast(L, inu2s, NULL, NULL, NULL);
							printf("inu2s ");
						} else if (!strcmp(lexem, "strcmp")) {
							LInsertLast(L, inscmp, NULL, NULL, NULL);
							printf("inscmp ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					case 'n':
						//null
						if (!strcmp(lexem, "null")) {
							LInsertLast(L, _null, NULL, NULL, NULL);
							printf("_null ");
						} else {
							//id
							if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}
							strcpy(p, lexem);
							LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", p);
						}
						break;
					default:
						//id
						if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
							free(lexem);
							return INTERNAL_COMPILER_ERROR;
						}
						strcpy(p, lexem);
						LInsertLast(L, id, NULL, NULL, p);
						printf("id_%s ", p);
						break;
				}
				lexem[0] = '\0';
				break;
			default:
				//chyba lexemu
				free(lexem); //printf("%d", letter);
				return LEXEM_ERROR;
		}
	}
	//*p, *n, *d neuvolňuji, přešli do tokenu
	LInsertLast(L, end, NULL, NULL, NULL);
	free(lexem);
	return 0;
}
