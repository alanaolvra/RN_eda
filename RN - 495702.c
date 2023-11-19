#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Definição da estrutura de um nó da árvore vermelho-preto
typedef struct No {
    int chave;
    char cor; // 'R' para vermelho, 'B' para preto
    struct No *esq, *dir, *pai;
} No;

typedef struct {
	No *raiz, *externo;
} Arvore;

// Função para realizar rotação à esquerda
void RotacaoE(No *x, Arvore *arvore) {
    No *y = x->dir;
	x->dir = y->esq;

	if(y->esq != arvore->externo) // atualiza o no a esquerda do pai
		y->esq->pai = x;

	y->pai = x->pai; //atualiza o no pai

	if(x->pai == arvore->externo) // se o pai de x for nulo, deixa y como raiz
		arvore->raiz = y;
	else if(x == x->pai->esq) // armazena y no lugar de x
		x->pai->esq = y;
	else
		x->pai->dir = y;

	y->esq = x; // faz x ser filho esquerdo de y
	x->pai = y; // atualiza o pai de x
}

// Função para realizar rotação à direita
void RotacaoD(No *y, Arvore *arvore) {
    No *x = y->esq;
	y->esq = x->dir;

	if(x->dir != arvore->externo)
		x->dir->pai = y;

	x->pai = y->pai;

	if(x->pai == arvore->externo)
		arvore->raiz = x;
	else if(y == y->pai->esq)
		y->pai->esq = x;
	else
		y->pai->dir = x;

	x->dir = y;
	y->pai = x;
}

// Função modificada RotaRN para incluir a rotação à esquerda e à direita
void RotaRN(No *z, Arvore *arvore) {
    while (z->pai != NULL && z->pai->cor == 'R') {
        if (z->pai == z->pai->pai->esq) {
            No *y = z->pai->pai->dir;
            if (y != NULL && y->cor == 'R') {
                z->pai->cor = 'B';
                y->cor = 'B';
                z->pai->pai->cor = 'R';
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    RotacaoE(z, arvore);
                }
                z->pai->cor = 'B';
                z->pai->pai->cor = 'R';
                RotacaoD(z->pai->pai, arvore);
            }
        } else {
            No *y = z->pai->pai->esq;
            if (y != NULL && y->cor == 'R') {
                z->pai->cor = 'B';
                y->cor = 'B';
                z->pai->pai->cor = 'R';
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    RotacaoD(z, arvore);
                }
                z->pai->cor = 'B';
                z->pai->pai->cor = 'R';
                RotacaoE(z->pai->pai, arvore);
            }
        }
    }
    arvore->raiz->cor = 'B';
}

void InserirRN(Arvore *arvore, int chave) {
    No *y = arvore->externo;
    No *pt = arvore->raiz;
    
    No *z = malloc(sizeof(No));
	z->chave = chave;
	z->cor = 'R';
	z->pai = y;
	z->esq = arvore->externo;
	z->dir = arvore->externo;

    while (pt != arvore->externo) {
        y = pt;
        if (z->chave == pt->chave) {
            printf("Chave existente\n");
            y = NULL;
            pt = arvore->externo;
        }else{
            if (z->chave < pt->chave) {
            pt = pt->esq;
        } else {
            pt = pt->dir;
        } 
    }

    if (y != NULL) {
        z->pai = y;
        if (y == arvore->externo) {
            arvore->raiz = z;
        } else{
            if (z->chave < y->chave) {
                y->esq = z;
            } else {
                y->dir = z;
            }
        } 
        z->esq = z->dir = arvore->externo;
        z->cor = 'R';
        RotaRN(z, arvore);  // Chamada da função RotaRN
        }
    }
}

void MoverPai(No *u, No *v, Arvore *arvore) {
    if (u->pai == arvore->externo) {
        arvore->raiz = v;
    } else {
        if (u == u->pai->esq) {
            u->pai->esq = v;
        } else {
            u->pai->dir = v;
        }     
    }
    v->pai = u->pai;     
}

// Função para realizar a rotação após a remoção
void RotaRemoverRN(No *x, Arvore *arvore) {
    while(x != arvore->raiz && x->cor == 'N') {
		if(x == x->pai->esq) {
			No *w = x->pai->dir;

			if(w->cor == 'R') {
				w->cor = 'N';
				x->pai->cor = 'R';
				RotacaoE(x->pai, arvore);
				w = x->pai->dir;
			}

			if(w->esq->cor == 'N' && w->dir->cor == 'N') {
				w->cor = 'R';
				x = x->pai;
			} else {
				if(w->dir->cor == 'N') {
					w->esq->cor = 'N';
					w->cor = 'R';
					RotacaoD(w, arvore);
					w = x->pai->dir;
				}

				w->cor = x->pai->cor;
				x->pai->cor = 'N';
				w->dir->cor = 'N';
				RotacaoE(x->pai, arvore);
				x = arvore->raiz;
			}
		} else {
			No *w = x->pai->esq;

			if(w->cor == 'R') {
				w->cor = 'N';
				x->pai->cor = 'R';
				RotacaoD(x->pai, arvore);
				w = x->pai->esq;
			}

			if(w->dir->cor == 'N' && w->esq->cor == 'N') {
				w->cor = 'R';
				x = x->pai;
			} else {
				if(w->esq->cor == 'N') {
					w->dir->cor = 'N';
					w->cor = 'R';
					RotacaoE(w, arvore);
					w = x->pai->esq;
				}

				w->cor = x->pai->cor;
				x->pai->cor = 'N';
				w->esq->cor = 'N';
				RotacaoD(x->pai, arvore);
				x = arvore->raiz;
			}
		}
    }
}

// Função para encontrar o sucessor de um nó na árvore
No* Sucessor(No* x) {
    while (x->esq != NULL) {
        x = x->esq;
    }
    return x;
}

// Função para remover um nó da árvore vermelho-preto
void RemoverRN(Arvore* arvore, int chave) {
    No* z;
    if(z == arvore->externo)
		return;

    No *y, *x;
    y = z;
    char corOriginal = y->cor;  

    if (y->esq == arvore->externo) {
        x = z->dir;
        MoverPai(z, z->dir, arvore);
    } else{
         if (y->dir == arvore->externo) {
            x = z->esq;
            MoverPai(z, z->esq, arvore);
        }else {
            y = Sucessor(z);
            corOriginal = y->cor;
            x = y->dir;
            if (y->pai != z) {
                MoverPai(y, x, arvore);
                y->dir = z->dir;
                y->pai->dir = y;
            }
            MoverPai(z, y, arvore);
            y->esq = z->esq;
            y->esq->pai = y;
        }
    }

    if (corOriginal == 'N') {
        RotaRemoverRN(x, arvore);
    }
}


// Função para calcular a altura negra de uma subárvore
int AlturaNegra(No *raiz) {
    if (raiz == NULL) {
        return 1;
    } else {
        int alturaEsq = AlturaNegra(raiz->esq);
        int alturaDir = AlturaNegra(raiz->dir);

        if (alturaEsq != alturaDir || (raiz->cor == 'R' && (raiz->esq == NULL || raiz->esq->cor == 'R'))) {
            // Verifica se as alturas negras são iguais e se não há nós vermelhos consecutivos
            return -1;
        } else {
            return alturaEsq + (raiz->cor == 'B' ? 1 : 0);
        }
    }
}

// Função para verificar se uma árvore é Rubro-Negra
int VerificarRubroNegra(No *aux, No *externo) {
	if(aux == externo)
		return true;
	if(VerificarRubroNegra(aux->esq, externo))
		return VerificarRubroNegra(aux->dir, externo);

	return false;
}

// Função para criar um novo nó
No* NovoNo(int chave, char cor) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        fprintf(stderr, "Erro na alocação de memória.\n");
        exit(EXIT_FAILURE);
    }
    novo->chave = chave;
    novo->cor = cor;
    novo->esq = novo->dir = novo->pai = NULL;
    return novo;
}

// Função para contar a quantidade de nós em uma árvore Rubro-Negra
int ContarNosRubroNegra(No *aux, No *externo) {
    if(aux == externo)
		return 0;

	return 1 + ContarNosRubroNegra(aux->esq, externo) + ContarNosRubroNegra(aux->dir, externo);  
}

// Função para liberar a memória alocada para a árvore
void LiberarArvore(No* raiz) {
    if (raiz != NULL) {
        LiberarArvore(raiz->esq);
        LiberarArvore(raiz->dir);
        free(raiz);
    }
}

int* BuscarChave(int chave) {
   int *keys = malloc(sizeof(int) * chave);

	srand(time(NULL));

	for(int i = 0; i < chave; ) {
		bool chave_repetida = true;
		int nova_chave = rand();

		for(int j = i - 1; j >= 0 && chave_repetida; j--)
			if(nova_chave == keys[j])
				chave_repetida = false;

		if(chave_repetida) {
			keys[i] = nova_chave;
			i++;
		}
	}

	return keys;
}

 No *BuscarNo(No *aux, No *externo, int key) {
	if(aux == externo)
		return externo;
	else if(aux->chave > key)
		return BuscarNo(aux->esq, externo, key);
	else if(aux->chave < key)
		return BuscarNo(aux->dir, externo, key);

	return aux;
}

Arvore CriarRN() {
	Arvore arvore;

	arvore.externo = malloc(sizeof(No));
	arvore.externo->cor = 'N';
	arvore.raiz = arvore.externo;

	return arvore;
}

int main() {
    srand(time(NULL));

    int totalArvores = 1;
    int totalInsercoes = 10;
    int totalRemocoes = 3;

    for (int i = 0; i < totalArvores; ++i) {
        Arvore arvore = CriarRN();
        int *keys = BuscarChave(totalInsercoes);
        int totalInsercoesAtual = 0;

        // Inserir aleatoriamente 10.000 nós no array
        for(int i = 0; i < totalInsercoes; i++)
		    InserirRN(&arvore, keys[i]);
        

        // Verificar se a árvore possui os 10.000 nós pela contagem
        int quantidadeNosInseridos = ContarNosRubroNegra(arvore.raiz, arvore.externo);
        printf("Arvore %d - Nós inseridos: %d\n", i + 1, quantidadeNosInseridos);

        // Verificar se a árvore continua Rubro-Negra após inserções
        if (VerificarRubroNegra(arvore.raiz, arvore.externo)) {
            printf("Arvore %d - Continua Rubro-Negra após inserções.\n", i + 1);
        } else {
            printf("Arvore %d - Não é Rubro-Negra após inserções.\n", i + 1);
            break;
        }

        // Remover 1.000 nós aleatórios do array
       for(int i = 0; i < totalInsercoes; i += totalRemocoes)
			RemoverRN(&arvore, keys[i]);

		int quantidadeNosRestantes = ContarNosRubroNegra(arvore.raiz, arvore.externo);

        // Verificar se a árvore possui os 9.000 nós após remoções
        printf("Arvore %d - Nós restantes após remoções: %d\n", i + 1, quantidadeNosRestantes);

        // Verificar se a árvore continua Rubro-Negra após remoções
        if (!VerificarRubroNegra(arvore.raiz, arvore.externo)) {
            printf("Arvore %d - Não é Rubro-Negra após remoções.\n", i + 1);
        } else {
            printf("Arvore %d - Continua Rubro-Negra após remoções.\n", i + 1);
        }

        // Liberar a memória alocada para o array e para a árvore
        free(keys);
        LiberarArvore(arvore.raiz);
    }

    return 0;
}