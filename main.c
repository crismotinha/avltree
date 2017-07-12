#include <stdio.h>
#include <stdlib.h>
#define TAM_ESPACO 10


typedef struct arvoreAVL{
    int info;
    int altura;
    struct arvoreAVL *esq;
    struct arvoreAVL *dir;
}ArvAVL_t;

ArvAVL_t *criar_avl(int _info){
    ArvAVL_t *raiz = (ArvAVL_t*)malloc(sizeof(ArvAVL_t));
    raiz->info = _info;
    raiz->altura = 0;
    raiz->esq = NULL;
    raiz->dir = NULL;
    return raiz;
}

int altura_NO(ArvAVL_t* no){
    if(no == NULL)
        return -1;
    else
        return no->altura;
}

int fatorBalanceamento_NO(ArvAVL_t* no){
    return labs(altura_NO(no->esq) - altura_NO(no->dir));
}

int maior(int a, int b){
    if(a > b)
        return a;
    else
        return b;
}

void print_pre_ordem(ArvAVL_t *raiz){
    if(raiz == NULL)
        return;
    if(raiz != NULL){
        printf("%d\n", raiz->info);
        print_pre_ordem(raiz->esq);
        print_pre_ordem(raiz->dir);
    }
}

void print_em_ordem(ArvAVL_t *raiz){
    if(raiz == NULL)
        return;
    if(raiz != NULL){
        print_em_ordem(raiz->esq);
        printf("%d\n", raiz->info);
        print_em_ordem(raiz->dir);
    }
}

void print_pos_ordem(ArvAVL_t *raiz){
    if(raiz == NULL)
        return;
    if(raiz != NULL){
        print_pos_ordem(raiz->esq);
        print_pos_ordem(raiz->dir);
        printf("%d\n", raiz->info);
    }
}

/*---------------------------------------*/

ArvAVL_t *RotacaoLL(ArvAVL_t *A){//LL
    printf("RotacaoLL no elemento %d\n", A->info);
    ArvAVL_t *B;
    B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    A->altura = maior(altura_NO(A->esq),altura_NO(A->dir)) + 1;
    B->altura = maior(altura_NO(B->esq),A->altura) + 1;
    A = B;
    return A;
}

ArvAVL_t *RotacaoRR(ArvAVL_t *A){//RR
    printf("RotacaoRR no elemento %d\n", A->info);
    ArvAVL_t *B;
    B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    A->altura = maior(altura_NO(A->esq),altura_NO(A->dir)) + 1;
    B->altura = maior(altura_NO(B->dir),A->altura) + 1;
    A = B;
    //printf("Retornou %d\n", A->info);
    return A;
}

ArvAVL_t *RotacaoLR(ArvAVL_t *A){//LR
    A->esq = RotacaoRR(A->esq);
    A = RotacaoLL(A);
    //printf("Fim da LR Retornou %d\n", A->info);
    return A;
}

ArvAVL_t *RotacaoRL(ArvAVL_t *A){//RL
    A->dir = RotacaoLL(A->dir);
    A = RotacaoRR(A);
    return A;
}


ArvAVL_t *insere_ArvAVL(ArvAVL_t *raiz, int valor, int *flag){
    if(raiz == NULL){//árvore vazia ou nó folha
        raiz = criar_avl(valor);
        *flag = 1;
    }
    else {
        if(valor < raiz->info){
            raiz->esq = insere_ArvAVL(raiz->esq, valor, flag);
            if(*flag == 1){
                if(fatorBalanceamento_NO(raiz) >= 2){
                    if(valor < raiz->esq->info ){
                        printf("\n\Antes de rotacionar LL:\n");
                        print_arvore(raiz, 0);
                        printf("\n\n--------------------------------------------\n\n");

                        raiz = RotacaoLL(raiz);

                        printf("Apos rotacao LL:\n");
                        print_arvore(raiz, 0);
                    }
                    else{
                        printf("Rotacao LR do %d\n", raiz->info);
                        printf("Antes de rotacionar LR:\n");
                        print_arvore(raiz, 0);
                        printf("\n\n--------------------------------------------\n\n");

                        raiz = RotacaoLR(raiz);

                        printf("Apos rotacao LR:\n");
                        print_arvore(raiz, 0);
                    }
                }
            }
        }
        else if(valor > raiz->info){
            raiz->dir = insere_ArvAVL(raiz->dir, valor, flag);
            //printf("Inseriu na dir: %d\nValor da flag: %d\n\n", raiz->dir->info, *flag);
            if(*flag == 1){
                //printf("Fator bal do %d: %d\n", raiz->info, fatorBalanceamento_NO(raiz));
                if(fatorBalanceamento_NO(raiz) >= 2){
                    if(raiz->dir->info < valor){

                        printf("\n\nAntes de rotacionar RR:\n");
                        print_arvore(raiz, 0);
                        printf("\n\n--------------------------------------------\n\n");

                        raiz = RotacaoRR(raiz);

                        printf("Apos rotacao RR:\n");
                        print_arvore(raiz, 0);

                    }else{
                        printf("Rotacao RL do %d\n", raiz->info);
                        printf("Antes de rotacionar RL:\n");
                        print_arvore(raiz, 0);
                        printf("\n\n--------------------------------------------\n\n");

                        raiz = RotacaoRL(raiz);

                        printf("Apos rotacao RL:\n");
                        print_arvore(raiz, 0);
                    }
                }
            }
        }else{
            printf("Valor duplicado!!\n");
            flag = 0;
        }
        //printf("final do metodo e valor %d\n", raiz->info);

        raiz->altura = maior(altura_NO(raiz->esq),altura_NO(raiz->dir)) + 1;
    }
    return raiz;
}

ArvAVL_t *procuraMenor(ArvAVL_t *atual){
    ArvAVL_t *no1 = atual;
    ArvAVL_t *no2 = atual->dir;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->dir;
    }
    return no1;
}

ArvAVL_t *remove_ArvAVL(ArvAVL_t *raiz, int valor){
	if(raiz == NULL){// valor não existe
	    printf("valor não existe!!\n");
	    return 0;
	}

	if(valor < raiz->info){
        raiz->esq = remove_ArvAVL(raiz->esq,valor);
	    if(raiz!= NULL){
            if(fatorBalanceamento_NO(raiz) >= 2){
                if(altura_NO(raiz->dir->esq) <= altura_NO(raiz->dir->dir))
                    raiz = RotacaoRR(raiz);
                else
                    raiz = RotacaoRL(raiz);
            }
	    }
	}

	if(raiz->info < valor){
        raiz->dir = remove_ArvAVL(raiz->dir,valor);
	    if(raiz != NULL){
            if(fatorBalanceamento_NO(raiz) >= 2){
                if(altura_NO(raiz->esq->dir) <= altura_NO(raiz->esq->esq) )
                    raiz = RotacaoLL(raiz);
                else
                    raiz = RotacaoLR(raiz);
            }
	    }
	}

	if(raiz->info == valor){
	    if((raiz->esq == NULL || raiz->dir == NULL)){/* Nó tem 1 filho ou nenhum */
			ArvAVL_t *oldNode = raiz;
			if(raiz->esq != NULL)
                raiz = raiz->esq;
            else
                raiz = raiz->dir;
			free(oldNode);
		}
		else { /* Nó tem 2 filhos */
			ArvAVL_t *temp = procuraMenor(raiz->esq);
			raiz->info = temp->info;
			raiz->esq = remove_ArvAVL(raiz->esq, raiz->info);
            if(fatorBalanceamento_NO(raiz) >= 2){
				if(altura_NO(raiz->esq->dir) <= altura_NO(raiz->esq->esq))
					raiz = RotacaoLL(raiz);
				else
					raiz = RotacaoLR(raiz);
			}
		}
		if (raiz != NULL)
            raiz->altura = maior(altura_NO(raiz->esq),altura_NO(raiz->dir)) + 1;
		return raiz;
	}

	raiz->altura = maior(altura_NO(raiz->esq),altura_NO(raiz->dir)) + 1;

	return raiz;
}


void print_arvore(ArvAVL_t *raiz, int espaco)
{
    if (raiz == NULL)
        return;

    espaco += TAM_ESPACO;

    /* Percorre até nó mais a direita */
    print_arvore(raiz->dir, espaco);

    printf("\n");
    int i;
    for (i = TAM_ESPACO; i < espaco; i++)
        printf(" ");
    printf("%d(%d)\n", raiz->info, raiz->altura);

    print_arvore(raiz->esq, espaco);
}

int main()
{
    /****EXEMPLOS****/

    /** Rotacao esquerda dupla **/
//    int flag = 0;
//
//    ArvAVL_t *raiz = criar_avl(7);
//    raiz = insere_ArvAVL(raiz, 4, &flag);
//    raiz = insere_ArvAVL(raiz, 12, &flag);
//    raiz = insere_ArvAVL(raiz, 6, &flag);
//    raiz = insere_ArvAVL(raiz, 5, &flag);
//
//    printf("\n\n--------------------------------------------\n\n");
//    printf("\n\nArvore completa:\n");
//    print_arvore(raiz, 0);
//
//    printf("\n\n--------------------------------------------\n\n");
//    printf("\n\nRemocao com 2 filhos:\n");
//
//    raiz = remove_ArvAVL(raiz, 5);
//    print_arvore(raiz, 0);
//
//    printf("\n\n--------------------------------------------\n\n");
//    printf("\n\nRemocao com 1 filho:\n");
//
//    raiz = remove_ArvAVL(raiz, 4);
//    print_arvore(raiz, 0);
//
//    printf("\n\n--------------------------------------------\n\n");
//    printf("\n\nRemocao sem filhos:\n");
//
//    raiz = remove_ArvAVL(raiz, 12);
//    print_arvore(raiz, 0);
//
//    printf("\n\n--------------------------------------------\n\n");
//    printf("\n\nRemocao de valor nao existente:\n");
//
//    raiz = remove_ArvAVL(raiz, 12);
//    print_arvore(raiz, 0);
//
//    printf("\n\n--------------------------------------------\n\n");
//    printf("\n\nInserçao de repetido:\n");
//
//    raiz = insere_ArvAVL(raiz, 6, &flag);
//    print_arvore(raiz, 0);

    /** Rotacao direita dupla **/
//        int flag = 0;
//
//        ArvAVL_t *raiz = criar_avl(8);
//        raiz = insere_ArvAVL(raiz, 6, &flag);
//        raiz = insere_ArvAVL(raiz, 12, &flag);
//        raiz = insere_ArvAVL(raiz, 9, &flag);
//        raiz = insere_ArvAVL(raiz, 10, &flag);
//
//        //printf("\n\n--------------------------------------------\n\n");
//        printf("\n\nArvore completa:\n");
//        print_arvore(raiz, 0);
//
//        printf("\n\n--------------------------------------------\n\n");
//        printf("\n\nRemocao com 2 filhos:\n");
//
//        raiz = remove_ArvAVL(raiz, 10);
//        print_arvore(raiz, 0);

    /** Rotacao simples direita **/
//        int flag = 0;
//
//        ArvAVL_t *raiz = criar_avl(5);
//        raiz = insere_ArvAVL(raiz, 4, &flag);
//        raiz = insere_ArvAVL(raiz, 1, &flag);
//
//        //printf("\n\n--------------------------------------------\n\n");
//        printf("\n\nArvore completa:\n");
//        print_arvore(raiz, 0);
//
//        printf("\n\n--------------------------------------------\n\n");
//        printf("\n\nRemocao com 2 filhos:\n");
//
//        raiz = remove_ArvAVL(raiz, 4);
//        print_arvore(raiz, 0);

    /** Rotacao simples esquerda **/
//        int flag = 0;
//
//        ArvAVL_t *raiz = criar_avl(5);
//        raiz = insere_ArvAVL(raiz, 6, &flag);
//        raiz = insere_ArvAVL(raiz, 8, &flag);
//
//        printf("\n\n--------------------------------------------\n\n");
//        printf("\n\nArvore completa:\n");
//        print_arvore(raiz, 0);
//
//        printf("\n\n--------------------------------------------\n\n");
//        printf("\n\nRemocao com 2 filhos:\n");
//
//        raiz = remove_ArvAVL(raiz, 6);
//        print_arvore(raiz, 0);


    return 0;
}
