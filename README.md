# README - Analisador Léxico e Sintático

## Regras Léxicas

Este analisador possui as seguintes regras léxicas para identificar e classificar tokens no código-fonte:

- **letra**: `a | b | ... | z | A | B | ... | Z`  
  Denota uma letra do alfabeto.

- **digito**: `0 | 1 | ... | 9`  
  Denota um dígito numérico.

- **id**: `{_} letra { letra | digito | _ }`  
  Identificadores que começam com um caractere de sublinhado (`_`) ou uma letra, seguidos por letras, dígitos ou sublinhados.

- **intcon**: `digito { digito }`  
  Constante inteira composta por um ou mais dígitos.

- **realcon**: `intcon.intcon`  
  Constante real, com parte inteira e decimal separadas por um ponto.

- **charcon**: `'ch' | '\n' | '\0'`  
  Constantes de caractere, onde `ch` é qualquer caractere imprimível da tabela ASCII, exceto `\` (barra invertida) e `'` (apóstrofo).

- **stringcon**: `"{ch}"`  
  Constantes de string, onde `ch` é qualquer caractere imprimível da tabela ASCII, exceto `"` (aspas) e o caractere newline.

- **idconst**: Denota a ocorrência de uma constante inteira declarada e inicializada previamente com `const int`.

- **comentário**: Comentários iniciam com `//` e se estendem até o fim da linha.

## Regras Sintáticas

As regras sintáticas descrevem a estrutura e a formação de declarações e comandos na linguagem. Símbolos não-terminais são apresentados em itálico, e símbolos terminais são apresentados em negrito.

### Estrutura do Programa

- **prog**: `{decl_list_var} {decl_block_prot}  block_main {block_def}`  
  Define a estrutura geral do programa.

### Declarações e Comandos

- **decl_list_var**: `[const] tipo decl_var { , decl_var }`  
  Declara variáveis, com a possibilidade de declaração de constantes.

- **decl_block_prot**: `block id [with [&] tipo { [ ] } { , [&] tipo { [ ] } }]`  
  Declaração de um bloco com seu identificador e tipos associados.

- **block_main**: `block main { decl_list_var } { cmd } endblock`  
  Declara o bloco principal do programa, incluindo variáveis e comandos.

### Declaração de Variáveis

- **decl_var**: `id {[ intcon | idconst ]} [ = (intcon | realcon | charcon | stringcon | { (intcon | realcon | charcon | stringcon) {, (intcon | realcon | charcon | stringcon) } } ) ]`  
  Declara variáveis com possíveis valores iniciais.

- **tipo**: `char | int | bool | real`  
  Tipos de dados suportados: caractere, inteiro, booleano e real.

### Definição de Blocos

- **block_def**: `block id [with tipo id1{ [intcon1 | idconst1] } { , tipo  id2 { [intcon2 | idconst2] } }] { decl_list_var } { cmd } endblock`  
  Definição de blocos com identificadores e tipos.

### Comandos

- **cmd**: 
  - `do (id [with id1 { , id2 }] | cmd ) varying idx from expr1 (to | downto) expr2`
  - `do (id [with id1 { , id2 }] | cmd ) while ( expr )`
  - `do (id [with id1 { , id2 }] | cmd ) [for expr times ]`
  - `if ( expr ) { cmd }{ elseif ( expr ) { cmd } } [ else { cmd }] endif`
  - `while ( expr ) { cmd } endwhile`
  - `atrib`
  - `goback`
  - `getint id | getreal id | getchar id`
  - `putint id | putreal id | putchar id`

- **atrib**: `id { [ expr ] } = expr`  
  Atribuição de valores a variáveis.

### Expressões

- **expr**: `expr_simp [ op_rel  expr_simp ]`  
  Expressões que podem incluir operadores relacionais.

- **expr_simp**: `[+ | – ] termo { (+ | – | ||) termo }`  
  Expressões simples com operadores aritméticos e lógicos.

- **termo**: `fator { (* | / | &&) fator }`  
  Termos que podem incluir multiplicação, divisão e operadores lógicos.

- **fator**: `id { [ expr ] } | intcon | realcon | charcon | ( expr ) | ! fator`  
  Fatores em expressões.

- **op_rel**: `== | != | <= | < | >= | >`  
  Operadores relacionais usados em expressões.

---
