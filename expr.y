%{
#include <stdio.h>
#include <stdlib.h> 
#include "prototipos.h"
#define YYSTYPE Atributos 

int linha = 1;
%}

%token TADD TMUL TSUB TDIV TAPAR TFPAR TNUMFLOAT TNUMINT TACH TFCH TPONTVIR TATRI TMENOR TMAIOR TE TNEG TOU TVIR TWHILE TIF TELSE TPRINT TREAD TINT TSTRING TFLOAT TRETURN TVOID TLITERAL TID

%%
Programa: ListaFuncoes BlocoPrincipal
	| BlocoPrincipal {$$.ptr = $1.ptr; imprimeLocals(); imprimeAssembly($$.ptr); apagaNoAST($$.ptr);}
	;
ListaFuncoes: ListaFuncoes Funcao
	| Funcao
	;
Funcao: TipoRetorno TID TAPAR DeclParametros TFPAR BlocoPrincipal
	| TipoRetorno TID TAPAR TFPAR BlocoPrincipal
	;
TipoRetorno: Tipo
	| TVOID
	;
DeclParametros: DeclParametros TVIR Parametro
	| Parametro
	;
Parametro: Tipo TID
	;
BlocoPrincipal: TACH Declaracoes ListaCmd TFCH {$$.ptr = $3.ptr;}
	| TACH ListaCmd TFCH {$$.ptr = $2.ptr;}
	;
Declaracoes: Declaracoes Declaracao 
	| Declaracao 
	;
Declaracao: Tipo ListaId TPONTVIR  {insereTabSim($2.listaID, $1.tipo); destroi(&$2.listaID);}
	;
Tipo: TINT {$$.tipo = T_INT;}
	| TSTRING {$$.tipo = T_STRING;}
	| TFLOAT {$$.tipo = T_FLOAT;}
	;
ListaId: ListaId TVIR TID {insereNoFim($1.listaID, (void *)&$3.id); } 
	| TID {cria(&$$.listaID); insereNoInicio($$.listaID, (void *)&$1.id);}
	;
Bloco: TACH ListaCmd TFCH {$$.ptr = $2.ptr;}
	;
ListaCmd: ListaCmd Comando {$$.ptr = criaNo(L, $1.ptr, $2.ptr);}
	| Comando {$$.ptr = $1.ptr; }
	;
Comando: CmdSe
	| CmdEnquanto
	| CmdAtrib {$$.ptr = $1.ptr; }
	| CmdEscrita
	| CmdLeitura
	| ChamadaProc
	| Retorno
	;
Retorno: TRETURN ExpressaoAritmetica TPONTVIR
	| TRETURN TLITERAL
	;
CmdSe: TIF TAPAR ExpressaoLogica TFPAR Bloco {$$.ptr = criaNoIf(IF, $3.ptr, $5.ptr, NULL);}
	| TIF TAPAR ExpressaoLogica TFPAR Bloco TELSE Bloco {$$.ptr = criaNoIf(IFELSE, $3.ptr, $5.ptr, $7.ptr);}
	;
CmdEnquanto: TWHILE TAPAR ExpressaoLogica TFPAR Bloco {$$.ptr = criaNo(WHILE, $3.ptr, $5.ptr);}
	;
CmdAtrib: TID TATRI ExpressaoAritmetica TPONTVIR {$1.ptr = criaFolhaID($1.id); $1.tipo = consultaTipo($1.ptr);  
					   if($1.tipo == T_STRING || $3.tipo == T_STRING){printf("Erro: Operação com string\n");}
	                   else if($1.tipo == T_INT && $3.tipo == T_FLOAT){ $3.tipo = T_INT; $$.ptr = criaNo(ATRIB, $1.ptr, f2i($3.ptr)); $$.tipo = T_INT;}
					   else if($1.tipo == T_FLOAT && $3.tipo == T_INT){ $3.tipo = T_FLOAT; $$.ptr = criaNo(ATRIB, $1.ptr, i2f($3.ptr)); $$.tipo = T_FLOAT;}
					   else {$$.ptr = criaNo(ATRIB, $1.ptr, $3.ptr); $$.tipo = $1.tipo;}} 
	| TID TATRI TLITERAL TPONTVIR
	;
CmdEscrita: TPRINT TAPAR ExpressaoAritmetica TFPAR TPONTVIR {$$.tipo = $3.tipo; $$.ptr = criaNo(PRINT, $3.ptr, NULL);}
	| TPRINT TAPAR TLITERAL TFPAR TPONTVIR {$$.tipo = T_STRING; $3.ptr = criaFolhaString($3.string); $$.ptr = criaNo(PRINT, $3.ptr, NULL);}
	;
CmdLeitura: TREAD TAPAR TID TFPAR TPONTVIR
	;
ChamadaProc: ChamadaFuncao TPONTVIR
	;
ChamadaFuncao: TID TAPAR ListaParametros TFPAR
	| TID TAPAR TFPAR
	;
ListaParametros: ListaParametros TVIR ExpressaoAritmetica
	| ListaParametros TVIR TLITERAL
	| ExpressaoAritmetica
	| TLITERAL
	;
ExpressaoRelacional: ExpressaoAritmetica TMAIOR ExpressaoAritmetica {if($1.tipo == T_STRING || $3.tipo == T_STRING) {printf("Erro: Operação com string\n");}
																	 else if($1.tipo == T_FLOAT && $3.tipo == T_INT) {$$.ptr = criaNo(MAIOR, $1.ptr, i2f($3.ptr)); $$.tipo = T_FLOAT;}
																	 else if($1.tipo == T_INT && $3.tipo == T_FLOAT) {$$.ptr = criaNo(MAIOR, i2f($1.ptr), $3.ptr); $$.tipo = T_FLOAT;}
																	 else {$$.ptr = criaNo(MAIOR, $1.ptr, $3.ptr); $$.tipo = $1.tipo;}}
	| ExpressaoAritmetica TMENOR ExpressaoAritmetica {if($1.tipo == T_STRING || $3.tipo == T_STRING) {printf("Erro: Operação com string\n");}
																	 else if($1.tipo == T_FLOAT && $3.tipo == T_INT) {$$.ptr = criaNo(MENOR, $1.ptr, i2f($3.ptr)); $$.tipo = T_FLOAT;}
																	 else if($1.tipo == T_INT && $3.tipo == T_FLOAT) {$$.ptr = criaNo(MENOR, i2f($1.ptr), $3.ptr); $$.tipo = T_FLOAT;}
																	 else {$$.ptr = criaNo(MENOR, $1.ptr, $3.ptr); $$.tipo = $1.tipo;}}
	| ExpressaoAritmetica TMENOR TATRI ExpressaoAritmetica {if($1.tipo == T_STRING || $4.tipo == T_STRING) {printf("Erro: Operação com string\n");}
																	 else if($1.tipo == T_FLOAT && $4.tipo == T_INT) {$$.ptr = criaNo(MENOR_IGUAL, $1.ptr, i2f($4.ptr)); $$.tipo = T_FLOAT;}
																	 else if($1.tipo == T_INT && $4.tipo == T_FLOAT) {$$.ptr = criaNo(MENOR_IGUAL, i2f($1.ptr), $4.ptr); $$.tipo = T_FLOAT;}
																	 else {$$.ptr = criaNo(MENOR_IGUAL, $1.ptr, $4.ptr); $$.tipo = $1.tipo;}}
	| ExpressaoAritmetica TMAIOR TATRI ExpressaoAritmetica {if($1.tipo == T_STRING || $4.tipo == T_STRING) {printf("Erro: Operação com string\n");}
																	 else if($1.tipo == T_FLOAT && $4.tipo == T_INT) {$$.ptr = criaNo(MAIOR_IGUAL, $1.ptr, i2f($4.ptr)); $$.tipo = T_FLOAT;}
																	 else if($1.tipo == T_INT && $4.tipo == T_FLOAT) {$$.ptr = criaNo(MAIOR_IGUAL, i2f($1.ptr), $4.ptr); $$.tipo = T_FLOAT;}
																	 else {$$.ptr = criaNo(MAIOR_IGUAL, $1.ptr, $4.ptr); $$.tipo = $1.tipo;}}
	| ExpressaoAritmetica TATRI TATRI ExpressaoAritmetica {if($1.tipo == T_STRING || $4.tipo == T_STRING) {printf("Erro: Operação com string\n");}
																	 else if($1.tipo == T_FLOAT && $4.tipo == T_INT) {$$.ptr = criaNo(IGUAL, $1.ptr, i2f($4.ptr)); $$.tipo = T_FLOAT;}
																	 else if($1.tipo == T_INT && $4.tipo == T_FLOAT) {$$.ptr = criaNo(IGUAL, i2f($1.ptr), $4.ptr); $$.tipo = T_FLOAT;}
																	 else {$$.ptr = criaNo(IGUAL, $1.ptr, $4.ptr); $$.tipo = $1.tipo;}}
	| ExpressaoAritmetica TNEG TATRI ExpressaoAritmetica {if($1.tipo == T_STRING || $4.tipo == T_STRING) {printf("Erro: Operação com string\n");}
																	 else if($1.tipo == T_FLOAT && $4.tipo == T_INT) {$$.ptr = criaNo(DIFERENTE, $1.ptr, i2f($4.ptr)); $$.tipo = T_FLOAT;}
																	 else if($1.tipo == T_INT && $4.tipo == T_FLOAT) {$$.ptr = criaNo(DIFERENTE, i2f($1.ptr), $4.ptr); $$.tipo = T_FLOAT;}
																	 else {$$.ptr = criaNo(DIFERENTE, $1.ptr, $4.ptr); $$.tipo = $1.tipo;}}
	;
ExpressaoLogica: ExpressaoLogica TE TE TermoLogico {$$.ptr = criaNo(E, $1.ptr, $4.ptr);}
	| ExpressaoLogica TOU TOU TermoLogico {$$.ptr = criaNo(OU, $1.ptr, $4.ptr);}
	| TermoLogico {$$.ptr = $1.ptr;}
	;
TermoLogico: TAPAR ExpressaoLogica TFPAR {$$.ptr = $2.ptr;}	
	| TNEG TermoLogico {$$.ptr = criaNo(NEGACAO, $2.ptr, NULL);}
	| ExpressaoRelacional {$$.ptr = $1.ptr;}
	;
ExpressaoAritmetica: Expr {$$.ptr = $1.ptr; $$.tipo = $1.tipo;}
	;
Expr: Expr TADD Termo {if($1.tipo == T_STRING || $3.tipo == T_STRING){printf("Erro: Operação com string\n");}
	                   else if($1.tipo == T_INT && $3.tipo == T_FLOAT){ $1.tipo = T_FLOAT; $$.ptr = criaNo(ADD, i2f($1.ptr), $3.ptr); $$.tipo = T_FLOAT;}
					   else if($1.tipo == T_FLOAT && $3.tipo == T_INT){ $3.tipo = T_FLOAT; $$.ptr = criaNo(ADD, $1.ptr, i2f($3.ptr)); $$.tipo = T_FLOAT;}
					   else {$$.ptr = criaNo(ADD, $1.ptr, $3.ptr); $$.tipo = $1.tipo;}}
	| Expr TSUB Termo { if($1.tipo == T_STRING || $3.tipo == T_STRING){printf("Erro: Operação com string\n");}
			 		   else if($1.tipo == T_INT && $3.tipo == T_FLOAT){ $1.tipo = T_FLOAT; $$.ptr = criaNo(SUB, i2f($1.ptr), $3.ptr); $$.tipo = T_FLOAT; }
					   else if($1.tipo == T_FLOAT && $3.tipo == T_INT){ $3.tipo = T_FLOAT; $$.ptr = criaNo(SUB, $1.ptr, i2f($3.ptr)); $$.tipo = T_FLOAT;}
					   else { $$.ptr = criaNo(SUB, $1.ptr, $3.ptr); $$.tipo = $1.tipo;}}
	| Termo {$$.ptr = $1.ptr; $$.tipo = $1.tipo;}
	;
Termo: Termo TMUL Fator {  if($1.tipo == T_STRING || $3.tipo == T_STRING){printf("Erro: Operação com string\n");}
					   else if($1.tipo == T_INT && $3.tipo == T_FLOAT){ $1.tipo = T_FLOAT; $$.ptr = criaNo(MUL, i2f($1.ptr), $3.ptr); $$.tipo = T_FLOAT; }
					   else if($1.tipo == T_FLOAT && $3.tipo == T_INT){ $3.tipo = T_FLOAT; $$.ptr = criaNo(MUL, $1.ptr, i2f($3.ptr)); $$.tipo = T_FLOAT;}
					   else {$$.ptr = criaNo(MUL, $1.ptr, $3.ptr); $$.tipo = $1.tipo;}}
	| Termo TDIV Fator { if($1.tipo == T_STRING || $3.tipo == T_STRING){printf("Erro: Operação com string\n");}
					   else if($1.tipo == T_INT && $3.tipo == T_FLOAT){ $1.tipo = T_FLOAT; $$.ptr = criaNo(DIV, i2f($1.ptr), $3.ptr); $$.tipo = T_FLOAT; }
					   else if($1.tipo == T_FLOAT && $3.tipo == T_INT){ $3.tipo = T_FLOAT; $$.ptr = criaNo(DIV, $1.ptr, i2f($3.ptr)); $$.tipo = T_FLOAT;}
					   else {$$.ptr = criaNo(DIV, $1.ptr, $3.ptr); $$.tipo = $1.tipo;}}
	| Fator {$$.ptr = $1.ptr; $$.tipo = $1.tipo;}
	;
Fator: TNUMINT {$$.ptr = criaFolhaInt(T_INT, $1.numInt); $$.tipo = T_INT;}
	| TNUMFLOAT {$$.ptr = criaFolhaFloat(T_FLOAT, $1.numFloat); $$.tipo = T_FLOAT;}
	| TID {$$.ptr = criaFolhaID($1.id); $$.tipo = consultaTipo($$.ptr);}
	| ChamadaFuncao
	| TAPAR Expr TFPAR {$$.ptr = $2.ptr; $$.tipo = $2.tipo;}
	| TSUB Fator {$$.tipo = $2.tipo; $$.ptr = criaNo(NEG, $2.ptr, NULL);}
	;

%%
#include "lex.yy.c"

int yyerror (char *str)
{
	printf("%s - antes %s linha: %d\n", str, yytext, linha);
	
} 		 

int yywrap()
{
	return 1;
}
