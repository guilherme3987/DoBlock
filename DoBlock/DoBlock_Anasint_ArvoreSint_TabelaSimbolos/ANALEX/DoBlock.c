#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "AnaSintDoBlock.c"
#include "FuncAuxDoBlock.c"
#include "AnaSintDoBlock.h"
#include "FuncAuxDoBlock.h"
#include "Processador_tokens.c"

FILE *fd;
TOKEN tk;

char TABS[200] = "";

void Analisador_lexico(FILE *fd)
{
    printf("\n[========== INICIO - Analise lexica ==========]\n");
    processador_tokens(fd);
    printf("\n[========== FIM - Analise lexica ==========]\n");
}

void Analisador_sintatico()
{
    // Iniciar_tabela();
    printf("\n[========== INICIO - Analise sintatica ==========]\n");
    prog();
    printf("\n[========== FIM - Analise sintatica ==========]\n");
}






int main()
{

    fd = fopen("C:/Users/guilh/OneDrive/Documentos/compilador_DoBlock/ANALEX/teste.dbk", "r");

    if (fd == NULL)
    {
        fprintf(stderr, "\nError ao abrir o arquivo.\n");
        return 1;
    }

    // Analisador_lexico(fd);
    
    Analisador_sintatico();


    fclose(fd);

    return 0;
}
