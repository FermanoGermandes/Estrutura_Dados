#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct EletricDates {
    char data[25]; 
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

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int data_igual(const char* d1, const char* d2) {
    return strncmp(d1, d2, 22) == 0;
}

int validar_data_formatada(const char* data) {
    int ano, mes, dia, hora, minuto, segundo;
    char periodo[3];
    return sscanf(data, "%4d-%2d-%2d %2d:%2d:%2d %2s", &ano, &mes, &dia, &hora, &minuto, &segundo, periodo) == 7
           && (strcmp(periodo, "AM") == 0 || strcmp(periodo, "PM") == 0);
}

int ja_existe(Node* head, const char* data) {
    while (head != NULL) {
        if (data_igual(head->dado.data, data)) return 1;
        head = head->next;
    }
    return 0;
}

Node* inserir_fim(Node* head, EletricDates dado) {
    if (ja_existe(head, dado.data)) {
        printf("Erro: Já existe um dado com essa data.\n");
        return head;
    }

    Node* novo = malloc(sizeof(Node));
    if (!novo) {
        printf("Erro de alocação de memória.\n");
        return head;
    }

    novo->dado = dado;
    novo->next = NULL;

    if (head == NULL) return novo;

    Node* atual = head;
    while (atual->next != NULL) atual = atual->next;
    atual->next = novo;
    return head;
}

Node* remover_dado(Node* head, const char* data_alvo) {
    Node* atual = head;
    Node* anterior = NULL;

    while (atual != NULL) {
        if (data_igual(atual->dado.data, data_alvo)) {
            if (anterior == NULL) {
                Node* temp = atual->next;
                free(atual);
                return temp;
            } else {
                anterior->next = atual->next;
                free(atual);
                return head;
            }
        }
        anterior = atual;
        atual = atual->next;
    }

    printf("Data não encontrada para remoção.\n");
    return head;
}

EletricDates* buscar_por_data(Node* head, const char* data_alvo) {
    while (head != NULL) {
        if (data_igual(head->dado.data, data_alvo)) {
            return &head->dado;
        }
        head = head->next;
    }
    return NULL;
}

#define MAX_LINHA 512

Node* carregar_dataset(const char* nome_arquivo, Node* lista) {
    FILE* fp = fopen(nome_arquivo, "r");
    if (!fp) {
        printf("Erro ao abrir arquivo %s\n", nome_arquivo);
        return lista;
    }

    char linha[MAX_LINHA];
    fgets(linha, MAX_LINHA, fp); // pula cabeçalho

    while (fgets(linha, MAX_LINHA, fp)) {
        EletricDates dado = {0};
        char* token = strtok(linha, ",");

        if (!token) continue;
        strncpy(dado.data, token, sizeof(dado.data) - 1);

        for (int pos = 1; pos <= 9 && (token = strtok(NULL, ",")); pos++) {
            double valor = atof(token);
            switch (pos) {
                case 1: dado.demanda_residual = valor; break;
                case 2: dado.demanda_contratada = valor; break;
                case 3: dado.geracao_despachavel = valor; break;
                case 4: dado.geracao_termica = valor; break;
                case 5: dado.importacoes = valor; break;
                case 6: dado.geracao_renovavel_total = valor; break;
                case 7: dado.carga_reduzida_manual = valor; break;
                case 8: dado.capacidade_instalada = valor; break;
                case 9: dado.perdas_geracao_total = valor; break;
            }
        }

        lista = inserir_fim(lista, dado);
    }

    fclose(fp);
    return lista;
}

void imprimir_dado(EletricDates dado) {
    printf("\nData: %s\n", dado.data);
    printf("Demanda Residual: %.2f\n", dado.demanda_residual);
    printf("Demanda Contratada: %.2f\n", dado.demanda_contratada);
    printf("Geracao Despachavel: %.2f\n", dado.geracao_despachavel);
    printf("Geracao Termica: %.2f\n", dado.geracao_termica);
    printf("Importacoes: %.2f\n", dado.importacoes);
    printf("Geracao Renovavel Total: %.2f\n", dado.geracao_renovavel_total);
    printf("Carga Reduzida Manual: %.2f\n", dado.carga_reduzida_manual);
    printf("Capacidade Instalada: %.2f\n", dado.capacidade_instalada);
    printf("Perdas Geracao Total: %.2f\n", dado.perdas_geracao_total);
    printf("-----------------------------------------\n");
}

void imprimir_lista(Node* head) {
    if (!head) {
        printf("Lista vazia.\n");
        return;
    }
    while (head) {
        imprimir_dado(head->dado);
        head = head->next;
    }
}

void liberar_lista(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Node* lista = inicializar_lista();
    const char* nome_arquivo = "ESK2033.csv";
    lista = carregar_dataset(nome_arquivo, lista);
    printf("Dataset '%s' carregado com sucesso.\n", nome_arquivo);

    char opcao[10];

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1 - Buscar dado por data\n");
        printf("2 - Inserir novo dado\n");
        printf("3 - Remover dado\n");
        printf("4 - Imprimir todos os dados\n");
        printf("5 - Sair\n");
        printf("Opcao: ");
        fgets(opcao, sizeof(opcao), stdin);

        switch (opcao[0]) {
            case '1': {
                char data_alvo[25];
                printf("Digite a data (YYYY-MM-DD HH:MM:SS AM/PM): ");
                fgets(data_alvo, sizeof(data_alvo), stdin);
                data_alvo[strcspn(data_alvo, "\n")] = '\0';

                EletricDates* dado = buscar_por_data(lista, data_alvo);
                if (dado) imprimir_dado(*dado);
                else printf("Data não encontrada.\n");
                break;
            }
            case '2': {
                EletricDates dado = {0};
                printf("Digite a data (YYYY-MM-DD HH:MM:SS AM/PM): ");
                fgets(dado.data, sizeof(dado.data), stdin);
                dado.data[strcspn(dado.data, "\n")] = '\0';

                if (!validar_data_formatada(dado.data)) {
                    printf("Erro: formato inválido.\n");
                    break;
                }

                if (ja_existe(lista, dado.data)) {
                    printf("Erro: dado já existe para essa data.\n");
                    break;
                }

                printf("Demanda Residual: "); scanf("%lf", &dado.demanda_residual);
                printf("Demanda Contratada: "); scanf("%lf", &dado.demanda_contratada);
                printf("Geracao Despachavel: "); scanf("%lf", &dado.geracao_despachavel);
                printf("Geracao Termica: "); scanf("%lf", &dado.geracao_termica);
                printf("Importacoes: "); scanf("%lf", &dado.importacoes);
                printf("Geracao Renovavel Total: "); scanf("%lf", &dado.geracao_renovavel_total);
                printf("Carga Reduzida Manual: "); scanf("%lf", &dado.carga_reduzida_manual);
                printf("Capacidade Instalada: "); scanf("%lf", &dado.capacidade_instalada);
                printf("Perdas Geracao Total: "); scanf("%lf", &dado.perdas_geracao_total);
                limpar_buffer();

                lista = inserir_fim(lista, dado);
                printf("Dado inserido com sucesso.\n");
                break;
            }
            case '3': {
                char data[25];
                printf("Digite a data para remover: ");
                fgets(data, sizeof(data), stdin);
                data[strcspn(data, "\n")] = '\0';
                lista = remover_dado(lista, data);
                printf("Dado removido com sucesso.\n");
                break;
            }
            case '4':
                imprimir_lista(lista);
                break;
            case '5':
                liberar_lista(lista);
                printf("Saindo...\n");
                return 0;
            default:
                printf("Opcao invalida.\n");
        }
    }

    return 0;
}
