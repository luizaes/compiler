#include "prototipos.h"
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
struct ABB *pABB;
int posFrame = 1;
int label = 1;
int numVar = 0;
FILE *arquivo;

int main(int argc, char *argv[]) {

	yyin = fopen(argv[1], "r");
	if (yyin == NULL) {
		printf("Problema ao abrir arquivo.\n");
		exit(EXIT_FAILURE);
	}

	if(criaABB(&pABB, sizeof(TabSim)) == FRACASSO) {
		printf("Erro ao criar árvore\n");
	}

	arquivo = fopen("Saida.j", "wr+");
	if (arquivo == NULL) {
		printf("Problema ao criar arquivo.\n");
		exit(EXIT_FAILURE);
	}

	cabecalhoArquivo();

	yyparse();

	rodapeArquivo();
	
	emOrdem(pABB, printar);
	destroiABB(&pABB);	

	fclose(yyin);
	return 0;
}

