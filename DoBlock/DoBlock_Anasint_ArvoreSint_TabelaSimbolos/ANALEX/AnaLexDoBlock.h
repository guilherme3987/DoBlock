#include <stdio.h>

#ifndef ANALEXDOBLOCK_H

#define ANALEXDOBLOCK_H

#define TAM_MAX_LEXEMA 31

enum TOKEN_CAT
{
    ID = 1,      // 1
    PAL_RESERV,  // 2
    SINAL,       // 3
    CONST_CHAR,  // 4
    CONST_FLOAT, // 5
    CONST_INT,   // 6
    ID_CONST,    // 7
    LITERAL,     // 8
    OP_ARIT,     // 9
    OP_LOGIC,    // 10
    OP_RELAC,    // 11
    FIM_EXPR,    // 12
    FIM_PROG     // 13
};

enum PAL_RESERV
{
    MAIN = 1, // 1
    BLOCK,    // 2
    ENDBLOCK, // 3
    CONST,    // 4
    CHAR,     // 5
    INT,      // 6
    REAL,     // 7
    BOOL,     // 8
    WITH,     // 9
    DO,       // 10
    VARYING,  // 11
    FROM,     // 12
    TO,       // 13
    DOWNTO,   // 14
    WHILE,    // 15
    ENDWHILE, // 16
    FOR,      // 17
    IF,       // 18
    ELSEIF,   // 19
    ELSE,     // 20
    ENDIF,    // 21
    GOBACK,   // 22
    GETINT,   // 23
    GETCHAR,  // 24
    GETREAL,  // 25
    PUTINT,   // 26
    PUTREAL,  // 27
    PUTCHAR   // 28
}; // e etc..

// Palavras reservadas da linguagem

enum SINAIS
{
    ABRE_PAR = 1, // 1
    FECHA_PAR,    // 2
    ABRE_CHAVE,   // 3
    FECHA_CHAVE,  // 4
    ABRE_COL,     // 5
    FECHA_COL,    // 6
    VIRGULA,      // 7
    REFERENCIA    // 8
};
// Sinais válidos da linguagem

enum OP_ARIT
{
    ATRIBUICAO = 1, // =
    ADICAO,         // +
    SUBTRACAO,      // -
    MULTIPLICACAO,  // *
    DIVISAO         // /
};

enum OP_RELAC
{
    IGUALDADE = 1, // ==
    DIFERENTE,     // !=
    MENOR_IGUAL,   // <=
    MAIOR_IGUAL,   // >=
    MENOR_QUE,     // <
    MAIOR_QUE      // >
};

enum OP_LOGIC
{
    AND_LOGIC = 1, // &&
    OR_LOGIC,      // ||
    NOT_LOGIC      // !
};

typedef struct
{
    enum TOKEN_CAT cat; // deve receber uma das constantes de enum TOKEN_CAT
    union
    {                                // parte variável do registro
        int codigo;                  // para tokens das categorias SINAL
        int indice;                  // indica o índice da tabela onde o Analex armazenou um literal(cadeira de caracter) para tokens da cat
        char lexema[TAM_MAX_LEXEMA]; // cadeia de caractares que corresponde o nome do token da cat. ID
        int valInt;                  // valor da constante inteira em tokens da cat. CONST_INT
        float valFloat;              // valor da constante flutuante em tokens da cat. CONST_FLOAT
        char caractere;              // caracter do token da cat. CONST_CHAR
    };
} TOKEN; // Tipo TOKEN

// Variáveis globais
extern TOKEN tk;
extern FILE *fd;
TOKEN AnaLex(FILE *);

#endif