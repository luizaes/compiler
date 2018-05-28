#include "prototipos.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Funções para a Árvore Sintática */

void gerarID(char *id){
	TabSim elemento;
		strcpy(elemento.id, id);
		TabSim *endereco;
		AST *ponteiro = NULL;
		if(buscaABB(pABB, (void *)&endereco, (void *)&elemento, comparaCamposChaves1) == SUCESSO){
			printf("iload %d\n", endereco->posicao);	
		}	
}

void gerarAtrib(char *id){
	TabSim elemento;
		strcpy(elemento.id, id);
		TabSim *endereco;
		AST *ponteiro = NULL;
		if(buscaABB(pABB, (void *)&endereco, (void *)&elemento, comparaCamposChaves1) == SUCESSO){
			printf("istore %d\n", endereco->posicao);	
		}	
}

void apagaNoAST(AST *p) { 
   if(p!=NULL)
   {   apagaNoAST(p->ptr1);
       apagaNoAST(p->ptr2);
       free(p);
   }
   p = NULL;
}

AST *criaFolhaFloat(int tipo, float constante) {

	AST * ponteiro = NULL;

	if((ponteiro = malloc(sizeof(AST))) != NULL) {
		ponteiro->id = NULL;
		ponteiro->ptr1 = NULL;
		ponteiro->ptr2 = NULL;
		
		ponteiro->cod = T_FLOAT;
		ponteiro->constFloat = constante;
		ponteiro->constInt = 0;
		ponteiro->tipo = T_FLOAT;
	}

	return ponteiro;
}

AST *criaFolhaInt(int tipo, int constante) {

	AST *ponteiro = NULL;

	if((ponteiro = malloc (sizeof(AST))) != NULL) {
		ponteiro->id = NULL;
		ponteiro->ptr1 = NULL;
		ponteiro->ptr2 = NULL;
		
		ponteiro->cod = T_INT;
		ponteiro->constInt = constante;
		ponteiro->constFloat = 0;
		ponteiro->tipo = T_INT;
	}

	return ponteiro;
}

AST *criaFolhaString(char *string) {

	AST *ponteiro = NULL;

	if((ponteiro = malloc (sizeof(AST))) != NULL) {
		ponteiro->id = NULL;
		ponteiro->ptr1 = NULL;
		ponteiro->ptr2 = NULL;
		
		ponteiro->cod = T_STRING;
		ponteiro->constInt = 0;
		ponteiro->constFloat = 0;
		strcpy(ponteiro->string, string);
		ponteiro->tipo = T_STRING;
	}

	return ponteiro;	
}

AST *criaFolhaID(char *id){
	TabSim elemento;
	strcpy(elemento.id, id);
	TabSim *endereco;
	AST *ponteiro = NULL;

	if(buscaABB(pABB, (void *)&endereco, (void *)&elemento, comparaCamposChaves1) == SUCESSO){

		if((ponteiro = (AST *) malloc (sizeof(AST))) != NULL) {
			ponteiro->id = endereco;
			ponteiro->ptr1 = NULL;
			ponteiro->ptr2 = NULL;
			ponteiro->cod = VARIAVEL;
			ponteiro->constInt = 0;
			ponteiro->constFloat = 0;
			ponteiro->tipo = endereco->tipo;
		}
	} else {
		printf("ERROR: A variável %s não foi declarada!\n", id);
		exit(EXIT_FAILURE);
	}
	return ponteiro;
}

AST *criaNo(int tipo, AST *esq, AST *dir){

	AST *ponteiro = NULL;
	if((ponteiro = (AST *) malloc (sizeof(AST))) != NULL) {
		ponteiro->cod = tipo;
		ponteiro->id = NULL;
		ponteiro->ptr1 = esq;
		ponteiro->ptr2 = dir;
		ponteiro->ptr3 = NULL;
		ponteiro->constInt = 0;
		ponteiro->constFloat = 0;
		if(tipo != L && tipo != WHILE && tipo != ATRIB && tipo != PRINT && tipo != NEG) {
			
			if(esq->tipo == T_INT && dir->tipo == T_FLOAT) {
				ponteiro->tipo = T_FLOAT; 
			} else if(esq->tipo == T_FLOAT && dir->tipo == T_INT) {
				ponteiro->tipo = T_FLOAT;
			} else if(esq->tipo == T_FLOAT && dir->tipo == T_FLOAT) {
				ponteiro->tipo = T_FLOAT;
			} else if(esq->tipo == T_INT && dir->tipo == T_INT) {
				ponteiro->tipo = T_INT;
			}
		} else if(tipo == WHILE || tipo == ATRIB || tipo == PRINT || tipo == NEG) {
			ponteiro->tipo = esq->tipo;
		}

	}
	return ponteiro;
}

AST *criaNoIf(int tipo, AST *esq, AST *centro, AST *dir){
	AST *ponteiro = NULL;
	if((ponteiro = (AST *) malloc (sizeof(AST))) != NULL) {
		ponteiro->cod = tipo;
		ponteiro->id = NULL;
		ponteiro->ptr1 = esq;
		ponteiro->ptr2 = centro;
		ponteiro->ptr3 = dir;
		ponteiro->constInt = 0;
		ponteiro->constFloat = 0;
	}
	return ponteiro;
}

void imprimeAssembly(AST *raiz) {
	
	int label1, label2, laux;

	if(raiz->cod == ADD) {
		imprimeAssembly(raiz->ptr1);
		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT){
			fprintf(arquivo,"  iadd\n");
		} else if (raiz->tipo == T_FLOAT){
			fprintf(arquivo, "  fadd\n");
		}		

	} else if(raiz->cod == SUB) {

		imprimeAssembly(raiz->ptr1);
		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT){
			fprintf(arquivo, "  isub\n");
		} else if (raiz->tipo == T_FLOAT){
			fprintf(arquivo, "  fsub\n");
		}

	} else if(raiz->cod == MUL) {

		imprimeAssembly(raiz->ptr1);
		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT){
			fprintf(arquivo, "  imul\n");
		} else if (raiz->tipo == T_FLOAT){
			fprintf(arquivo, "  fmul\n");
		}

	} else if(raiz->cod == DIV) {

		imprimeAssembly(raiz->ptr1);
		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT){
			fprintf(arquivo, "  idiv\n");
		} else if (raiz->tipo == T_FLOAT){
			fprintf(arquivo, "  fdiv\n");
		}

	} else if(raiz->cod == T_INT) {

		if(raiz->constInt <= 5) {
			fprintf(arquivo, "  iconst_%d\n", raiz->constInt);
		} else if(raiz->constInt == -1) {
			fprintf(arquivo, "  iconst_m1\n");
		} else if((raiz->constInt > 5 && raiz->constInt < 128) || (raiz->constInt < -1 && raiz->constInt >= -128) ) {
			fprintf(arquivo, "  bipush %d\n", raiz->constInt);
		} else {
			fprintf(arquivo, "  ldc %d\n", raiz->constInt); 
		}

	} else if(raiz->cod == T_FLOAT) {

		fprintf(arquivo, "  ldc %f\n", raiz->constFloat);

	} else if(raiz->cod == VARIAVEL) {

		if(raiz->tipo == T_INT){
			fprintf(arquivo, "  iload %d\n", raiz->id->posicao);
		}else if(raiz->tipo == T_FLOAT){
			fprintf(arquivo, "  fload %d\n", raiz->id->posicao);
		} else if(raiz->tipo == T_STRING){
			//Ainda não implementado
		}

	} else if(raiz->cod == ATRIB){

		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT){
			fprintf(arquivo, "  istore %d\n", raiz->ptr1->id->posicao);
		} else if (raiz->tipo == T_FLOAT){
			fprintf(arquivo, "  fstore %d\n", raiz->ptr1->id->posicao);
		} 
	
	}else if(raiz->cod == NEG){

		imprimeAssembly(raiz->ptr1);
		if(raiz->tipo == T_INT){
			fprintf(arquivo, "  ineg\n");
		} else if (raiz->tipo == T_FLOAT){
			fprintf(arquivo, "  fneg\n");
		}
	
	}
	else if(raiz->cod == I2F){

		imprimeAssembly(raiz->ptr1);
		fprintf(arquivo, "  i2f\n");
	
	}
	else if(raiz->cod == F2I){

		imprimeAssembly(raiz->ptr1);
		fprintf(arquivo, "  f2i\n");
	
	} else if(raiz->cod == L) {
		
		if(raiz->ptr1 != NULL){
			imprimeAssembly(raiz->ptr1);
		}
		if(raiz->ptr2 != NULL){
			imprimeAssembly(raiz->ptr2);
		}

	}  else if(raiz->cod == IF) {

		label1 = label++;
		label2 = label++;
		imprimeLabel(raiz->ptr1, label1, label2);
		fprintf(arquivo, " L%d: \n", label1);
		imprimeAssembly(raiz->ptr2);
		fprintf(arquivo, " L%d: \n", label2);

	} else if(raiz->cod == IFELSE) {

		label1 = label++;
		label2 = label++;
		imprimeLabel(raiz->ptr1, label1, label2);
		fprintf(arquivo, " L%d: \n", label1);
		imprimeAssembly(raiz->ptr2);
		laux = label++;
		fprintf(arquivo, "  goto L%d\n", laux);
		fprintf(arquivo, " L%d:\n", label2);
		imprimeAssembly(raiz->ptr3);
		fprintf(arquivo, " L%d:\n", laux);
	
	} else if(raiz->cod == WHILE) {

		laux = label++;
		label1 = label++;
		label2 = label++;
		fprintf(arquivo, " L%d:\n", laux);
		imprimeLabel(raiz->ptr1, label1, label2);
		fprintf(arquivo, " L%d:\n", label1);
		imprimeAssembly(raiz->ptr2);
		fprintf(arquivo, "  goto L%d\n", laux);
		fprintf(arquivo, " L%d:\n", label2);

	} else if(raiz->cod == PRINT) {

		fprintf(arquivo, "  getstatic java/lang/System/out Ljava/io/PrintStream;\n");

		if(raiz->tipo == T_STRING) {
			fprintf(arquivo, "  ldc %s\n", raiz->ptr1->string);
			fprintf(arquivo, "  invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V\n");
		} else {
			imprimeAssembly(raiz->ptr1);
			if(raiz->tipo == T_INT) {
				fprintf(arquivo, "  invokevirtual java/io/PrintStream/println(I)V\n");	
			} else if(raiz->tipo == T_FLOAT) {
				fprintf(arquivo, "  invokevirtual java/io/PrintStream/println(F)V\n");	
			}
		}
	}

}

void imprimeLabel(AST *raiz, int lv, int lf) {
	
	int laux;

	if(raiz->cod == E) {

		laux = label++;
		imprimeLabel(raiz->ptr1, laux, lf);
		fprintf(arquivo, " L%d: \n", laux);
		imprimeLabel(raiz->ptr2, lv, lf);

	} else if(raiz->cod == OU) {

		laux = label++;
		imprimeLabel(raiz->ptr1, lv, laux);
		fprintf(arquivo, " L%d: \n", laux);
		imprimeLabel(raiz->ptr2, lv, lf);

	} else if(raiz->cod == NEGACAO) {

		imprimeLabel(raiz->ptr1, lf, lv);
		
	} else if(raiz->cod == IGUAL) {

		imprimeAssembly(raiz->ptr1);
		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT) {
			fprintf(arquivo, "  if_icmpeq L%d\n", lv);
		} else if(raiz->tipo == T_FLOAT) {
			fprintf(arquivo, "  fcmpl\n");
			fprintf(arquivo, "  ifeq L%d\n", lv);
		}
		fprintf(arquivo, "  goto L%d\n", lf);

	} else if(raiz->cod == DIFERENTE) {

		imprimeAssembly(raiz->ptr1);
		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT) {
			fprintf(arquivo, "  if_icmpne L%d\n", lv);
		} else if(raiz->tipo == T_FLOAT) {
			fprintf(arquivo, "  fcmpl\n");
			fprintf(arquivo, "  ifne L%d\n", lv);
		}
		fprintf(arquivo, "  goto L%d\n", lf);

	} else if(raiz->cod == MAIOR) {

		imprimeAssembly(raiz->ptr1);
		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT) {
			fprintf(arquivo, "  if_icmpgt L%d\n", lv);
		} else if(raiz->tipo == T_FLOAT) {
			fprintf(arquivo, "  fcmpl\n");
			fprintf(arquivo, "  ifgt L%d\n", lv);
		}
		fprintf(arquivo, "  goto L%d\n", lf);

	} else if(raiz->cod == MENOR) {

		imprimeAssembly(raiz->ptr1);
		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT) {
			fprintf(arquivo, "  if_icmplt L%d\n", lv);
		} else if(raiz->tipo == T_FLOAT) {
			fprintf(arquivo, "  fcmpl\n");
			fprintf(arquivo, "  iflt L%d\n", lv);
		}
		fprintf(arquivo, "  goto L%d\n", lf);

	} else if(raiz->cod == MAIOR_IGUAL) {

		imprimeAssembly(raiz->ptr1);
		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT) {
			fprintf(arquivo, "  if_icmpge L%d\n", lv);
		} else if(raiz->tipo == T_FLOAT) {
			fprintf(arquivo, "  fcmpl\n");
			fprintf(arquivo, "  ifge L%d\n", lv);
		}
		fprintf(arquivo, "  goto L%d\n", lf);

	} else if(raiz->cod == MENOR_IGUAL) {

		imprimeAssembly(raiz->ptr1);
		imprimeAssembly(raiz->ptr2);
		if(raiz->tipo == T_INT) {
			fprintf(arquivo, "  if_icmple L%d\n", lv);
		} else if(raiz->tipo == T_FLOAT) {
			fprintf(arquivo, "  fcmpl\n");
			fprintf(arquivo, "  ifle L%d\n", lv);
		}
		fprintf(arquivo, "  goto L%d\n", lf);

	}

}

AST * i2f(AST *p){
	AST * no = NULL;
	no = (AST *)malloc(sizeof(AST));
	if(no == NULL){
		printf("Erro alocar nó i2f\n");
		return NULL;
	}
	no->cod = I2F;
	no->id = NULL;
	no->ptr1 = p;
	no->ptr2 = NULL;
	no->constInt = 0;
	no->constFloat = 0;
	no->tipo = p->tipo;
	return no;
}

AST * f2i(AST *p){
	AST * no = NULL;
	no = (AST *)malloc(sizeof(AST));
	if(no == NULL){
		printf("Erro alocar nó f2i\n");
		return NULL;
	}
	no->cod = F2I;
	no->id = NULL;
	no->ptr1 = p;
	no->ptr2 = NULL;
	no->constInt = 0;
	no->constFloat = 0;
	no->tipo = p->tipo;
	return no;
}

int consultaTipo(AST *ast){
	return ast->id->tipo;
}

void cabecalhoArquivo() {
	fprintf(arquivo, ".class public Saida\n.super java/lang/Object\n\n");
	fprintf(arquivo, ".method public <init>()V\n aload_0\n\n");
	fprintf(arquivo, " invokenonvirtual java/lang/Object/<init>()V\n return\n.end method\n\n");
	fprintf(arquivo, ".method public static main([Ljava/lang/String;)V\n .limit stack 7\n");
}

void rodapeArquivo() {
	fprintf(arquivo, " return\n");
	fprintf(arquivo, ".end method\n");
}

void imprimeLocals() {
	fprintf(arquivo, " .limit locals %d\n\n", numVar+1);
}


/* Funções para a Tabela de Símbolos */

void insereTabSim(LDSE *pLista, int tipo) {

	TabSim elemento;
	char aux[MAX_ID];
	elemento.tipo = tipo;

	while(pLista->lista != NULL) {
		numVar++;
		buscaNoFim(pLista, aux);
		removeDoFim(pLista);
		elemento.posicao = posFrame;
		posFrame++;
		strcpy(elemento.id, aux);
		insereABB(pABB, (void *)&elemento, comparaCamposChaves1);
	}
}

/* Funções para manipulação da árvore */

// Cria a àrvore
int criaABB(ABB **pp, int tamInfo) {
	int ret = FRACASSO;
	if ((*pp = (ABB *) malloc(sizeof(ABB))) != NULL ) {
		(*pp)->raiz = NULL;
		(*pp)->tamInfo = tamInfo;
		ret = SUCESSO;
    }
	return ret;
}

// Destroi a àrvore
void destroiABB(ABB **pp) {
	reiniciaABB(*pp);
    free(*pp);
    (*pp)= NULL;
}

// Reinicia a àrvore
void reiniciaABB(ABB *p) {
   apagaNoABB(p->raiz);
   p->raiz = NULL;
}

// Busca na árvore - Modificada para colocar o endereço da célula em destino
int buscaABB(ABB *pa, void **destino, void *chaveDeBusca, unsigned short(* cmp)(void *p1, void *p2)) {  
	NoABB *aux;
   	unsigned int ret = FRACASSO;
   	aux = pa->raiz;

   	while( aux != NULL && (*cmp)(chaveDeBusca,aux->dados) != '=')
    	aux = (*cmp)(chaveDeBusca,aux->dados)== '<' ? aux->esq : aux->dir;
   	if (aux != NULL) {
   		*destino = (TabSim *) aux->dados;
        ret = SUCESSO;
    }
   	return ret;
}

// Insere na árvore
int insereABB(ABB *pa, void *novoReg, unsigned short (* cmp)(void *p1, void *p2)) {  	
	NoABB *p, *q, *novoNoABB;

    p = q = pa->raiz;
 	while( p != NULL )
      	{  if ( (*cmp)(novoReg,p->dados) == '=')
               	return FRACASSO; 
            else
     		{   	q = p;
			p = ((*cmp)(novoReg,p->dados)== '<') ? p->esq : p->dir;
                }
        }


  	if ((novoNoABB = (NoABB *) malloc (sizeof(NoABB))) != NULL)
    	{
		if((novoNoABB->dados = (void *)malloc(pa->tamInfo)) !=NULL)
	        {	memcpy(novoNoABB->dados, novoReg, pa->tamInfo);
	         	novoNoABB->dir = novoNoABB->esq = NULL; 


	            	if (q != NULL)
	                	if( (*cmp)(novoNoABB->dados,q->dados) == '<')
	                	    q->esq = novoNoABB;
				else
               			     q->dir=novoNoABB;
            		else
		                pa->raiz = novoNoABB; 
           		return SUCESSO;
        	}
        	else
            		free(novoNoABB);
    }
    return FRACASSO;  
}

// Percorre em pré-ordem
int preOrdem(ABB *pa, void (* processa)(void *p)) {
	if(pa->raiz == NULL) {
        return FRACASSO;
	} else {
        percorreEmPreOrdem(pa->raiz, processa);
        return SUCESSO;
    }
}

// Percorre em pós-ordem MODIFICADA PARA AST
int posOrdem(AST *pa, void (* processa)(void *p)) {
	if(pa == NULL) {
        return FRACASSO;
   	} else {
   		//printf("chamei\n");
        percorreEmPosOrdem(pa, processa);
        return SUCESSO;
   	}
}

// Percorre em ordem
int emOrdem(ABB *pa,  void (* processa)(void *p)){
   if(pa->raiz == NULL) {
        return FRACASSO;
   } else {
   		percorreEmOrdem(pa->raiz, processa);
        return SUCESSO;
     }
}

int emOrdemAST(AST *pa,  void (* processa)(void *p)){
   if(pa == NULL) {
        return FRACASSO;
   } else {
   		percorreEmOrdemAST(pa, processa);
        return SUCESSO;
     }
}

void apagaNoABB(NoABB *p) {
 	if(p!=NULL) {
 	   apagaNoABB(p->esq);
       apagaNoABB(p->dir);
       free( p->dados );
       free(p);
   }

}

void percorreEmPreOrdem(NoABB *p,  void (* processa)(void *p)) {
	if(p != NULL) { 
    	processa(p->dados);
      	percorreEmPreOrdem(p->esq, processa);
      	percorreEmPreOrdem(p->dir, processa);
    }
}

void percorreEmOrdem(NoABB *p,  void (* processa)(void *p)) {  
	if(p != NULL) { 	
    	percorreEmOrdem(p->esq, processa);
      	processa(p->dados);
      	percorreEmOrdem(p->dir, processa);
    }
}

void percorreEmOrdemAST(AST *p,  void (* processa)(void *p)) {  
	if(p != NULL && (p->cod == ATRIB || p->cod == VARIAVEL )) { 	
    	percorreEmOrdemAST(p->ptr1, processa);
      	processa(p);
      	percorreEmOrdemAST(p->ptr2, processa);
    }
}

void percorreEmPosOrdem(AST *p, void (* processa)(void *p)) { 
	if(p != NULL) { 
    	percorreEmPosOrdem(p->ptr1, processa);
      	percorreEmPosOrdem(p->ptr2, processa);
      	processa(p);
    }
}

unsigned short comparaCamposChaves1(void *pInfo1, void *pInfo2)
{	char a1[MAX_ID], a2[MAX_ID]; 
	strcpy(a1, ((TabSim *)pInfo1)->id);
	strcpy(a2, ((TabSim *)pInfo2)->id);


 	if (strcmp(a1, a2) < 0)
		return '<';
	else
    		if (strcmp(a1, a2) > 0)
         		return '>';
      		else	
         		return '=';
}

unsigned short  comparaCamposChaves2(void *chave, void *pInfo)
{  
	char ch1[MAX_ID], ch2[MAX_ID]; 
	strcpy(ch1, ((char *)chave));
	strcpy(ch2, ((TabSim *)pInfo)->id);

   if (strcmp(ch1, ch2) < 0)
	return '<';
   else
    	if (strcmp(ch1, ch2) > 0)
         	return '>';
       	else
         	return '=';
}

void printar(void *info) {
	TabSim *tb = (TabSim *)info;

	printf("Variável: %s  Posição: %d ", tb->id, tb->posicao);
	if(tb->tipo == 2) {
		printf("Tipo: Int\n");
	} else if(tb->tipo == 3) {
		printf("Tipo: String\n");
	} else if(tb->tipo == 4) {
		printf("Tipo: Float\n");
	}
}

/* Funções para manipulação da lista encadeada */

// Cria a lista
int cria(LDSE **pp) {

	LDSE *desc = NULL;

	if((desc = (LDSE *)malloc(sizeof(LDSE))) != NULL) {
		// Aloquei o descritor, agora vou inicializá-lo

		desc->lista = NULL;
		
	} else {
		return FRACASSO;
	} 

	(*pp) = desc;
		
	return SUCESSO;
}

// Insere no início da lista
int insereNoInicio(LDSE *p, char *id) {

	NoLDSE *aux = NULL;

	if((aux = (NoLDSE *)malloc(sizeof(NoLDSE))) != NULL) {
		strcpy(aux->id, id);
		aux->prox = p->lista;
		p->lista = aux;
	} else {
		return FRACASSO;
	}
		
	return SUCESSO;
}

// Insere no fim da lista
int insereNoFim(LDSE *p, char *id) {

	NoLDSE *aux = NULL, *temp = NULL;
		
	if((aux = (NoLDSE *)malloc(sizeof(NoLDSE))) != NULL) {
		
			strcpy(aux->id, id);
			temp = p->lista;
		
			while(temp->prox != NULL) {
				temp = temp->prox;
			}
					
			temp->prox = aux;
			aux->prox = NULL;
		
	} else {
		return FRACASSO;
	}
			
	return SUCESSO;
}

// Busca no fim
int buscaNoFim(LDSE *p, char *reg) {

	NoLDSE *aux = NULL;

	if(p->lista == NULL) {
		return FRACASSO;
	}
	aux = p->lista;
	while(aux->prox != NULL) {
		aux = aux->prox;
	}

	strcpy(reg, aux->id);
	return SUCESSO;
}

// Remove do fim
int removeDoFim(LDSE *p) {

	NoLDSE *aux = NULL, *pre = NULL;
	
	if(p->lista == NULL) {
		return FRACASSO;
	}

	pre = p->lista;
	aux = p->lista->prox;
	if(aux == NULL) {
		free(pre);
		p->lista = NULL;
		return SUCESSO;
	}
	while(aux->prox != NULL) {
		pre = aux;		
		aux = aux->prox;
	}

	pre->prox = NULL;
	free(aux);
	return SUCESSO;
}

// Reinicializa a lista
void reinicia(LDSE *p) {

	NoLDSE *aux = NULL;

	if(p->lista != NULL) {
		aux = p->lista->prox;
		while(aux != NULL) {
			free(p->lista);
			p->lista = aux;
			aux = aux->prox;
		}
		free(p->lista);
		p->lista = NULL;
	}
}

// Destroi a lista
void destroi(LDSE **pp) {

	reinicia(*pp);
	free(*pp);
	(*pp) = NULL;
}
