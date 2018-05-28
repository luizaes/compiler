#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H

#define MAX_ID 11
#define SUCESSO 1
#define FRACASSO 0
#define T_INT 2
#define T_STRING 3
#define T_FLOAT 4
#define VARIAVEL 5
#define MUL 6
#define ADD 7
#define SUB 8
#define DIV 9
#define ATRIB 10
#define NEG 11
#define I2F 12
#define F2I 13
#define L 14
#define MAIOR 15
#define MENOR 16
#define MENOR_IGUAL 17
#define MAIOR_IGUAL 18
#define DIFERENTE 19
#define E 20
#define OU 21
#define NEGACAO 22
#define IF 23
#define IFELSE 24
#define IGUAL 25
#define WHILE 26
#define PRINT 27

#include <stdio.h>

extern struct ABB *pABB;
extern int posFrame;
extern int label;
extern int numVar;
extern FILE *arquivo;

/* Estrutura relacionada a árvore sintática*/

typedef struct ast {
	int cod; 
	struct TabSim *id; 
	struct ast *ptr1;
	struct ast *ptr2;
	struct ast *ptr3;
	int constInt;
	float constFloat;
	char string[10000];
	int tipo; 
}AST;

/* Estruturas relacionadas à árvore */
typedef struct noABB { 
	void *dados;
	struct noABB *esq;
	struct noABB *dir;
}NoABB;

typedef struct ABB {
	NoABB *raiz;
    int tamInfo;
}ABB;

/* Estruturas relacionadas à lista encadeada */
typedef struct no {
	char id[MAX_ID];
	struct no *prox;
}NoLDSE;

typedef struct desc {
	NoLDSE *lista;
}LDSE;

/* Tabela de símbolos e struct com atributos necessários */
typedef struct TabSim {
	char id[MAX_ID];
	int tipo;
	int posicao;
}TabSim;

typedef struct Atributos {
	int tipo;
	LDSE *listaID;
	char id[MAX_ID];
	AST *ptr;
	float numFloat;
	int numInt;
	char string[10000];
}Atributos;

/*Protótipos das funções da Árvore Sintática*/
void apagaNoAST(AST *p);
void rodapeArquivo();
void imprimeLocals();
void cabecalhoArquivo();
AST *criaFolhaID(char *id);
AST *criaNo(int tipo, AST *esq, AST *dir);
AST *criaFolhaInt(int tipo, int constante);
AST *criaFolhaFloat(int tipo, float constante);
AST *criaFolhaString(char *string);
void percorreEmOrdemAST(AST *p,  void (* processa)(void *p));
int emOrdemAST(AST *pa,  void (* processa)(void *p));
void gerarAtrib(char *id);
void gerarID(char *id);
void imprimeAssembly(AST *raiz);
AST * i2f(AST *p);
AST * f2i(AST *p);
int consultaTipo(AST *ast);
AST *criaNoIf(int tipo, AST *esq, AST *centro, AST *dir);
void imprimeLabel(AST *raiz, int lv, int lf);

/* Protótipos das funções da Tabela de Simbolos */
void insereTabSim(LDSE *pLista, int tipo);
void printar(void *info);

/* Funções da árvore */
void apagaNoABB(NoABB *p);
void percorreEmPreOrdem(NoABB *pa, void (* processa)(void *p));
void percorreEmPosOrdem(AST *pa, void (* processa)(void *p));
void percorreEmOrdem(NoABB *pa,  void (* processa)(void *p));
int criaABB(ABB **pp, int tamInfo);
void destroiABB(ABB **pp);
void reiniciaABB(ABB *p);
void apagaNoABB(NoABB *p);
int buscaABB(ABB *p, void **destino, void *chaveDeBusca, unsigned short (* cmp)(void *p1, void *p2));
int insereABB(ABB *p, void *novoReg, unsigned short (* cmp)(void *p1, void *p2));
int emOrdem(ABB *pa, void (* processa)(void *p));
int posOrdem(AST *pa, void (* processa)(void *p));
int preOrdem(ABB *pa,  void (* processa)(void *p));
unsigned short comparaCamposChaves1(void *pInfo1, void *pInfo2);
unsigned short  comparaCamposChaves2(void *chave, void *pInfo);

/* Funções da lista */
int cria(LDSE **pp);
int insereNoInicio(LDSE *p, char *novo);
int insereNoFim(LDSE *p, char *dados);
int buscaNoFim(LDSE *p, char *reg);
int removeDoFim(LDSE *p);
void destroi(LDSE **pp);
void reinicia(LDSE *p);

#endif
