// tetris nivel mestre

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// struct peças
typedef struct {
    char nome;
    int id;
} Peca;

// struct fila circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// struct pilha
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

int contadorID = 0;

// cria uma peça nova
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

// inicializa fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

// inicializa pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// verifica se cheio/vazio
int filaCheia(Fila *f) { return f->quantidade == TAM_FILA; }
int filaVazia(Fila *f) { return f->quantidade == 0; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

// insere na fila
void enqueue(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = nova;
    f->quantidade++;
}

// remove da fila
Peca dequeue(Fila *f) {
    Peca removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

// empilhar
void push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = nova;
}

// desempilhar
Peca pop(Pilha *p) {
    Peca removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// exibir fila
void exibirFila(Fila *f) {
    if (filaVazia(f)) {
        printf("fila vazia\n");
        return;
    }
    int i = f->frente;
    for (int c = 0; c < f->quantidade; c++) {
        printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
        i = (i + 1) % TAM_FILA;
    }
}

// exibir pilha
void exibirPilha(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("pilha vazia\n");
        return;
    }
    printf("(topo -> base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
}

// troca 1x1 entre fila e pilha
void trocaSimples(Fila *f, Pilha *p) {
    Peca temp = f->itens[f->frente];
    f->itens[f->frente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
}

// troca múltipla (3 da fila com 3 da pilha)
void trocaMultiplas(Fila *f, Pilha *p) {
    int idxFila = f->frente;
    int idxPilha = p->topo;

    for (int i = 0; i < 3; i++) {
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[idxPilha];
        p->itens[idxPilha] = temp;

        idxFila = (idxFila + 1) % TAM_FILA;
        idxPilha--;
    }
}


// agora implementando o menu e as ações no main
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // preenche fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    do {
        printf("\n==========================\n");
        printf("estado atual:\n\n");

        printf("fila de peças\t");
        exibirFila(&fila);
        printf("\n");

        printf("pilha de reserva\t");
        exibirPilha(&pilha);
        printf("\n");

        printf("\n1 - Jogar peça\n");
        printf("2 - Enviar peça da fila para a pilha\n");
        printf("3 - Usar peça da pilha\n");
        printf("4 - Trocar peça da fila com topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            if (!filaVazia(&fila)) {
                Peca jogada = dequeue(&fila);
                printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca());
            } else {
                printf("\nfila vazia\n");
            }

        } else if (opcao == 2) {
            if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                Peca movida = dequeue(&fila);
                push(&pilha, movida);
                printf("\nPeça enviada para pilha: [%c %d]\n", movida.nome, movida.id);
                enqueue(&fila, gerarPeca());
            } else {
                printf("\nNão foi possível enviar\n");
            }

        } else if (opcao == 3) {
            if (!pilhaVazia(&pilha)) {
                Peca usada = pop(&pilha);
                printf("\nPeça usada: [%c %d]\n", usada.nome, usada.id);
            } else {
                printf("\npilha vazia\n");
            }

        } else if (opcao == 4) {
            if (!filaVazia(&fila) && !pilhaVazia(&pilha)) {
                trocaSimples(&fila, &pilha);
                printf("\ntroca simples realizada\n");
            } else {
                printf("\nnão foi possível trocar\n");
            }

        } else if (opcao == 5) {
            if (fila.quantidade >= 3 && pilha.topo >= 2) {
                trocaMultiplas(&fila, &pilha);
                printf("\ntroca múltipla realizada\n");
            } else {
                printf("\nnão há peças suficientes para troca múltipla\n");
            }

        } else if (opcao == 0) {
            printf("\nSaindo...\n");
        } else {
            printf("\nopção inválida\n");
        }

    } while (opcao != 0);

    return 0;
}
