#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "AnaLexDoBlock.h"
#include "FuncAuxDoBlock.h"

#define TAM_LEXEMA 50
#define TAM_LITERAL 50
#define TAM_NUM 20

int indiceLiteral = 0;
char tableLit[TAM_LITERAL][TAM_LEXEMA] = {""};
int lti = 0;
int contLinha = 1;

TOKEN AnaLex(FILE *fd)
{
    int estado = 0;
    int tamL = 0;
    int tamD = 0;
    char lexema[TAM_LEXEMA] = "";
    char digitos[TAM_NUM] = "";

    char c;

    TOKEN token;

    while (1)
    {
        c = fgetc(fd);

        switch (estado)
        {
        case 0:
            if (c == ' ' || c == '\t')
            {
                estado = 0;
            }
            else if (c == '\'')
            {
                estado = 9;
                lexema[tamL++] = c;

                lexema[tamL] = '\0';
            }
            else if (c == '\"')
            {
                estado = 15;
            }
            else if (c == '/')
            {
                estado = 18;
            }
            else if (c == '=')
            {
                estado = 21;
            }
            else if ((c == '_') || isalpha(c))
            {
                estado = 1;
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
            }
            else if (isdigit(c))
            {
                estado = 4;
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            }
            else if (c == '+')
            {
                estado = 35;
                token.cat = OP_ARIT;
                token.codigo = ADICAO;
                return token;
            }
            else if (c == '-')
            {
                estado = 36;
                token.cat = OP_ARIT;
                token.codigo = SUBTRACAO;
                return token;
            }
            else if (c == '*')
            {
                estado = 37;
                token.cat = OP_ARIT;
                token.codigo = MULTIPLICACAO;
                return token;
            }
            else if (c == '!')
            {
                estado = 24;
                token.cat = OP_RELAC;
            }
            else if (c == '|')
            {
                estado = 42;
                token.cat = OP_LOGIC;
            }
            else if (c == '&')
            {
                estado = 39;
            }
            else if (c == '>')
            {
                estado = 30;
                token.cat = OP_RELAC;
            }
            else if (c == '<')
            {
                estado = 27;
                token.cat = OP_RELAC;
            }
            else if (c == '{')
            {
                estado = 33;
                token.cat = SINAL;
                token.codigo = ABRE_CHAVE;
                return token;
            }
            else if (c == '}')
            {
                estado = 34;
                token.cat = SINAL;
                token.codigo = FECHA_CHAVE;
                return token;
            }
            else if (c == '(')
            {
                estado = 44;
                token.cat = SINAL;
                token.codigo = ABRE_PAR;
                return token;
            }
            else if (c == ')')
            {
                estado = 45;
                token.cat = SINAL;
                token.codigo = FECHA_PAR;
                return token;
            }
            else if (c == ',')
            {
                estado = 52;
                token.cat = SINAL;
                token.codigo = VIRGULA;
                return token;
            }
            else if (c == '[')
            {
                estado = 46;
                token.cat = SINAL;
                token.codigo = ABRE_COL;
                return token;
            }
            else if (c == ']')
            {
                estado = 47;
                token.cat = SINAL;
                token.codigo = FECHA_COL;
                return token;
            }
            else if (c == '\n')
            {
                contLinha++;
                estado = 0;
            }
            else if (c == EOF)
            {
                token.cat = FIM_PROG;
                contLinha = 1;
                return token;
            }
            else
            {
                errorLex(contLinha, c);
            }
            break;
        case 1:
            if (c == '_' || isalpha(c) || isdigit(c))
            {
                estado = 1;
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
            }
            else
            {
                estado = 3;

                ungetc(c, fd);

                strcpy(token.lexema, lexema);

                // verificacao de palavras reservadas
                if (strcmp("main", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = MAIN;
                }
                else if (strcmp("block", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = BLOCK;
                } // e etc..
                else if (strcmp("endblock", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = ENDBLOCK;
                }
                else if (strcmp("const", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = CONST;
                }
                else if (strcmp("char", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = CHAR;
                }
                else if (strcmp("int", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = INT;
                }
                else if (strcmp("real", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = REAL;
                } 
                else if (strcmp("bool", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = BOOL;
                }
                else if (strcmp("with", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = WITH;
                }
                else if (strcmp("do", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = DO;
                }
                else if (strcmp("varying", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = VARYING;
                }
                else if (strcmp("from", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = FROM;
                }
                else if (strcmp("to", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = TO;
                }
                else if (strcmp("downto", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = DOWNTO;
                }
                else if (strcmp("while", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = WHILE;
                }
                else if (strcmp("endwhile", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = ENDWHILE;
                }
                else if (strcmp("for", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = FOR;
                }
                else if (strcmp("if", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = IF;
                }
                else if (strcmp("elseif", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = ELSEIF;
                }
                else if (strcmp("else", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = ELSE;
                }
                else if (strcmp("endif", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = ENDIF;
                }
                else if (strcmp("goback", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = GOBACK;
                }
                else if (strcmp("getint", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = GETINT;
                }
                else if (strcmp("getreal", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = GETREAL;
                }
                else if (strcmp("getchar", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = GETCHAR;
                }
                else if (strcmp("putint", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = PUTINT;
                }
                else if (strcmp("putreal", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = PUTREAL;
                }
                else if (strcmp("putchar", lexema) == 0)
                {
                    token.cat = PAL_RESERV;
                    token.codigo = PUTCHAR;
                }
                else
                {
                    token.cat = ID;
                    strcpy(token.lexema, lexema);
                }

                return token;
            }
            break;
        case 4:
            if (isdigit(c))
            {
                estado = 4;
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            }
            else if (c == '.')
            {
                estado = 7;
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            }
            else if (c == '_' || isalpha(c))
            {
                errorLex(contLinha, c);
            }
            else
            {
                estado = 5;
                ungetc(c, fd);

                token.cat = CONST_INT;
                token.valInt = atoi(digitos);

                return token;
            }
            break;
        case 7:
            if (isdigit(c))
            {
                estado = 7;

                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            }
            else
            {
                estado = 8;

                ungetc(c, fd);

                token.cat = CONST_FLOAT;
                token.valFloat = atof(digitos);

                return token;
            }
            break;
        case 9:
            if (c == '\'')
            {
                errorLex(contLinha, c);
            }
            else if (c == '\\')
            {
                estado = 12;
            }
            else if (isprint(c))
            {
                estado = 10;
                lexema[tamL++] = c;
                lexema[tamL] = '\0';
            }
            else
            {
                errorLex(contLinha, c);
            }
            break;
        case 10:
            if (c == '\'')
            {
                estado = 0;

                token.cat = CONST_CHAR;

                strcpy(token.lexema, lexema);

                return token;
            }
            else
            {
                errorLex(contLinha, c);
            }
            break;
        case 12:
            if (c == 'n')
            {
                estado = 10;
                strcpy(lexema, "enter");
            }
            else if (c == '0')
            {
                estado = 10;

                strcpy(lexema, "null");
            }
            else
            {
                errorLex(contLinha, c);
            }
            break;
        case 15:
            if (c == '\"')
            {
                if (tamL == 0)
                {
                    errorLex(contLinha, c);
                }

                estado = 0;

                token.cat = LITERAL;
                strcpy(token.lexema, lexema);

                return token;
            }
            else if (isprint(c))
            {
                if (tamL < TAM_LEXEMA - 1)
                {
                    lexema[tamL++] = c;
                    lexema[tamL] = '\0';
                }
                else
                {
                    errorLex(contLinha, c);
                }
            }
            else if (c == '\n')
            {
                errorLex(contLinha, c);
            }
            break;
        case 18:
            if (c == '/')
            {
                estado = 19;

                lexema[tamL++] = c;
                lexema[tamL] = '\0';
            }
            else
            {
                estado = 38;
                ungetc(c, fd);

                token.cat = OP_ARIT;
                token.codigo = DIVISAO;

                return token;
            }
            break;
        case 19:
            if (c == '\n')
            {
                estado = 0; // Voltar ao estado inicial
            }
            else
            {
                // Continuar lendo o comentário
                lexema[tamL++] = c;
            }
            break;
        case 21:
            if (c == '=')
            {
                estado = 22;

                token.cat = OP_RELAC;
                token.codigo = IGUALDADE;

                return token;
            }
            else
            {
                estado = 23;

                ungetc(c, fd);

                token.cat = OP_ARIT;
                token.codigo = ATRIBUICAO;

                return token;
            }
            break;
        case 24:
            if (c == '=')
            {
                estado = 0;

                token.codigo = DIFERENTE;

                return token;
            }
            else
            {
                estado = 0;
                ungetc(c, fd);

                token.codigo = NOT_LOGIC;

                return token;
            }
            break;
        case 27:
            if (c == '=')
            {
                estado = 28;

                token.codigo = MENOR_IGUAL;

                return token;
            }
            else
            {
                estado = 0;
                ungetc(c, fd);

                token.codigo = MENOR_QUE;

                return token;
            }
            break;
        case 30:
            if (c == '=')
            {
                estado = 31;

                token.codigo = MAIOR_IGUAL;

                return token;
            }
            else
            {
                estado = 0;
                ungetc(c, fd);

                token.codigo = MAIOR_QUE;

                return token;
            }
            break;
        case 39:
            if (c == '&')
            {
                estado = 40;

                token.cat = OP_LOGIC;

                token.codigo = AND_LOGIC;

                return token;
            }
            else
            {
                estado = 41;

                token.cat = SINAL;

                ungetc(c, fd);

                token.codigo = REFERENCIA;

                return token;
            }
            break;
        case 42:
            if (c == '|')
            {
                estado = 43;

                token.cat = OP_LOGIC;

                token.codigo = OR_LOGIC;

                return token;
            }
            else
            {
                errorLex(contLinha, c);
            }
            break;
        }
    }
}