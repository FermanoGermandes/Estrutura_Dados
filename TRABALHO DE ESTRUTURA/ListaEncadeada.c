#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct EletricDates {
    char data[24]; 
    double demanda_residual; 
    double demanda_contratada;
    double geracao_despachavel;
    double geracao_termica;
    double importacoes;
    double geracao_renovavel_total;
    double carga_reduzida_manual;
    double capacidade_instalada;
    double perdas_geracao_total;
} EletricDates; 

typedef struct Node {
    EletricDates dado;
    struct Node* next;
} Node;

Node* inicializar_lista() {
    return NULL;
}

Node* inserir_dado (Node* head, EletricDates dado){
    Node* novo = malloc(sizeof(Node));
    novo->dado = dado;
    novo->next = NULL;

    if(head == NULL){
        return novo;
    }

    Node* atual = head;
    while (atual->next != NULL) {
        atual = atual->next;
    }

    atual->next = novo;
    return head;  
}










