/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#include "errors_enum.h"
#include "tokens.h"

Token get_token/*Lexem_analyzer*/(/*List *L*/) {
	//inicializace
	char lexem[256];
	int n;
	double d;
	Token new = {next, 0, 0, ""};
	//pokud nealokuje, vrátí chybu
	/*if (!(lexem = (char *)malloc(sizeof(char) * 256))) {
		return INTERNAL_COMPILER_ERROR;
	}*/
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
				//LInsertLast(L, next, NULL, NULL, NULL);
				//new = {next, 0, 0, ""};
				printf("next\n");
				return new;
				//letter = getchar();
				break;
			case ':':
				//LInsertLast(L, colon, NULL, NULL, NULL);
				new.kw = colon;
				printf("colon ");
				return new;
				//letter = getchar();
				break;
			case '(':
				//LInsertLast(L, lbracket, NULL, NULL, NULL);
				new.kw = lbracket;
				printf("lbracket ");
				return new;
				//letter = getchar();
				break;
			case ')':
				//LInsertLast(L, rbracket, NULL, NULL, NULL);
				new.kw = rbracket;
				printf("pbracket ");
				return new;
				//letter = getchar();
				break;
			case '{':
				//LInsertLast(L, lblock, NULL, NULL, NULL);
				new.kw = lblock;
				printf("lblock\n");
				return new;
				//letter = getchar();
				break;
			case '}':
				//LInsertLast(L, rblock, NULL, NULL, NULL);
				new.kw = rblock;
				printf("pblock\n");
				return new;
				//letter = getchar();
				break;
			case ',':
				//LInsertLast(L, comma, NULL, NULL, NULL);
				new.kw = comma;
				printf("comma ");
				return new;
				//letter = getchar();
				break;
			case '.':
				//LInsertLast(L, dot, NULL, NULL, NULL);
				new.kw = dot;
				printf("dot ");
				return new;
				//letter = getchar();
				break;
			case '+':
				//LInsertLast(L, plus, NULL, NULL, NULL);
				new.kw = plus;
				printf("plus ");
				return new;
				//letter = getchar();
				break;
			case '-':
				//LInsertLast(L, minus, NULL, NULL, NULL);
				new.kw = minus;
				printf("minus ");
				return new;
				//letter = getchar();
				break;
			case '*':
				//LInsertLast(L, multiply, NULL, NULL, NULL);
				new.kw = multiply;
				printf("krat ");
				return new;
				//letter = getchar();
				break;
			case '?':
				//LInsertLast(L, question_mark, NULL, NULL, NULL);
				new.kw = question_mark;
				printf("choice ");
				return new;
				//letter = getchar();
				break;
			case '|':
				//LInsertLast(L, vertical_bar, NULL, NULL, NULL);
				new.kw = vertical_bar;
				printf("popodm ");
				return new;
				//letter = getchar();
				break;
			case '@':
				//LInsertLast(L, at, NULL, NULL, NULL);
				new.kw = at;
				printf("zavin ");
				return new;
				//letter = getchar();
				break;
			case '[':
				letter = getchar();
				if (letter == ']') {
					//LInsertLast(L, square_brackets, NULL, NULL, NULL);
					new.kw = square_brackets;
					printf("square_brackets ");
					return new;
					//letter = getchar();
				} else {
					//free(lexem);
					new.kw = LEXEM;
					new.i = LEXEM_ERROR;
					return new;
				}
				break;
			case '<':
				letter = getchar();
				if (letter == '=') {
					//LInsertLast(L, lequal, NULL, NULL, NULL);
					new.kw = lequal;
					printf("lequal ");
					return new;
					//letter = getchar();
				} else {
					//LInsertLast(L, less, NULL, NULL, NULL);
					new.kw = less;
					printf("less ");
					ungetc(letter, stdin);
					return new;
				}
				break;
			case '>':
				letter = getchar();
				if (letter == '=') {
					//LInsertLast(L, mequal, NULL, NULL, NULL);
					new.kw = mequal;
					printf("mequal ");
					return new;
					//letter = getchar();
				} else {
					//LInsertLast(L, more, NULL, NULL, NULL);
					new.kw = more;
					printf("more ");
					ungetc(letter, stdin);
					return new;
				}
				break;
			case '!':
				letter = getchar();
				if (letter == '=') {
					//LInsertLast(L, nequal, NULL, NULL, NULL);
					new.kw = nequal;
					printf("nequal ");
					return new;
					//letter = getchar();
				} else {
					//free(lexem);
					new.kw = LEXEM;
					new.i = LEXEM_ERROR;
					return new;
				}
				break;
			case '=':
				letter = getchar();
				if (letter != '=') {
					//LInsertLast(L, equal, NULL, NULL, NULL);
					new.kw = equal;
					printf("compare_equal ");
					ungetc(letter, stdin);
					return new;
				} else {
					//LInsertLast(L, compare_equal, NULL, NULL, NULL);
					new.kw = compare_equal;
					printf("equal ");
					return new;
					//letter = getchar();
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
					//LInsertLast(L, division, NULL, NULL, NULL);
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
						//free(lexem);
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
											//free(lexem);
											new.kw = LEXEM;
											new.i = LEXEM_ERROR;
											return new;
										}
									} else {
										//chyba lexemu
										//free(lexem);
										new.kw = LEXEM;
										new.i = LEXEM_ERROR;
										return new;
									}
									break;
								default:
									//chyba lexemu
									//free(lexem);
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
				/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
					free(lexem);
					return INTERNAL_COMPILER_ERROR;
				}*/
				new.kw = text;
				strcpy(new.s, lexem);
				//LInsertLast(L, text, NULL, NULL, p);
				printf("text_%s ", LGetStrAct(&new));
				ungetc(letter, stdin);
				return new;
				//lexem[0] = '\0';
				break;
			case '"':
				//string
				letter = getchar();
				if (letter == '"') {
					//druhý
					letter = getchar();
					//prázdný
					/*if (!(p = (char *)malloc(sizeof(char)))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}
					*p = '\0';
					LInsertLast(L, text, NULL, NULL, p);*/
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
											//free(lexem);
											new.kw = LEXEM;
											new.i = LEXEM_ERROR;
											return new;
										}
									} else {
										//chyba lexemu
										//free(lexem);
										new.kw = LEXEM;
										new.i = LEXEM_ERROR;
										return new;
									}
									break;
								default:
									//chyba lexemu
									//free(lexem);
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
						//free(lexem);
						new.kw = LEXEM;
						new.i = LEXEM_ERROR;
						return new;
					}
					/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}*/
					new.kw = text;
					strcpy(new.s, lexem);
					printf("text_%s ", LGetStrAct(&new));
					return new;
					//LInsertLast(L, text, NULL, NULL, p);
					//lexem[0] = '\0';
					//letter = getchar();
				}
				break;
			case '0':
				//nezačíná nulou
				letter = getchar();
				if (letter >= '0' && letter <= '9') {
					//free(lexem);
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
						//free(lexem);
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
							//free(lexem);
							new.kw = LEXEM;
							new.i = LEXEM_ERROR;
							return new;
						}
						while (letter >= '0' && letter <= '9') {
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
						/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
							free(lexem);
							return INTERNAL_COMPILER_ERROR;
						}*/
						/*if (!(d = (double *)malloc(sizeof(double)))) {
							free(lexem);
							return INTERNAL_COMPILER_ERROR;
						}*/
						d = atof(lexem);
						//LInsertLast(L, decim, NULL, d, p);
						//lexem[0] = '\0';
						new.kw = decim;
						new.f = d;
						strcpy(new.s, lexem);
						printf("decim_%s_%f ", LGetStrAct(&new), LGetFloatAct(new));
						return new;
					} else {
						//jen tečka
						/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
							free(lexem);
							return INTERNAL_COMPILER_ERROR;
						}
						strcpy(p, lexem);
						if (!(d = (double *)malloc(sizeof(double)))) {
							free(lexem);
							return INTERNAL_COMPILER_ERROR;
						}*/
						d = atof(lexem);
						//LInsertLast(L, decim, NULL, d, p);
						new.kw = decim;
						new.f = d;
						strcpy(new.s, lexem);
						printf("decim_%s_%f ", LGetStrAct(&new), LGetFloatAct(new));
						return new;
						//lexem[0] = '\0';
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
						//free(lexem);
						new.kw = LEXEM;
						new.i = LEXEM_ERROR;
						return new;
					}
					while (letter >= '0' && letter <= '9') {
						strncat(lexem, &letter, 1);
						letter = getchar();
					}
					/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}
					strcpy(p, lexem);
					if (!(d = (double *)malloc(sizeof(double)))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}*/
					d = atof(lexem);
					//LInsertLast(L, decim, NULL, d, p);
					new.kw = decim;
					new.f = d;
					strcpy(new.s, lexem);
					printf("decim_%s_%f ", LGetStrAct(&new), LGetFloatAct(new));
					return new;
					//lexem[0] = '\0';
				} else {
					//celá
					/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}
					strcpy(p, lexem);
					if (!(n = (int *)malloc(sizeof(int)))) {
						free(lexem);
						return INTERNAL_COMPILER_ERROR;
					}*/
					n = atoi(lexem);
					//LInsertLast(L, num, n, NULL, p);
					//printf("num_%s_%d ", p, *n);
					new.kw = num;
					new.i = n;
					strcpy(new.s, lexem);
					printf("num_%s_%d ", LGetStrAct(&new), LGetNumAct(new));
					return new;
					//lexem[0] = '\0';
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
						//LInsertLast(L, underscore, NULL, NULL, NULL);
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
							//LInsertLast(L, inord, NULL, NULL, NULL);
							new.kw = inord;
							printf("inord ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							//LInsertLast(L, id, NULL, NULL, p);
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
							//LInsertLast(L, inchr, NULL, NULL, NULL);
							new.kw = inchr;
							printf("inchr ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "const")) {
							//LInsertLast(L, constant, NULL, NULL, NULL);
							new.kw = constant;
							printf("konst ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "concat")) {
							//LInsertLast(L, inccat, NULL, NULL, NULL);
							new.kw = inccat;
							printf("inccat ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'v':
						//var void
						if (!strcmp(lexem, "var")) {
							//LInsertLast(L, variable, NULL, NULL, NULL);
							new.kw = variable;
							printf("variable ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "void")) {
							//LInsertLast(L, dtvoid, NULL, NULL, NULL);
							new.kw = dtvoid;
							printf("dtvoid ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'l':
						//length
						if (!strcmp(lexem, "length")) {
							//LInsertLast(L, inlen, NULL, NULL, NULL);
							new.kw = inlen;
							printf("inlen ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
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
									//LInsertLast(L, inres, NULL, NULL, NULL);
									new.kw = inres;
									printf("inres ");
									ungetc(letter, stdin);
									return new;
								} else {
									//id
									/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
										free(lexem);
										return INTERNAL_COMPILER_ERROR;
									}*/
									new.kw = id;
									strcpy(new.s, lexem);
									//LInsertLast(L, id, NULL, NULL, p);
									printf("id_%s ", LGetStrAct(&new));
									ungetc(letter, stdin);
									return new;
								}
								break;
							case 'i':
								if (!strcmp(lexem, "readi32")) {
									//LInsertLast(L, inrei, NULL, NULL, NULL);
									new.kw = inrei;
									printf("inrei ");
									ungetc(letter, stdin);
									return new;
								} else {
									//id
									/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
										free(lexem);
										return INTERNAL_COMPILER_ERROR;
									}*/
									new.kw = id;
									strcpy(new.s, lexem);
									//LInsertLast(L, id, NULL, NULL, p);
									printf("id_%s ", LGetStrAct(&new));
									ungetc(letter, stdin);
									return new;
								}
								break;
							case 'f':
								if (!strcmp(lexem, "readf64")) {
									//LInsertLast(L, inref, NULL, NULL, NULL);
									new.kw = inref;
									printf("inref ");
									ungetc(letter, stdin);
									return new;
								} else {
									//id
									/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
										free(lexem);
										return INTERNAL_COMPILER_ERROR;
									}*/
									new.kw = id;
									strcpy(new.s, lexem);
									//LInsertLast(L, id, NULL, NULL, p);
									printf("id_%s ", LGetStrAct(&new));
									ungetc(letter, stdin);
									return new;
								}
								break;
							case 'r':
								if (!strcmp(lexem, "return")) {
									//LInsertLast(L, _return, NULL, NULL, NULL);
									new.kw = _return;
									printf("_return ");
									ungetc(letter, stdin);
									return new;
								} else {
									//id
									/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
										free(lexem);
										return INTERNAL_COMPILER_ERROR;
									}*/
									new.kw = id;
									strcpy(new.s, lexem);
									//LInsertLast(L, id, NULL, NULL, p);
									printf("id_%s ", LGetStrAct(&new));
									ungetc(letter, stdin);
									return new;
								}
								break;
							default:
								//id
								/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
									free(lexem);
									return INTERNAL_COMPILER_ERROR;
								}*/
									new.kw = id;
								strcpy(new.s, lexem);
								//LInsertLast(L, id, NULL, NULL, p);
								printf("id_%s ", LGetStrAct(&new));
								ungetc(letter, stdin);
								return new;
								break;
						}
						break;
					case 'p':
						//pub
						if (!strcmp(lexem, "pub")) {
							//LInsertLast(L, _pub, NULL, NULL, NULL);
							new.kw = _pub;
							printf("_pub ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'u':
						//u8
						if (!strcmp(lexem, "u8")) {
							//LInsertLast(L, dtstr, NULL, NULL, NULL);
							new.kw = dtstr;
							printf("dtstr ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
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
							//LInsertLast(L, _main, NULL, NULL, NULL);
							new.kw = _main;
							printf("_main ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'f':
						//fn f64 f2i
						if (!strcmp(lexem, "fn")) {
							//LInsertLast(L, _fn, NULL, NULL, NULL);
							new.kw = _fn;
							printf("_fn ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "f64")) {
							//LInsertLast(L, dtflt, NULL, NULL, NULL);
							new.kw = dtflt;
							printf("dtflt ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "f2i")) {
							//LInsertLast(L, inf2i, NULL, NULL, NULL);
							new.kw = inf2i;
							printf("inf2i ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'w':
						//while write
						if (!strcmp(lexem, "write")) {
							//LInsertLast(L, inwrt, NULL, NULL, NULL);
							new.kw = inwrt;
							printf("inwrt ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "while")) {
							//LInsertLast(L, _while, NULL, NULL, NULL);
							new.kw = _while;
							printf("_while ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'e':
						//else
						if (!strcmp(lexem, "else")) {
							//LInsertLast(L, _else, NULL, NULL, NULL);
							new.kw = _else;
							printf("_else ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'i':
						//if i32 ifj import i2f
						if (!strcmp(lexem, "if")) {
							//LInsertLast(L, _if, NULL, NULL, NULL);
							new.kw = _if;
							printf("_if ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "i32")) {
							//LInsertLast(L, dtint, NULL, NULL, NULL);
							new.kw = dtint;
							printf("dtint ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "ifj")) {
							//LInsertLast(L, inbuild, NULL, NULL, NULL);
							new.kw = inbuild;
							printf("inbuild ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "import")) {
							//LInsertLast(L, _import, NULL, NULL, NULL);
							new.kw = _import;
							printf("_import ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "i2f")) {
							//LInsertLast(L, ini2f, NULL, NULL, NULL);
							new.kw = ini2f;
							printf("ini2f ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 's':
						//string substring strcmp
						if (!strcmp(lexem, "substring")) {
							//LInsertLast(L, inssub, NULL, NULL, NULL);
							new.kw = inssub;
							printf("inssub ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "string")) {
							//LInsertLast(L, inu2s, NULL, NULL, NULL);
							new.kw = inu2s;
							printf("inu2s ");
							ungetc(letter, stdin);
							return new;
						} else if (!strcmp(lexem, "strcmp")) {
							//LInsertLast(L, inscmp, NULL, NULL, NULL);
							new.kw = inscmp;
							printf("inscmp ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					case 'n':
						//null
						if (!strcmp(lexem, "null")) {
							//LInsertLast(L, _null, NULL, NULL, NULL);
							new.kw = _null;
							printf("_null ");
							ungetc(letter, stdin);
							return new;
						} else {
							//id
							/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
								free(lexem);
								return INTERNAL_COMPILER_ERROR;
							}*/
							new.kw = id;
							strcpy(new.s, lexem);
							//LInsertLast(L, id, NULL, NULL, p);
							printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							return new;
						}
						break;
					default:
						//id
						/*if (!(p = (char *)malloc(sizeof(char) * (1 + strlen(lexem))))) {
							free(lexem);
							return INTERNAL_COMPILER_ERROR;
						}*/
						new.kw = id;
						strcpy(new.s, lexem);
						//LInsertLast(L, id, NULL, NULL, p);
						printf("id_%s ", LGetStrAct(&new));
						ungetc(letter, stdin);
						return new;
				}
				//lexem[0] = '\0';
				break;
			default:
				//chyba lexemu
				//free(lexem); //printf("%d", letter);
				new.kw = LEXEM;
				new.i = LEXEM_ERROR;
				return new;
		}
	}
	//*p, *n, *d neuvolňuji, přešli do tokenu
	//LInsertLast(L, end, NULL, NULL, NULL);
	//free(lexem);
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