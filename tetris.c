//Tetris aventureiro

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que define uma peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura para a Fila Circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int fim;
    int total;
} FilaCircular;

// Estrutura para a Pilha de Reserva
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} PilhaReserva;

// Variável global para garantir IDs únicos
int contadorId = 0;

// Funções de Inicialização e Geração das peças em random
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorId++;
    return p;
}

void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->fim = -1;
    f->total = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        f->fim = (f->fim + 1) % TAM_FILA;
        f->itens[f->fim] = gerarPeca();
        f->total++;
    }
}

void inicializarPilha(PilhaReserva *p) {
    p->topo = -1;
}

// Operações da Fila
Peca dequeue(FilaCircular *f) {
    Peca p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->total--;
    
    // Reposição automática: sempre que sai uma, entra uma nova
    f->fim = (f->fim + 1) % TAM_FILA;
    f->itens[f->fim] = gerarPeca();
    f->total++;
    
    return p;
}

// Operações da Pilha
int push(PilhaReserva *p, Peca peca) {
    if (p->topo < TAM_PILHA - 1) {
        p->itens[++(p->topo)] = peca;
        return 1; // Sucesso
    }
    return 0; // Pilha cheia
}

Peca pop(PilhaReserva *p) {
    return p->itens[(p->topo)--];
}

// Função para exibir o estado atual
void exibirEstado(FilaCircular f, PilhaReserva p) {
    printf("\n==========================================\n");
    printf("ESTADO ATUAL\n");
    printf("==========================================\n");
    
    // Exibir Fila
    printf("Fila de peças:    ");
    for (int i = 0; i < TAM_FILA; i++) {
        int indice = (f.frente + i) % TAM_FILA;
        printf("[%c %d] ", f.itens[indice].nome, f.itens[indice].id);
    }
    
    // Exibir Pilha
    printf("\nPilha de reserva (Topo -> Base): ");
    if (p.topo == -1) {
        printf("[Vazia]");
    } else {
        for (int i = p.topo; i >= 0; i--) {
            printf("[%c %d] ", p.itens[i].nome, p.itens[i].id);
        }
    }
    printf("\n------------------------------------------\n");
}

int main() {
    srand(time(NULL));
    FilaCircular fila;
    PilhaReserva pilha;
    int opcao = -1;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    while (opcao != 0) {
        exibirEstado(fila, pilha);
        printf("Opções de Ação:\n");
        printf("1 - Jogar peça (da fila)\n");
        printf("2 - Reservar peça (fila -> pilha)\n");
        printf("3 - Usar peça reservada (da pilha)\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = dequeue(&fila);
                printf("\n>> VOCÊ JOGOU A PEÇA: [%c %d]\n", jogada.nome, jogada.id);
                break;
            }
            case 2: {
                if (pilha.topo < TAM_PILHA - 1) {
                    // Remove da fila e coloca na pilha
                    Peca paraReserva = dequeue(&fila);
                    push(&pilha, paraReserva);
                    printf("\n>> PEÇA [%c %d] MOVIDA PARA A RESERVA.\n", paraReserva.nome, paraReserva.id);
                } else {
                    printf("\n!! ERRO: Pilha de reserva cheia!\n");
                }
                break;
            }
            case 3: {
                if (pilha.topo != -1) {
                    Peca reservada = pop(&pilha);
                    printf("\n>> VOCÊ USOU A PEÇA RESERVADA: [%c %d]\n", reservada.nome, reservada.id);
                } else {
                    printf("\n!! ERRO: Nenhuma peça na reserva!\n");
                }
                break;
            }
            case 0:
                printf("Encerrando jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    }

    return 0;
}
