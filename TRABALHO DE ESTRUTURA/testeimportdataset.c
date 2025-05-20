#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

// Struct para armazenar os dados do CSV
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

// Nó da lista encadeada
typedef struct Node {
    EletricDates dado;
    struct Node* proximo;
} Node;

// Inicializa lista (NULL = lista vazia)
Node* inicializar_lista() {
    return NULL;
}

// Insere nó no início da lista (mais simples para teste)
Node* inserir_inicio(Node* head, EletricDates dado) {
    Node* novo = (Node*) malloc(sizeof(Node));
    if (!novo) {
        printf("Erro de alocação\n");
        exit(1);
    }
    novo->dado = dado;
    novo->proximo = head;
    return novo;
}

// Imprime os dados da lista
void imprimir_lista(Node* head) {
    Node* atual = head;
    while (atual != NULL) {
        printf("Data: %s, Demanda Residual: %.2lf\n",
            atual->dado.data, atual->dado.demanda_residual);
        atual = atual->proximo;
    }
}

// Função que lê o CSV e cria a lista
Node* ler_csv(const char* nome_arquivo, Node* lista) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return lista;
    }

    char linha[MAX_LINE];

    // Lê e descarta a primeira linha (cabeçalho)
    fgets(linha, MAX_LINE, arquivo);

    while (fgets(linha, MAX_LINE, arquivo)) {
        EletricDates dado;

        // Remove o '\n' do fim da linha
        linha[strcspn(linha, "\n")] = 0;

        // Faz o parsing da linha para a struct
        sscanf(linha, "%23[^,],%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
            dado.data,
            &dado.demanda_residual,
            &dado.demanda_contratada,
            &dado.geracao_despachavel,
            &dado.geracao_termica,
            &dado.importacoes,
            &dado.geracao_renovavel_total,
            &dado.carga_reduzida_manual,
            &dado.capacidade_instalada,
            &dado.perdas_geracao_total
        );

        // Insere o dado no início da lista
        lista = inserir_inicio(lista, dado);
    }

    fclose(arquivo);
    return lista;
}

int main() {
    Node* lista = inicializar_lista();

    // Altere para o nome do arquivo CSV baixado
    lista = ler_csv("ESK2033.csv", lista);

    // Imprime para testar
    imprimir_lista(lista);

    return 0;
}
