#include "TabSimb.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// Inicializa a tabela de símbolos
void Iniciar_tabela() {
    tabela_idef.tamTab = 0;
}

// Imprime a tabela de símbolos
void Imprimir_tabela() {
    printf("\nTabela de Símbolos:\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("| %-6s | %-10s | %-10s | %-6s | %-10s | %-6s | %-8s |\n", 
           "Índice", "Lexema", "Tipo", "Escopo", "Categoria", "Zombie", "Endereço");
    printf("------------------------------------------------------------------------------------\n");

    for (int i = 0; i < tabela_idef.tamTab; i++) {
        printf("| %-6d | %-10s | %-10d | %-6d | %-10s | %-6d | %-8d |\n",
               i,
               tabela_idef.tabela_simb[i].lexema,
               tabela_idef.tabela_simb[i].tipo,
               tabela_idef.tabela_simb[i].escopo,
               tabela_idef.tabela_simb[i].categoria,
               tabela_idef.tabela_simb[i].zombie,
               tabela_idef.tabela_simb[i].endereco);
    }
    printf("------------------------------------------------------------------------------------\n");
}


// Busca um lexema na tabela de símbolos
int Buscar_tabela(const char lexema[]) {
    for (int i = 0; i < tabela_idef.tamTab; i++) {
        if (strcmp(tabela_idef.tabela_simb[i].lexema, lexema) == 0) {
            return i;  // Retorna o índice se encontrar o lexema
        }
    }
    return -1;  // Retorna -1 se não encontrar o lexema
}

// Busca um lexema em um escopo específico
int Buscar_escopo(char lexema[], int escopo) {
    for (int i = 0; i < tabela_idef.tamTab; i++) {
        if (strcmp(tabela_idef.tabela_simb[i].lexema, lexema) == 0 && tabela_idef.tabela_simb[i].escopo == escopo) {
            return i;
        }
    }
    return -1;
}

// Insere um novo símbolo na tabela de símbolos e testa se já foi declarado
int Insercao_tabela(char lexema[], int escopo, int tipo, char categoria[], bool zombie) {
    if (tabela_idef.tamTab >= TAM_MAX_TAB) {
        printf("[ERRO] Tabela de símbolos cheia.\n");
        return -1;
    }

    int pos = Buscar_escopo(lexema, escopo);
    if (pos != -1) {
        printf("[ERRO] Identificador '%s' já declarado no escopo atual.\n", lexema);
        return -1;
    }

    TabSimb novoSimbolo;
    strncpy(novoSimbolo.lexema, lexema, TAM_MAX_LEXEMA - 1);
    novoSimbolo.lexema[TAM_MAX_LEXEMA - 1] = '\0'; // Assegura que a string esteja terminada em null
    novoSimbolo.escopo = escopo;
    novoSimbolo.tipo = tipo;
    strncpy(novoSimbolo.categoria, categoria, TAM_CATEGORIA - 1);
    novoSimbolo.categoria[TAM_CATEGORIA - 1] = '\0'; // Assegura que a string esteja terminada em null
    novoSimbolo.zombie = zombie;
    novoSimbolo.endereco = tabela_idef.tamTab;

    tabela_idef.tabela_simb[tabela_idef.tamTab++] = novoSimbolo;
    return 0;
}

// Remove o último símbolo inserido na tabela de símbolos
int Remover_ultimo() {
    if (tabela_idef.tamTab > 0) {
        tabela_idef.tamTab--;
        return 0;
    } else {
        printf("[ERRO] Tabela de símbolos vazia.\n");
        return -1;
    }
}
