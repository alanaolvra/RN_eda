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

// Função para realizar rotação à esquerda
void RotacaoE(No *x, No **ptraiz) {
    No *y = x->dir;
    x->dir = y->esq;
    if (y->esq != NULL) {
        y->esq->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == NULL) {
        *ptraiz = y;
    } else if (x == x->pai->esq) {
        x->pai->esq = y;
    } else {
        x->pai->dir = y;
    }
    y->esq = x;
    x->pai = y;
}

// Função para realizar rotação à direita
void RotacaoD(No *y, No **ptraiz) {
    No *x = y->esq;
    y->esq = x->dir;
    if (x->dir != NULL) {
        x->dir->pai = y;
    }
    x->pai = y->pai;
    if (y->pai == NULL) {
        *ptraiz = x;
    } else if (y == y->pai->esq) {
        y->pai->esq = x;
    } else {
        y->pai->dir = x;
    }
    x->dir = y;
    y->pai = x;
}

// Função modificada RotaRN para incluir a rotação à esquerda e à direita
void RotaRN(No *z, No **ptraiz) {
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
                    RotacaoE(z, ptraiz);
                }
                z->pai->cor = 'B';
                z->pai->pai->cor = 'R';
                RotacaoD(z->pai->pai, ptraiz);
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
                    RotacaoD(z, ptraiz);
                }
                z->pai->cor = 'B';
                z->pai->pai->cor = 'R';
                RotacaoE(z->pai->pai, ptraiz);
            }
        }
    }
    (*ptraiz)->cor = 'B';
}

void InserirRN(No *z, No **ptraiz) {
    No *y = NULL;
    No *pt = *ptraiz;

    if (*ptraiz == NULL) {
            *ptraiz = z;
            z->cor = 'B';  // Se o nó é a raiz, ele deve ser preto
            return;
    }

    while (pt != NULL) {
        y = pt;
        if (z->chave == pt->chave) {
            printf("Chave existente\n");
            y = NULL;
            pt = NULL;
        } else if (z->chave < pt->chave) {
            pt = pt->esq;
        } else {
            pt = pt->dir;
        }
    }

    if (y != NULL) {
        z->pai = y;
        if (y == NULL) {
            *ptraiz = z;
        } else if (z->chave < y->chave) {
            y->esq = z;
        } else {
            y->dir = z;
        }
        z->esq = z->dir = NULL;
        z->cor = 'R';
        RotaRN(z, ptraiz);  // Chamada da função RotaRN
    }

}

void MoverPai(No *u, No *v, No **ptraiz) {
    if (u->pai == NULL) {
        *ptraiz = v;
    } else {
        if (u == u->pai->esq) {
            u->pai->esq = v;
        } else {
            u->pai->dir = v;
        }
        if (v != NULL) {
            v->pai = u->pai;
        }
    }
}

// Função para realizar a rotação após a remoção
void RotaRemoverRN(No *x, No **ptraiz) {
    while (x != *ptraiz && x->cor == 'N') {
        if (x == x->pai->esq) {
            No *w = x->pai->dir;
            if (w->cor == 'R') {
                w->cor = 'N';
                x->pai->cor = 'R';
                RotacaoE(x->pai, ptraiz);
                w = x->pai->dir;
            }
            if (w->esq->cor == 'N' && w->dir->cor == 'N') {
                w->cor = 'R';
                x = x->pai;
            } else {
                if (w->dir->cor == 'N') {
                    w->esq->cor = 'N';
                    w->cor = 'R';
                    RotacaoD(w, ptraiz);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = 'N';
            }
            w->dir->cor = 'N';
            RotacaoE(x->pai, ptraiz);
            x = *ptraiz;
        } else {
            No *w = x->pai->esq;
            if (w->cor == 'R') {
                w->cor = 'N';
                x->pai->cor = 'R';
                RotacaoD(x->pai, ptraiz);
                w = x->pai->esq;
            }
            if (w->dir->cor == 'N' && w->esq->cor == 'N') {
                w->cor = 'R';
                x = x->pai;
            } else {
                if (w->esq->cor == 'N') {
                    w->dir->cor = 'N';
                    w->cor = 'R';
                    RotacaoE(w, ptraiz);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = 'N';
            }
            w->esq->cor = 'N';
            RotacaoD(x->pai, ptraiz);
            x = *ptraiz;
        }
    }
    x->cor = 'N';
}

// Função para encontrar o sucessor de um nó na árvore
No* Sucessor(No* x) {
    while (x->esq != NULL) {
        x = x->esq;
    }
    return x;
}

// Função para remover um nó da árvore vermelho-preto
void RemoverRN(No* z, No** ptraiz) {

    if (z == *ptraiz) {
        return; // Não há o que remover se o nó é nulo
    }
    No *y, *x;
    y = z;
    char corOriginal = y->cor;  
    if (y->esq == NULL) {
        x = z->dir;
        MoverPai(z, z->dir, ptraiz);
    } else if (y->dir == NULL) {
        x = z->esq;
        MoverPai(z, z->esq, ptraiz);
    } else {
        y = Sucessor(z);
        corOriginal = y->cor;
        x = y->dir;
        if (y->pai != z) {
            MoverPai(y, x, ptraiz);
            y->dir = z->dir;
            y->pai->dir = y;
        }
        MoverPai(z, y, ptraiz);
        y->esq = z->esq;
        y->esq->pai = y;
    }

    if (corOriginal == 'N') {
        if (x != NULL) {
            x->cor = 'N';
        }
        RotaRemoverRN(x, ptraiz);
    } else {
        z->cor = 'N';
    }

    if (z == *ptraiz) {
        *ptraiz = x;
        if (x != NULL) {
            x->cor = 'N';
        }
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
int VerificarRubroNegra(No *raiz) {
    if (AlturaNegra(raiz) == -1) {
        return 0;  // Não é Rubro-Negra
    } else {
        return 1;  // É Rubro-Negra
    }
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
int ContarNosRubroNegra(No *raiz) {
    if (raiz == NULL) 
        return 0;

    int nosEsq = 0;
    int nosDir = 0;

    if (raiz->esq != NULL) {
        nosEsq = ContarNosRubroNegra(raiz->esq);
    }

    if (raiz->dir != NULL) {
        nosDir = ContarNosRubroNegra(raiz->dir);
    }

    int x = 1 + nosEsq + nosDir;
    return x;   
}

// Função para liberar a memória alocada para a árvore
void LiberarArvore(No* raiz) {
    if (raiz != NULL) {
        LiberarArvore(raiz->esq);
        LiberarArvore(raiz->dir);
        free(raiz);
    }
}

No* BuscarNo(No* raiz, int chave) {
    while (raiz != NULL && raiz->chave != chave) {
        if (chave < raiz->chave) {
            raiz = raiz->esq;
        } else {
            raiz = raiz->dir;
        }
    }
    return raiz;
}


int main() {
    srand(time(NULL));

    int totalArvores = 1;
    int totalInsercoes = 100;
    int totalRemocoes = 10;

    for (int i = 0; i < totalArvores; ++i) {
        No *raiz = NULL;
        No **arrayNos = malloc(totalInsercoes * sizeof(No *));
        int totalInsercoesAtual = 0;

        // Inserir aleatoriamente 10.000 nós no array
        for (int j = 0; j < totalInsercoes; ++j) {
            int chave = rand() % 100001; // Chaves entre 0 e 100.000
            No *novoNo = NovoNo(chave, 'R'); // Criar um novo nó com a chave
            InserirRN(novoNo, &raiz);
            arrayNos[totalInsercoesAtual] = novoNo;
            ++totalInsercoesAtual;
        }

        // Verificar se a árvore possui os 10.000 nós pela contagem
        int quantidadeNosInseridos = ContarNosRubroNegra(raiz);
        printf("Arvore %d - Nós inseridos: %d\n", i + 1, quantidadeNosInseridos);

        // Verificar se a árvore continua Rubro-Negra após inserções
        if (VerificarRubroNegra(raiz)) {
            printf("Arvore %d - Continua Rubro-Negra após inserções.\n", i + 1);
        } else {
            printf("Arvore %d - Não é Rubro-Negra após inserções.\n", i + 1);
            break;
        }

        // Remover 1.000 nós aleatórios do array
        for (int k = 0; k < totalRemocoes; ++k) {
            if (totalInsercoesAtual > 0) {
                int indiceAnterior, indiceRemover;
                do {
                     indiceRemover = rand() % totalInsercoes;
                } while (indiceRemover == indiceAnterior);
                indiceAnterior = indiceRemover;
                No *noRemover = arrayNos[indiceRemover];
                RemoverRN(noRemover, &raiz);
                // Atualizar o arrayNos e o totalInsercoesAtual após a remoção
                for (int l = indiceRemover; l < totalInsercoesAtual - 1; ++l) {
                    arrayNos[l] = arrayNos[l + 1];
                }
                --totalInsercoesAtual;
            }
        }

        // Verificar se a árvore possui os 9.000 nós após remoções
        int quantidadeNosRemovidos = ContarNosRubroNegra(raiz);
        printf("Arvore %d - Nós restantes após remoções: %d\n", i + 1, quantidadeNosRemovidos);

        // Verificar se a árvore continua Rubro-Negra após remoções
        if (!VerificarRubroNegra(raiz)) {
            printf("Arvore %d - Não é Rubro-Negra após remoções.\n", i + 1);
        } else {
            printf("Arvore %d - Continua Rubro-Negra após remoções.\n", i + 1);
        }

        // Liberar a memória alocada para o array e para a árvore
        free(arrayNos);
        LiberarArvore(raiz);
    }

    return 0;
}