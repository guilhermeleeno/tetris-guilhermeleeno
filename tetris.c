//Tetris novato

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5

// struct da peça
typedef struct {
    char nome;   // tipo: i, o, t, l
    int id;      // identificador único
} Peca;

// struct da fila circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// protótipos
void inicializarFila(Fila *f);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
Peca gerarPeca();
void enqueue(Fila *f, Peca nova);
Peca dequeue(Fila *f);
void exibirFila(Fila *f);

// contador global para os ids
int contadorID = 0;

int main() {
    Fila fila;
    int opcao;

    srand(time(NULL));
    inicializarFila(&fila);

    // preenchendo fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    do {
        printf("\nFila de peças:\n");
        exibirFila(&fila);

        printf("\n1 - Jogar peça\n");
        printf("2 - Inserir nova peça\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            if (!filaVazia(&fila)) {
                Peca removida = dequeue(&fila);
                printf("\nPeça jogada: [%c %d]\n", removida.nome, removida.id);
            } else {
                printf("\nFila vazia\n");
            }

        } else if (opcao == 2) {
            if (!filaCheia(&fila)) {
                Peca nova = gerarPeca();
                enqueue(&fila, nova);
                printf("\nPeça inserida: [%c %d]\n", nova.nome, nova.id);
            } else {
                printf("\nFila cheia\n");
            }

        } else if (opcao == 0) {
            printf("\nSaindo...\n");

        } else {
            printf("\nOpção inválida\n");
        }

    } while (opcao != 0);

    return 0;
}

// inicializa a fila circular
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

// verifica se fila está cheia
int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

// verifica se fila está vazia
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

// insere no final da fila
void enqueue(Fila *f, Peca nova) {
    if (filaCheia(f)) return;

    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = nova;
    f->quantidade++;
}

// remove da frente da fila
Peca dequeue(Fila *f) {
    Peca removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

// exibe a fila atual
void exibirFila(Fila *f) {
    if (filaVazia(f)) {
        printf("vazia\n");
        return;
    }

    int i = f->frente;
    for (int c = 0; c < f->quantidade; c++) {
        printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

// gera uma peça nova automaticamente
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;

    p.nome = tipos[rand() % 4];
    p.id = contadorID++;

    return p;
}
