#include <stdio.h>
#include <stdlib.h>

// Definição do nó da lista
typedef struct Node {
    int valor;
    struct Node* proximo;
} Node;

// Inserir no início
Node* inserir_inicio(Node* head, int valor) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->valor = valor;
    novo->proximo = head;
    return novo;
}

// Imprimir lista
void imprimir_lista(Node* head) {
    Node* atual = head;
    while (atual != NULL) {
        printf("%d -> ", atual->valor);
        atual = atual->proximo;
    }
    printf("NULL\n");
}

int main() {
    Node* lista = NULL;

    lista = inserir_inicio(lista, 10);
    lista = inserir_inicio(lista, 20);
    lista = inserir_inicio(lista, 30);

    imprimir_lista(lista);

    return 0;
}
