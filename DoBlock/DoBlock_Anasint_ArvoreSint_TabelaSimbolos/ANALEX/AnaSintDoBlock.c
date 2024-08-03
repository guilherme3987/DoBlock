#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "AnaLexDoBlock.c"
#include "AnaLexDoBlock.h"
#include "AnaSintDoBlock.h"
#include "TabSimb.h"
#include "TabSimb.c"

TabIdef tabela_idef;
int escopoAtual = 0;
// unsigned, pois esta comparando variáveis tipo assinado com uma variável de tipo não assinado 
void consome(int esperado) {
    if ((unsigned int)tk.cat == (unsigned int)esperado || (unsigned int)tk.codigo == (unsigned int)esperado) {
        tk = AnaLex(fd);
    } else {
        char errMsg[100];
        sprintf(errMsg, "Token inesperado. Esperado: %d, Encontrado: %d", esperado, tk.codigo);
        errorSint(contLinha, errMsg);
    }
}

void prog() {
    PrintNodo("Início do Programa", AVANCA);
    
    Iniciar_tabela();
    
    tk = AnaLex(fd);

    while (tk.cat == PAL_RESERV && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL)) {
        decl_list_var();
    }

    while (tk.cat == PAL_RESERV && tk.codigo == BLOCK) {
        decl_block_prot();
    }

    if (!(tk.cat == PAL_RESERV && tk.codigo == MAIN)) {
        errorSint(contLinha, "Declaracao de bloco main esperada.");
    }

    block_main();

    while (tk.cat == PAL_RESERV && tk.codigo == BLOCK) {
        block_def();
    }

    if (tk.cat != FIM_PROG) {
        errorSint(contLinha, "Fim do Arquivo Esperado");
    }

    printf("EXPRESSÃO OK\n");
    Imprimir_tabela();
    
    PrintNodo("Fim do Programa", RETROCEDE);
}

void decl_list_var() {
    PrintNodo("Declaração de Lista de Variáveis", AVANCA);
    
    if (tk.codigo == CONST) {
        consome(CONST);
    }
    tipo();
    decl_var();

    while (tk.codigo == VIRGULA) {
        consome(VIRGULA);
        decl_var();
    }
    
    PrintNodo("Fim da Declaração de Lista de Variáveis", RETROCEDE);
}

void decl_block_prot() {
    PrintNodo("Declaração de Bloco de Protótipo", AVANCA);
    
    consome(BLOCK);

    if (tk.codigo != MAIN) {
        escopoAtual++;
        consome(ID);

        if (tk.codigo == WITH) {
            consome(WITH);

            while (true) {
                if (tk.cat == FIM_PROG || tk.codigo == MAIN || tk.codigo == BLOCK) {
                    break;
                }

                if (tk.codigo == REFERENCIA) {
                    consome(REFERENCIA);
                }

                tipo();

                if (tk.codigo == ABRE_COL) {
                    consome(ABRE_COL);
                    consome(FECHA_COL);
                }

                if (tk.codigo == VIRGULA) {
                    consome(VIRGULA);
                }
            }
        }
        escopoAtual--;
    }
    
    PrintNodo("Fim da Declaração de Bloco de Protótipo", RETROCEDE);
}

void block_main() {
    PrintNodo("Início do Bloco Main", AVANCA);
    
    consome(MAIN);
    escopoAtual++;
    
    while (tk.cat == PAL_RESERV && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL)) {
        decl_list_var();
    }

    while (tk.codigo != ENDBLOCK) {
        cmd();
    }

    consome(ENDBLOCK);
    escopoAtual--;
    
    PrintNodo("Fim do Bloco Main", RETROCEDE);
}

void block_def() {
    PrintNodo("Início da Definição de Bloco", AVANCA);
    
    consome(BLOCK);
    escopoAtual++;  // Enter a new block scope
    consome(ID);

    if (tk.codigo == WITH) {
        consome(WITH);
        tipo();
        consome(ID);

        while (tk.codigo == ABRE_COL) {
            consome(ABRE_COL);

            if (tk.cat == CONST_INT || tk.cat == ID) {
                consome(tk.cat);
            }

            consome(FECHA_COL);
        }

        while (tk.codigo == VIRGULA) {
            consome(VIRGULA);
            tipo();
            consome(ID);

            if (tk.codigo == ABRE_COL) {
                consome(ABRE_COL);

                if (tk.cat == CONST_INT || tk.cat == ID) {
                    consome(tk.cat);
                }

                consome(FECHA_COL);
            }
        }
    }

    while (tk.cat == PAL_RESERV && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == REAL || tk.codigo == BOOL)) {
        decl_list_var();
    }

    while (tk.codigo != ENDBLOCK) {
        cmd();
    }

    consome(ENDBLOCK);
    escopoAtual--;  // Exit the block scope
    
    PrintNodo("Fim da Definição de Bloco", RETROCEDE);
}

void tipo() {
    PrintNodo("Tipo", AVANCA);
    
    if (tk.codigo == CHAR || tk.codigo == INT || tk.codigo == REAL || tk.codigo == BOOL) {
        consome(tk.codigo);
    } else {
        errorSint(contLinha, "Tipo invalido");
    }
    
    PrintNodo("Fim do Tipo", RETROCEDE);
}

void decl_var() {
    PrintNodo("Declaração de Variável", AVANCA);
    
    // Captura o lexema antes de consumir
    char lexema[TAM_MAX_LEXEMA];
    strncpy(lexema, tk.lexema, TAM_MAX_LEXEMA - 1);
    lexema[TAM_MAX_LEXEMA - 1] = '\0';

    if (Buscar_escopo(lexema, escopoAtual) != -1) {
        printf("[ERRO] Identificador '%s' já declarado no escopo atual.\n", lexema);
        errorSint(contLinha, "Identificador já declarado no escopo atual");
    }

    int tipo_var = tk.codigo;
    consome(ID);

    if (Insercao_tabela(lexema, escopoAtual, tipo_var, "var", false) == -1) {
        printf("[ERRO] Não foi possível inserir o identificador na tabela de símbolos.\n");
    }

    while (tk.codigo == ABRE_COL) {
        consome(ABRE_COL);

        if (tk.cat == CONST_INT || tk.cat == ID) {
            consome(tk.cat);
        }

        consome(FECHA_COL);
    }

    if (tk.codigo == ATRIBUICAO) {
        consome(ATRIBUICAO);

        if (tk.cat == CONST_INT || tk.cat == CONST_FLOAT || tk.cat == CONST_CHAR || tk.cat == LITERAL) {
            consome(tk.cat);
        } else if (tk.codigo == ABRE_CHAVE) {
            consome(ABRE_CHAVE);

            do {
                if (tk.cat == CONST_INT || tk.cat == CONST_FLOAT || tk.cat == CONST_CHAR || tk.cat == LITERAL) {
                    consome(tk.cat);
                }

                if (tk.codigo == VIRGULA) {
                    consome(VIRGULA);
                } else {
                    break;
                }
            } while (true);

            consome(FECHA_CHAVE);
        } else {
            errorSint(contLinha, "Valor esperado após '='.");
        }
    }
    
    PrintNodo("Fim da Declaração de Variável", RETROCEDE);
}

void atrib() {
    PrintNodo("Atribuição", AVANCA);
    
    consome(ID);
    while (tk.codigo == ABRE_COL) {
        consome(ABRE_COL);
        expr();
        consome(FECHA_COL);
    }
    consome(ATRIBUICAO);
    expr();
    
    PrintNodo("Fim da Atribuição", RETROCEDE);
}

void expr() {
    PrintNodo("Expressão", AVANCA);
    
    expr_simp();
    if (tk.cat == OP_RELAC && (tk.codigo == IGUALDADE || tk.codigo == DIFERENTE || tk.codigo == MENOR_IGUAL ||
                               tk.codigo == MENOR_QUE || tk.codigo == MAIOR_IGUAL || tk.codigo == MAIOR_QUE)) {
        op_rel();
        expr_simp();
    }
    
    PrintNodo("Fim da Expressão", RETROCEDE);
}

void expr_simp() {
    PrintNodo("Expressão Simples", AVANCA);
    
    termo();
    while (tk.cat == OP_ARIT && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO)) {
        op_arit();
        termo();
    }
    
    PrintNodo("Fim da Expressão Simples", RETROCEDE);
}

void termo() {
    PrintNodo("Termo", AVANCA);
    
    fator();
    while (tk.cat == OP_ARIT && (tk.codigo == MULTIPLICACAO || tk.codigo == DIVISAO)) {
        op_arit();
        fator();
    }
    
    PrintNodo("Fim do Termo", RETROCEDE);
}

void fator() {
    PrintNodo("Fator", AVANCA);
    
    if (tk.cat == CONST_INT || tk.cat == CONST_FLOAT || tk.cat == CONST_CHAR || tk.cat == LITERAL) {
        consome(tk.cat);
    } else if (tk.cat == ID) {
        consome(ID);
        if (tk.codigo == ABRE_COL) {
            consome(ABRE_COL);
            expr();
            consome(FECHA_COL);
        }
    } else if (tk.codigo == ABRE_PAR) {
        consome(ABRE_PAR);
        expr();
        consome(FECHA_PAR);
    } else {
        errorSint(contLinha, "Fator inválido.");
    }
    
    PrintNodo("Fim do Fator", RETROCEDE);
}

void op_arit() {
    PrintNodo("Operador Aritmético", AVANCA);
    
    if (tk.codigo == ADICAO || tk.codigo == SUBTRACAO || tk.codigo == MULTIPLICACAO || tk.codigo == DIVISAO) {
        consome(tk.codigo);
    } else {
        errorSint(contLinha, "Operador aritmético esperado.");
    }
    
    PrintNodo("Fim do Operador Aritmético", RETROCEDE);
}

void op_rel() {
    PrintNodo("Operador Relacional", AVANCA);
    
    if (tk.codigo == IGUALDADE || tk.codigo == DIFERENTE || tk.codigo == MENOR_IGUAL || tk.codigo == MENOR_QUE ||
        tk.codigo == MAIOR_IGUAL || tk.codigo == MAIOR_QUE) {
        consome(tk.codigo);
    } else {
        errorSint(contLinha, "Operador relacional esperado.");
    }
    
    PrintNodo("Fim do Operador Relacional", RETROCEDE);
}

void cmd() {
    PrintNodo("Início do Comando", AVANCA);

    if (tk.cat == PAL_RESERV) {
        switch (tk.codigo) {
            case DO:
                PrintNodo("Comando DO", AVANCA);
                consome(DO);
                if (tk.cat == ID) {
                    consome(ID);
                    if (tk.codigo == WITH) {
                        PrintNodo("Comando WITH", AVANCA);
                        consome(WITH);
                        consome(ID);
                        while (tk.codigo == VIRGULA) {
                            consome(VIRGULA);
                            consome(ID);
                        }
                        PrintNodo("Fim do Comando WITH", RETROCEDE);
                    }
                    if (tk.codigo == VARYING) {
                        PrintNodo("Comando VARYING", AVANCA);
                        consome(VARYING);
                        consome(ID);
                        consome(FROM);
                        expr();
                        if (tk.codigo == TO) {
                            consome(TO);
                        } else if (tk.codigo == DOWNTO) {
                            consome(DOWNTO);
                        } else {
                            errorSint(contLinha, "Esperado 'to' ou 'downto'");
                        }
                        expr();
                        PrintNodo("Fim do Comando VARYING", RETROCEDE);
                    } else if (tk.codigo == WHILE) {
                        PrintNodo("Comando WHILE", AVANCA);
                        consome(WHILE);
                        consome(ABRE_PAR);
                        expr();
                        consome(FECHA_PAR);
                        PrintNodo("Fim do Comando WHILE", RETROCEDE);
                    } else if (tk.codigo == FOR) {
                        PrintNodo("Comando FOR", AVANCA);
                        consome(FOR);
                        expr();
                        PrintNodo("Fim do Comando FOR", RETROCEDE);
                    }
                } else {
                    cmd();
                    if (tk.codigo == VARYING) {
                        PrintNodo("Comando VARYING", AVANCA);
                        consome(VARYING);
                        consome(ID);
                        consome(FROM);
                        expr();
                        if (tk.codigo == TO) {
                            consome(TO);
                        } else if (tk.codigo == DOWNTO) {
                            consome(DOWNTO);
                        } else {
                            errorSint(contLinha, "Esperado 'to' ou 'downto'");
                        }
                        expr();
                        PrintNodo("Fim do Comando VARYING", RETROCEDE);
                    } else if (tk.codigo == WHILE) {
                        PrintNodo("Comando WHILE", AVANCA);
                        consome(WHILE);
                        consome(ABRE_PAR);
                        expr();
                        consome(FECHA_PAR);
                        PrintNodo("Fim do Comando WHILE", RETROCEDE);
                    } else if (tk.codigo == FOR) {
                        PrintNodo("Comando FOR", AVANCA);
                        consome(FOR);
                        expr();
                        PrintNodo("Fim do Comando FOR", RETROCEDE);
                    }
                }
                PrintNodo("Fim do Comando DO", RETROCEDE);
                break;

            case IF:
                PrintNodo("Comando IF", AVANCA);
                consome(IF);
                consome(ABRE_PAR);
                expr();
                consome(FECHA_PAR);

                if (tk.codigo == ABRE_CHAVE) {
                    PrintNodo("Bloco IF", AVANCA);
                    consome(ABRE_CHAVE);
                    cmd();
                    consome(FECHA_CHAVE);
                    PrintNodo("Fim do Bloco IF", RETROCEDE);
                } else {
                    cmd();
                }

                while (tk.codigo == ELSEIF) {
                    PrintNodo("Comando ELSEIF", AVANCA);
                    if (tk.codigo == ABRE_CHAVE) {
                        consome(ELSEIF);
                        consome(ABRE_PAR);
                        expr();
                        consome(FECHA_PAR);
                        consome(ABRE_CHAVE);
                        cmd();
                        consome(FECHA_CHAVE);
                        PrintNodo("Fim do Comando ELSEIF", RETROCEDE);
                    } else {
                        cmd();
                    }
                }

                if (tk.codigo == ELSE) {
                    PrintNodo("Comando ELSE", AVANCA);
                    consome(ELSE);
                    if (tk.codigo == ABRE_CHAVE) {
                        consome(ABRE_CHAVE);
                        cmd();
                        consome(FECHA_CHAVE);
                        PrintNodo("Fim do Comando ELSE", RETROCEDE);
                    } else {
                        cmd();
                    }
                }

                consome(ENDIF);
                PrintNodo("Fim do Comando IF", RETROCEDE);
                break;

            case WHILE:
                PrintNodo("Comando WHILE", AVANCA);
                consome(WHILE);
                consome(ABRE_PAR);
                expr();
                consome(FECHA_PAR);
                consome(ABRE_CHAVE);
                cmd();
                consome(FECHA_CHAVE);
                consome(ENDWHILE);
                PrintNodo("Fim do Comando WHILE", RETROCEDE);
                break;

            case GOBACK:
                PrintNodo("Comando GOBACK", AVANCA);
                consome(GOBACK);
                PrintNodo("Fim do Comando GOBACK", RETROCEDE);
                break;

            case GETINT:
                PrintNodo("Comando GETINT", AVANCA);
                consome(GETINT);
                consome(ID);
                PrintNodo("Fim do Comando GETINT", RETROCEDE);
                break;

            case GETREAL:
                PrintNodo("Comando GETREAL", AVANCA);
                consome(GETREAL);
                consome(ID);
                PrintNodo("Fim do Comando GETREAL", RETROCEDE);
                break;

            case GETCHAR:
                PrintNodo("Comando GETCHAR", AVANCA);
                consome(GETCHAR);
                consome(ID);
                PrintNodo("Fim do Comando GETCHAR", RETROCEDE);
                break;

            case PUTINT:
                PrintNodo("Comando PUTINT", AVANCA);
                consome(PUTINT);
                consome(ID);
                PrintNodo("Fim do Comando PUTINT", RETROCEDE);
                break;

            case PUTREAL:
                PrintNodo("Comando PUTREAL", AVANCA);
                consome(PUTREAL);
                consome(ID);
                PrintNodo("Fim do Comando PUTREAL", RETROCEDE);
                break;

            case PUTCHAR:
                PrintNodo("Comando PUTCHAR", AVANCA);
                consome(PUTCHAR);
                consome(ID);
                PrintNodo("Fim do Comando PUTCHAR", RETROCEDE);
                break;

            default:
                errorSint(contLinha, "Comando inválido.");
        }
    } else if (tk.cat == ID) {
        atrib();
    } else if (tk.cat != FIM_PROG) {
        errorSint(contLinha, "Comando inválido.");
    }

    PrintNodo("Fim do Comando", RETROCEDE);
}