/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#include "errors_enum.h"
#include "tokens.h"

Token get_token() {
	//inicializace
	int n;
	size_t lex_size = 8;
	double d;
	Token new = {next, 0, 0, NULL};
	char letter = getchar();

	char *p;
	char *lexem = malloc(sizeof(char) * lex_size);
	if (lexem == NULL) {
		new.kw = INTERNAL;
		new.i = INTERNAL_COMPILER_ERROR;
		return new;
	}
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
				//printf("next\n");
				free(lexem);
				return new;
				break;
			case ':':
				new.kw = colon;
				//printf("colon ");
				free(lexem);
				return new;
				break;
			case '(':
				new.kw = lbracket;
				//printf("lbracket ");
				free(lexem);
				return new;
				break;
			case ')':
				new.kw = rbracket;
				//printf("pbracket ");
				free(lexem);
				return new;
				break;
			case '{':
				new.kw = lblock;
				//printf("lblock\n");
				free(lexem);
				return new;
				break;
			case '}':
				new.kw = rblock;
				//printf("pblock\n");
				free(lexem);
				return new;
				break;
			case ',':
				new.kw = comma;
				//printf("comma ");
				free(lexem);
				return new;
				break;
			case '.':
				new.kw = dot;
				//printf("dot ");
				free(lexem);
				return new;
				break;
			case '+':
				new.kw = plus;
				//printf("plus ");
				free(lexem);
				return new;
				break;
			case '-':
				new.kw = minus;
				//printf("minus ");
				free(lexem);
				return new;
				break;
			case '*':
				new.kw = multiply;
				//printf("krat ");
				free(lexem);
				return new;
				break;
			case '?':
				new.kw = question_mark;
				//printf("choice ");
				free(lexem);
				return new;
				break;
			case '|':
				new.kw = vertical_bar;
				//printf("popodm ");
				free(lexem);
				return new;
				break;
			case '@':
				new.kw = at;
				//printf("zavin ");
				free(lexem);
				return new;
				break;
			case '[':
				letter = getchar();
				if (letter == ']') {
					new.kw = square_brackets;
					//printf("square_brackets ");
					free(lexem);
					return new;
				} else {
					new.kw = LEXEM;
					new.i = LEXEM_ERROR;
					free(lexem);
					return new;
				}
				break;
			case '<':
				letter = getchar();
				if (letter == '=') {
					new.kw = lequal;
					//printf("lequal ");
					free(lexem);
					return new;
				} else {
					new.kw = less;
					//printf("less ");
					ungetc(letter, stdin);
					free(lexem);
					return new;
				}
				break;
			case '>':
				letter = getchar();
				if (letter == '=') {
					new.kw = mequal;
					//printf("mequal ");
					free(lexem);
					return new;
				} else {
					new.kw = more;
					//printf("more ");
					ungetc(letter, stdin);
					free(lexem);
					return new;
				}
				break;
			case '!':
				letter = getchar();
				if (letter == '=') {
					new.kw = nequal;
					//printf("nequal ");
					free(lexem);
					return new;
				} else {
					new.kw = LEXEM;
					new.i = LEXEM_ERROR;
					free(lexem);
					return new;
				}
				break;
			case '=':
				letter = getchar();
				if (letter != '=') {
					new.kw = equal;
					//printf("equal ");
					ungetc(letter, stdin);
					free(lexem);
					return new;
				} else {
					new.kw = compare_equal;
					//printf("compare equal ");
					free(lexem);
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
					//printf("division ");
					ungetc(letter, stdin);
					free(lexem);
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
						free(lexem);
						return new;
					}
					letter = getchar();
					//takes all symbols
					while (letter != EOF && letter != '\n') {
						//check if size sufficient
						if (strlen(lexem) >= (lex_size - 1)) {
							lex_size *= 2;
							p = realloc(lexem, sizeof(char) * lex_size);
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							} else {
								lexem = p;
							}
						}
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
											free(lexem);
											return new;
										}
									} else {
										//chyba lexemu
										new.kw = LEXEM;
										new.i = LEXEM_ERROR;
										free(lexem);
										return new;
									}
									break;
								default:
									//chyba lexemu
									new.kw = LEXEM;
									new.i = LEXEM_ERROR;
									free(lexem);
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
				p = malloc(sizeof(char) * (strlen(lexem) + 1));
				if (p == NULL) {
					free(lexem);
					new.kw = INTERNAL;
					new.i = INTERNAL_COMPILER_ERROR;
					return new;
				}
				new.kw = text;
				strcpy(p, lexem);
				new.s = p;
				//printf("text_%s ", LGetStrAct(&new));
				ungetc(letter, stdin);
				free(lexem);
				return new;
				break;
			case '"':
				//string
				letter = getchar();
				if (letter == '"') {
					//druhý
					//prázdný
					p = malloc(sizeof(char) * (strlen(lexem) + 1));
					if (p == NULL) {
						free(lexem);
						new.kw = INTERNAL;
						new.i = INTERNAL_COMPILER_ERROR;
						return new;
					}
					new.kw = text;
					strcpy(p, lexem);
					new.s = p;
					//printf("text_%s ", LGetStrAct(&new));
					free(lexem);
					return new;
				} else {
					//jednoduchý
					while (letter != '"' && letter != EOF && letter != '\n') {
						//check if size sufficient
						if (strlen(lexem) >= (lex_size - 1)) {
							lex_size *= 2;
							p = realloc(lexem, sizeof(char) * lex_size);
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							} else {
								lexem = p;
							}
						}
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
											free(lexem);
											return new;
										}
									} else {
										//chyba lexemu
										new.kw = LEXEM;
										new.i = LEXEM_ERROR;
										free(lexem);
										return new;
									}
									break;
								default:
									//chyba lexemu
									new.kw = LEXEM;
									new.i = LEXEM_ERROR;
									free(lexem);
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
						free(lexem);
						return new;
					}
					p = malloc(sizeof(char) * (strlen(lexem) + 1));
					if (p == NULL) {
						free(lexem);
						new.kw = INTERNAL;
						new.i = INTERNAL_COMPILER_ERROR;
						return new;
					}
					new.kw = text;
					strcpy(p, lexem);
					new.s = p;
					//printf("text_%s ", LGetStrAct(&new));
					free(lexem);
					return new;
				}
				break;
			case '0':
				//nezačíná nulou
				letter = getchar();
				if (letter >= '0' && letter <= '9') {
					new.kw = LEXEM;
					new.i = LEXEM_ERROR;
					free(lexem);
					return new;
				}
				strncat(lexem, "00", 1);
			case '1' ... '9':
				//čísla
				while (letter >= '0' && letter <= '9') {
					//check if size sufficient
					if (strlen(lexem) >= (lex_size - 1)) {
						lex_size *= 2;
						p = realloc(lexem, sizeof(char) * lex_size);
						if (p == NULL) {
							free(lexem);
							new.kw = INTERNAL;
							new.i = INTERNAL_COMPILER_ERROR;
							return new;
						} else {
							lexem = p;
						}
					}
					strncat(lexem, &letter, 1);
					letter = getchar();
				}
				//desetiná
				//check if size sufficient
				if (strlen(lexem) >= (lex_size - 1)) {
					lex_size *= 2;
					p = realloc(lexem, sizeof(char) * lex_size);
					if (p == NULL) {
						free(lexem);
						new.kw = INTERNAL;
						new.i = INTERNAL_COMPILER_ERROR;
						return new;
					} else {
						lexem = p;
					}
				}
				if (letter == '.') {
					strncat(lexem, &letter, 1);
					letter = getchar();
					if (!(letter >= '0' && letter <= '9')) {
						new.kw = LEXEM;
						new.i = LEXEM_ERROR;
						free(lexem);
						return new;
					}
					while (letter >= '0' && letter <= '9') {
						//check if size sufficient
						if (strlen(lexem) >= (lex_size - 1)) {
							lex_size *= 2;
							p = realloc(lexem, sizeof(char) * lex_size);
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							} else {
								lexem = p;
							}
						}
						strncat(lexem, &letter, 1);
						letter = getchar();
					}
					ungetc(letter, stdin);
					//exponent
					//check if size sufficient
					if (strlen(lexem) >= (lex_size - 1)) {
						lex_size *= 2;
						p = realloc(lexem, sizeof(char) * lex_size);
						if (p == NULL) {
							free(lexem);
							new.kw = INTERNAL;
							new.i = INTERNAL_COMPILER_ERROR;
							return new;
						} else {
							lexem = p;
						}
					}
					if (letter == 'e' || letter == 'E') {
						strncat(lexem, &letter, 1);
						letter = getchar();
						if (letter == '+' || letter == '-') {
							//check if size sufficient
							if (strlen(lexem) >= (lex_size - 1)) {
								lex_size *= 2;
								p = realloc(lexem, sizeof(char) * lex_size);
								if (p == NULL) {
									free(lexem);
									new.kw = INTERNAL;
									new.i = INTERNAL_COMPILER_ERROR;
									return new;
								} else {
									lexem = p;
								}
							}
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
						if (!(letter >= '0' && letter <= '9')) {
							new.kw = LEXEM;
							new.i = LEXEM_ERROR;
							free(lexem);
							return new;
						}
						while (letter >= '0' && letter <= '9') {
							//check if size sufficient
							if (strlen(lexem) >= (lex_size - 1)) {
								lex_size *= 2;
								p = realloc(lexem, sizeof(char) * lex_size);
								if (p == NULL) {
									free(lexem);
									new.kw = INTERNAL;
									new.i = INTERNAL_COMPILER_ERROR;
									return new;
								} else {
									lexem = p;
								}
							}
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
						d = atof(lexem);
						new.kw = decim;
						new.f = d;
						p = malloc(sizeof(char) * (strlen(lexem) + 1));
						if (p == NULL) {
							free(lexem);
							new.kw = INTERNAL;
							new.i = INTERNAL_COMPILER_ERROR;
							return new;
						}
						strcpy(p, lexem);
						new.s = p;
						//printf("decim_%s_%f ", LGetStrAct(&new), LGetFloatAct(new));
						free(lexem);
						return new;
					} else {
						d = atof(lexem);
						new.kw = decim;
						new.f = d;
						p = malloc(sizeof(char) * (strlen(lexem) + 1));
						if (p == NULL) {
							free(lexem);
							new.kw = INTERNAL;
							new.i = INTERNAL_COMPILER_ERROR;
							return new;
						}
						strcpy(p, lexem);
						new.s = p;
						//printf("decim_%s_%f ", LGetStrAct(&new), LGetFloatAct(new));
						free(lexem);
						return new;
					}
				} else if (letter == 'e' || letter == 'E') {
					//exponent
					strncat(lexem, &letter, 1);
					letter = getchar();
					if (letter == '+' || letter == '-') {
						//check if size sufficient
						if (strlen(lexem) >= (lex_size - 1)) {
							lex_size *= 2;
							p = realloc(lexem, sizeof(char) * lex_size);
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							} else {
								lexem = p;
							}
						}
						strncat(lexem, &letter, 1);
						letter = getchar();
					}
					if (!(letter >= '0' && letter <= '9')) {
						new.kw = LEXEM;
						new.i = LEXEM_ERROR;
						free(lexem);
						return new;
					}
					while (letter >= '0' && letter <= '9') {
						//check if size sufficient
						if (strlen(lexem) >= (lex_size - 1)) {
							lex_size *= 2;
							p = realloc(lexem, sizeof(char) * lex_size);
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							} else {
								lexem = p;
							}
						}
						strncat(lexem, &letter, 1);
						letter = getchar();
					}
					d = atof(lexem);
					new.kw = decim;
					new.f = d;
					p = malloc(sizeof(char) * (strlen(lexem) + 1));
					if (p == NULL) {
						free(lexem);
						new.kw = INTERNAL;
						new.i = INTERNAL_COMPILER_ERROR;
						return new;
					}
					strcpy(p, lexem);
					new.s = p;
					//printf("decim_%s_%f ", LGetStrAct(&new), LGetFloatAct(new));
					free(lexem);
					return new;
				} else {
					//celá
					n = atoi(lexem);
					new.kw = num;
					new.i = n;
					p = malloc(sizeof(char) * (strlen(lexem) + 1));
					if (p == NULL) {
						free(lexem);
						new.kw = INTERNAL;
						new.i = INTERNAL_COMPILER_ERROR;
						return new;
					}
					strcpy(p, lexem);
					new.s = p;
					//printf("num_%s_%d ", LGetStrAct(&new), LGetNumAct(new));
					ungetc(letter, stdin);
					free(lexem);
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
						//printf("underscore ");
						ungetc(letter, stdin);
						free(lexem);
						return new;
						break;
					} else {
						strcat(lexem, "_");
					}
				}
				//identifikátory
				while ((letter >= '0' && letter <= '9') || (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || letter == '_') {
					//check if size sufficient
					if (strlen(lexem) >= (lex_size - 1)) {
						lex_size *= 2;
						p = realloc(lexem, sizeof(char) * lex_size);
						if (p == NULL) {
							free(lexem);
							new.kw = INTERNAL;
							new.i = INTERNAL_COMPILER_ERROR;
							return new;
						} else {
							lexem = p;
						}
					}
					strncat(lexem, &letter, 1);
					letter = getchar();
				}
				switch (lexem[0]) {
					case 'c':
						//const concat chr
						if (!strcmp(lexem, "const")) {
							new.kw = constant;
							//printf("konst ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'v':
						//var void
						if (!strcmp(lexem, "var")) {
							new.kw = variable;
							//printf("variable ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else if (!strcmp(lexem, "void")) {
							new.kw = dtvoid;
							//printf("dtvoid ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'r':
						//return readstr readi32 readf64
						if (!strcmp(lexem, "return")) {
							new.kw = _return;
							//printf("_return ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'p':
						//pub
						if (!strcmp(lexem, "pub")) {
							new.kw = _pub;
							//printf("_pub ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'u':
						//u8
						if (!strcmp(lexem, "u8")) {
							new.kw = dtstr;
							//printf("dtstr ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'm':
						//main
						if (!strcmp(lexem, "main")) {
							new.kw = _main;
							//printf("_main ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'f':
						//fn f64 f2i
						if (!strcmp(lexem, "fn")) {
							new.kw = _fn;
							//printf("_fn ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else if (!strcmp(lexem, "f64")) {
							new.kw = dtflt;
							//printf("dtflt ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'w':
						//while write
						if (!strcmp(lexem, "while")) {
							new.kw = _while;
							//printf("_while ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'e':
						//else
						if (!strcmp(lexem, "else")) {
							new.kw = _else;
							//printf("_else ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'i':
						//if i32 ifj import i2f
						if (!strcmp(lexem, "if")) {
							new.kw = _if;
							//printf("_if ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else if (!strcmp(lexem, "i32")) {
							new.kw = dtint;
							//printf("dtint ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else if (!strcmp(lexem, "ifj")) {
							new.kw = inbuild;
							//printf("inbuild ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else if (!strcmp(lexem, "import")) {
							new.kw = _import;
							//printf("_import ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'n':
						//null
						if (!strcmp(lexem, "null")) {
							new.kw = _null;
							//printf("_null ");
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else {
							//id
							new.kw = id;
							p = malloc(sizeof(char) * (strlen(lexem) + 1));
							if (p == NULL) {
								free(lexem);
								new.kw = INTERNAL;
								new.i = INTERNAL_COMPILER_ERROR;
								return new;
							}
							strcpy(p, lexem);
							new.s = p;
							//printf("id_%s ", LGetStrAct(&new));
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					default:
						//id
						new.kw = id;
						p = malloc(sizeof(char) * (strlen(lexem) + 1));
						if (p == NULL) {
							free(lexem);
							new.kw = INTERNAL;
							new.i = INTERNAL_COMPILER_ERROR;
							return new;
						}
						strcpy(p, lexem);
						new.s = p;
						//printf("id_%s ", LGetStrAct(&new));
						ungetc(letter, stdin);
						free(lexem);
						return new;
				}
				break;
			default:
				//chyba lexemu
				new.kw = LEXEM;
				new.i = LEXEM_ERROR;
				free(lexem);
				return new;
		}
	}
	new.kw = end;
	free(lexem);
	return new;
}