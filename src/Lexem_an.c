/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains implementation of a function used for lexical analysis
 *  @file  Lexem_an.c
 *  @author Ivo Puchnar, xpuchn02
 *  @brief Implementation file for lexical analysis
 */

#include "errors_enum.h"
#include "tokens.h"

Token get_token() {
	//initialization
	int n;											//variable for storing the obtained integer
	double d;										//variable for storing the obtained double
	size_t lex_size = 8;							//current length of lexem
	Token new = {next, 0, 0, NULL};					//default token before modifications
	char letter = getchar();						//last character

	char *p;										//support pointer for reallocation
	char *lexem = malloc(sizeof(char) * lex_size);	//current lexeme
	if (lexem == NULL) {
		new.kw = INTERNAL;
		new.i = INTERNAL_COMPILER_ERROR;
		return new;
	}
	lexem[0] = '\0';

	//Body of function
	while (letter != EOF) {
		switch (letter) {
			//white characters
			case ' ':
			case '\n':
			case '\t':
			case '\r':
				letter = getchar();
				break;
			//single-character tokens
			case ';':
				free(lexem);
				return new;
				break;
			case ':':
				new.kw = colon;
				free(lexem);
				return new;
				break;
			case '(':
				new.kw = lbracket;
				free(lexem);
				return new;
				break;
			case ')':
				new.kw = rbracket;
				free(lexem);
				return new;
				break;
			case '{':
				new.kw = lblock;
				free(lexem);
				return new;
				break;
			case '}':
				new.kw = rblock;
				free(lexem);
				return new;
				break;
			case ',':
				new.kw = comma;
				free(lexem);
				return new;
				break;
			case '.':
				new.kw = dot;
				free(lexem);
				return new;
				break;
			case '+':
				new.kw = plus;
				free(lexem);
				return new;
				break;
			case '-':
				new.kw = minus;
				free(lexem);
				return new;
				break;
			case '*':
				new.kw = multiply;
				free(lexem);
				return new;
				break;
			case '?':
				new.kw = question_mark;
				free(lexem);
				return new;
				break;
			case '|':
				new.kw = vertical_bar;
				free(lexem);
				return new;
				break;
			case '@':
				new.kw = at;
				free(lexem);
				return new;
				break;
			//two-character tokens
			case '[':
				letter = getchar();
				if (letter == ']') {
					new.kw = square_brackets;
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
					free(lexem);
					return new;
				} else {
					new.kw = less;
					ungetc(letter, stdin);
					free(lexem);
					return new;
				}
				break;
			case '>':
				letter = getchar();
				if (letter == '=') {
					new.kw = mequal;
					free(lexem);
					return new;
				} else {
					new.kw = more;
					ungetc(letter, stdin);
					free(lexem);
					return new;
				}
				break;
			case '!':
				letter = getchar();
				if (letter == '=') {
					new.kw = nequal;
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
					ungetc(letter, stdin);
					free(lexem);
					return new;
				} else {
					new.kw = compare_equal;
					free(lexem);
					return new;
				}
				break;
			case '/':
				letter = getchar();
				//comments
				if (letter == '/') {
					letter = getchar();
					while (letter != '\n' && letter != EOF) {
						letter = getchar();
					}
				} else {
					new.kw = division;
					ungetc(letter, stdin);
					free(lexem);
					return new;
				}
				break;
			//multi-line strings
			case '\\':
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
						//add character
						if (letter > 31) {
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
					}
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
					strncat(lexem, "\n\n", 1);
					letter = getchar();
					//skips whitespace on newline
					while (letter == ' ' || letter == '\t' || letter == '\n' || letter == '\r') {
						letter = getchar();
					}
				} while (letter == '\\');
				//deletes the last \n
				lexem[strlen(lexem)-1]='\0';
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
				ungetc(letter, stdin);
				free(lexem);
				return new;
				break;
			//line string
			case '"':
				letter = getchar();
				//empty string
				if (letter == '"') {
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
					free(lexem);
					return new;
				//non-empty string
				} else {
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
						//escape sequence
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
									//hexadecimal
									let[0] = getchar();
									let[1] = getchar();
									let[2] = '\0';
									if (((let[0] >= '0' && let[0] <= '9') || (let[0] >= 'a' && let[0] <= 'f') || (let[0] >= 'A' && let[0] <= 'F')) && ((let[1] >= '0' && let[1] <= '9') || (let[1] >= 'a' && let[1] <= 'f') || (let[1] >= 'A' && let[1] <= 'F'))) {
										sscanf(let, "%X", &y);
										letter = y;
										strncat(lexem, &letter, 1);
										letter = getchar();
									//lexeme error
									} else {
										new.kw = LEXEM;
										new.i = LEXEM_ERROR;
										free(lexem);
										return new;
									}
									break;
								//lexeme error
								default:
									new.kw = LEXEM;
									new.i = LEXEM_ERROR;
									free(lexem);
									return new;
							}
						//add character
						} else if (letter > 31) {
							strncat(lexem, &letter, 1);
							letter = getchar();
						}
					}
					//lexeme error
					if (letter == EOF || letter == '\n') {
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
					free(lexem);
					return new;
				}
				break;
			case '0':
				letter = getchar();
				//does not start with zero
				if (letter >= '0' && letter <= '9') {
					new.kw = LEXEM;
					new.i = LEXEM_ERROR;
					free(lexem);
					return new;
				}
				strncat(lexem, "00", 1);
			case '1' ... '9':
				//whole numbers
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
				//decimal numbers
				if (letter == '.') {
					strncat(lexem, &letter, 1);
					letter = getchar();
					//error if not digit
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
					//exponent
					if (letter == 'e' || letter == 'E') {
						strncat(lexem, &letter, 1);
						letter = getchar();
						//possible sign
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
						//error if not digit
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
						free(lexem);
						return new;
					//decimal numbers
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
						free(lexem);
						return new;
					}
				//exponent
				} else if (letter == 'e' || letter == 'E') {
					strncat(lexem, &letter, 1);
					letter = getchar();
					//possible sign
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
					//error if not digit
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
					free(lexem);
					return new;
				//whole numbers
				} else {
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
					ungetc(letter, stdin);
					free(lexem);
					return new;
				}
				break;
			case 'a' ... 'z':
			case 'A' ... 'Z':
			case '_':
				lexem[0] = '\0';
				//singular underscore
				if (letter == '_') {
					letter = getchar();
					if (!(letter >= 'a' && letter <= 'z') && !(letter >= 'A' && letter <= 'Z') && !(letter >= '0' && letter <= '9') && letter != '_') {
						new.kw = underscore;
						ungetc(letter, stdin);
						free(lexem);
						return new;
						break;
					} else {
						strcat(lexem, "_");
					}
				}
				//identifiers and keywords
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
						//const
						if (!strcmp(lexem, "const")) {
							new.kw = constant;
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
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'v':
						//var void
						if (!strcmp(lexem, "var")) {
							new.kw = variable;
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else if (!strcmp(lexem, "void")) {
							new.kw = dtvoid;
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
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'r':
						//return
						if (!strcmp(lexem, "return")) {
							new.kw = _return;
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
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'p':
						//pub
						if (!strcmp(lexem, "pub")) {
							new.kw = _pub;
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
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'u':
						//u8
						if (!strcmp(lexem, "u8")) {
							new.kw = dtstr;
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
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'm':
						//main
						if (!strcmp(lexem, "main")) {
							new.kw = _main;
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
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'f':
						//fn f64
						if (!strcmp(lexem, "fn")) {
							new.kw = _fn;
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else if (!strcmp(lexem, "f64")) {
							new.kw = dtflt;
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
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'w':
						//while
						if (!strcmp(lexem, "while")) {
							new.kw = _while;
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
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'e':
						//else
						if (!strcmp(lexem, "else")) {
							new.kw = _else;
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
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'i':
						//if i32 ifj import
						if (!strcmp(lexem, "if")) {
							new.kw = _if;
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else if (!strcmp(lexem, "i32")) {
							new.kw = dtint;
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else if (!strcmp(lexem, "ifj")) {
							new.kw = inbuild;
							ungetc(letter, stdin);
							free(lexem);
							return new;
						} else if (!strcmp(lexem, "import")) {
							new.kw = _import;
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
							ungetc(letter, stdin);
							free(lexem);
							return new;
						}
						break;
					case 'n':
						//null
						if (!strcmp(lexem, "null")) {
							new.kw = _null;
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
						ungetc(letter, stdin);
						free(lexem);
						return new;
				}
				break;
			default:
				//lexeme error
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