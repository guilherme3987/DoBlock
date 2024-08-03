#ifndef TABSIMB
#define TABSIMB

#include <stdio.h>
#include <stdlib.h>
#include "AnaLexDoBlock.h"
#include <stdbool.h>

#define TAM_MAX_TAB 100
#define TAM_CATEGORIA 4

// Definição da estrutura TabSimb
typedef struct
{
    char lexema[TAM_MAX_LEXEMA];
    int tipo;
    int escopo;
    char categoria[TAM_CATEGORIA];
    bool zombie;
    int endereco;
} TabSimb;

// Definição da estrutura TabIdef
typedef struct
{
    int tamTab;
    TabSimb tabela_simb[TAM_MAX_TAB];
} TabIdef;

// Declaração de variáveis externas
extern TabIdef tabela_idef;
extern TOKEN tk;
extern int escopoAtual;

// Declaração de funções
void Iniciar_tabela();
void Imprimir_tabela();
int Buscar_tabela(const char lexema[]);
int Buscar_escopo(char lexema[], int escopo);
int Insercao_tabela(char lexema[], int escopo, int tipo, char categoria[TAM_CATEGORIA], bool zombie);
int Remover_ultimo();

#endif