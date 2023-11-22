//Nome: Alana Cristina Lima de Oliveira 	Matrícula: 495702
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef enum {
	R, N
} COR;

//Estrutura da RN
typedef struct NO {
	int key;
	COR cor;
	struct NO *pai, *esq, *dir;
} NO;

typedef struct {
	NO *raiz, *externo;
} Arvore;

Arvore CriarRN() {
	Arvore arvore;

	arvore.externo = malloc(sizeof(NO));
	arvore.externo->cor = N;
	arvore.raiz = arvore.externo;

	return arvore;
}

// Rotacao pra esquerda
void RotacaoE(Arvore *arvore, NO *x) {
	// y armazena o filho a direita de x
	NO *y = x->dir;
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

// Rotacao pra direita
void RotacaoD(Arvore *arvore, NO *y) {
	NO *x = y->esq;
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

void RotaRN(Arvore *arvore, NO *z) {
	// interar ate que z nao seja mais raiz e a cor do pai seja rubro
	while(z->pai->cor == R) {
		 // encontra o tio e armazena em y
		if(z->pai == z->pai->pai->esq) {
			NO *y = z->pai->pai->dir;

			// se o tio for rubro
			if(y->cor == R) {
				z->pai->cor = N;
				y->cor = N; // troca a cor do tio e a do pai pra negra
				z->pai->pai->cor = R; // muda a cor do avo pra rubro
				z = z->pai->pai; // move z para o avo
			} 
			else {
				// se o tio for negro
				if(z == z->pai->dir) {
					// LR
                    // Troca a cor do no atual com a do avo
					z = z->pai;
					RotacaoE(arvore, z); // gira o pai pra esquerda
				}
				// LL
                // Troca a cor do pai com a do avo
				z->pai->cor = N;
				z->pai->pai->cor = R;
				RotacaoD(arvore, z->pai->pai); // gira o avo pra direita
			}
		} else {
			NO *y = z->pai->pai->esq;

			if(y->cor == R) {
				z->pai->cor = N;
				y->cor = N;
				z->pai->pai->cor = R;
				z = z->pai->pai;
			} else {
				if(z == z->pai->esq) {
					// RL
					z = z->pai;
					RotacaoD(arvore, z); // gira o pai pra direita
				}
				// RR
				z->pai->cor = N; // troca a cor do pai com a do avo
				z->pai->pai->cor = R;
				RotacaoE(arvore, z->pai->pai); // gira o avo pra esquerda
			}
		}
	}

	arvore->raiz->cor = N; // cor da raiz vai ser sempre negra
}

void InserirRN(Arvore *arvore, int key) {
	NO *x = arvore->raiz, *y = arvore->externo;

	// Etapas padrao pra insercao do primeiro no
	while(x != arvore->externo) {
		y = x;

		if(key < x->key)
			x = x->esq;
		else
			x = x->dir;
	}
	// Alocar memoria pro novo no
	NO *z = malloc(sizeof(NO));
	z->key = key;
	z->cor = R;
	z->pai = y;
	z->esq = arvore->externo;
	z->dir = arvore->externo;

	if(y == arvore->externo) //se a raiz for nula, faz de z a raiz
		arvore->raiz = z;
	else if(key < y->key)
		y->esq = z;
	else
		y->dir = z;

	RotaRN(arvore, z); // verifica se nenhuma regra da RN esta sendo violada
}

NO *BuscarNo(NO *aux, NO *externo, int key) {
	if(aux == externo)
		return externo;
	else if(aux->key > key)
		return BuscarNo(aux->esq, externo, key);
	else if(aux->key < key)
		return BuscarNo(aux->dir, externo, key);
	else
		return aux;
}

void MoverPai(Arvore *arvore, NO *u, NO *v) {  // funcao pra substituir nos
	if(u->pai == arvore->externo)
		arvore->raiz = v;
	else if(u == u->pai->esq) // se U for o filho da esquerda
		u->pai->esq = v;
	else
		u->pai->dir = v; // se U for filho da direita

	v->pai = u->pai;
}

NO *Sucessor(Arvore *arvore, NO *x) {
	while(x->esq != arvore->externo)
		x = x->esq;

	return x;
}

void RotaRemoverRN(Arvore *arvore, NO *x) {
	while(x != arvore->raiz && x->cor == N) {
		if(x == x->pai->esq) {
			NO *w = x->pai->dir;

			if(w->cor == R) {
				w->cor = N;
				x->pai->cor = R;
				RotacaoE(arvore, x->pai);
				w = x->pai->dir;
			}

			if(w->esq->cor == N && w->dir->cor == N) {
				w->cor = R;
				x = x->pai;
			} else {
				if(w->dir->cor == N) {
					w->esq->cor = N;
					w->cor = R;
					RotacaoD(arvore, w);
					w = x->pai->dir;
				}

				w->cor = x->pai->cor;
				x->pai->cor = N;
				w->dir->cor = N;
				RotacaoE(arvore, x->pai);
				x = arvore->raiz;
			}
		} else {
			NO *w = x->pai->esq;

			if(w->cor == R) {
				w->cor = N;
				x->pai->cor = R;
				RotacaoD(arvore, x->pai);
				w = x->pai->esq;
			}

			if(w->dir->cor == N && w->esq->cor == N) {
				w->cor = R;
				x = x->pai;
			} else {
				if(w->esq->cor == N) {
					w->dir->cor = N;
					w->cor = R;
					RotacaoE(arvore, w);
					w = x->pai->esq;
				}

				w->cor = x->pai->cor;
				x->pai->cor = N;
				w->esq->cor = N;
				RotacaoD(arvore, x->pai);
				x = arvore->raiz;
			}
		}
	}

	x->cor = N;
}

void RemoverRN(Arvore *arvore, int key) {
	NO *z = BuscarNo(arvore->raiz, arvore->externo, key);

	if(z == arvore->externo)
		return;

	NO *y = z, *x;
	COR y_original_cor = y->cor;

	if(z->esq == arvore->externo) {
		x = z->dir;
		MoverPai(arvore, z, z->dir);
	} else if(z->dir == arvore->externo) {
		x = z->esq;
		MoverPai(arvore, z, z->esq);
	} else {
		y = Sucessor(arvore, z->dir);
		y_original_cor = y->cor;
		x = y->dir;

		if(y->pai == z)
			x->pai = y;
		else {
			MoverPai(arvore, y, y->dir);
			y->dir = z->dir;
			y->dir->pai = y;
		}

		MoverPai(arvore, z, y);
		y->esq = z->esq;
		y->esq->pai = y;
		y->cor = z->cor;
	}

	if(y_original_cor == N)
		RotaRemoverRN(arvore, x);

	free(z);
}

void Transversal(NO *aux, NO *externo) {
	if(aux != externo) {
		Transversal(aux->esq, externo);
		Transversal(aux->dir, externo);
		free(aux);
	}
}

void LiberarArvore(Arvore *arvore) {
	Transversal(arvore->raiz, arvore->externo);
	free(arvore->externo);
}


int *BuscarChave(int qtd_chave) {
	int *keys = malloc(sizeof(int) * qtd_chave);

	srand(time(NULL));

	for(int i = 0; i < qtd_chave; ) {
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

int TransversalNegra(NO *aux, NO *externo) {
	if(aux == externo)
		return 1;

	int esq_altura = TransversalNegra(aux->esq, externo),
	    dir_altura = TransversalNegra(aux->dir, externo);

	if(esq_altura != dir_altura || esq_altura == -1)
		return -1;
	else
		return esq_altura + (aux->cor == N? 1 : 0);
}

int AlturaNegra(NO *aux, NO *externo) {
	if(aux == externo)
		return 1;

	int esq_altura = TransversalNegra(aux->esq, externo),
	    dir_altura = TransversalNegra(aux->dir, externo);

	if(esq_altura != dir_altura || esq_altura == -1)
		return -1;
	else
		return esq_altura;
}

bool VerificarRubroNegra(NO *aux, NO *externo) {
	if(aux == externo)
		return true;

	if(AlturaNegra(aux, externo) != -1)
		if(VerificarRubroNegra(aux->esq, externo))
			return VerificarRubroNegra(aux->dir, externo);

	return false;
}

int ContarNos(NO *aux, NO *externo) {
	if(aux == externo)
		return 0;

	return 1 + ContarNos(aux->esq, externo) + ContarNos(aux->dir, externo);
}

int main() {
	int qnt_arvore = 1;
	int cont = 0;
	for(cont = 0; cont<qnt_arvore; cont++){
		Arvore arvore = CriarRN();
		const int qtd_chave = 100;
		int *keys = BuscarChave(qtd_chave);
		puts("-------------------------------- ");
		printf("\nArvore RN %d\n", cont + 1);
		printf("*Inserindo %d nós.*\n", qtd_chave);
		for(int i = 0; i < qtd_chave; i++)
		InserirRN(&arvore, keys[i]);

		int qtd_nos = ContarNos(arvore.raiz, arvore.externo);

		if(qtd_chave == qtd_nos)
			puts("Todos os nós foram inseridos.");
		else {
			printf("%d nós foram inseridos.\n", qtd_nos);
			exit(EXIT_FAILURE);
		}

		if(VerificarRubroNegra(arvore.raiz, arvore.externo))
			puts("A árvore é RN.");
		else {
			puts("A árvore não é RN");
			exit(EXIT_FAILURE);
		}

		int qtd_remover = 10;
		printf("*Removendo %d nós.*\n", qtd_remover);
		for(int i = 0; i < qtd_chave; i +=10)
			RemoverRN(&arvore, keys[i]);

		qtd_nos = ContarNos(arvore.raiz, arvore.externo);

		if(qtd_chave - qtd_remover == qtd_nos)
			printf("Todos os nós foram removidos. Nós atuais: %d\n", qtd_nos);
		else {
			printf("%d nós ainda estão na árvore.\n", qtd_nos);
			exit(EXIT_FAILURE);
		}

		if(VerificarRubroNegra(arvore.raiz, arvore.externo))
			puts("A árvore é RN após as remoções.");
		else {
			puts("A árvore não é RN após as remoções");
			exit(EXIT_FAILURE);
		}

		free(keys);
		LiberarArvore(&arvore);
	}
	return 0;
}
