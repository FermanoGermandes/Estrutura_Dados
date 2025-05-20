#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096

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

void print_tabela(EletricDates* e) {
    printf("%-24s | %10.2f | %10.2f | %10.2f | %10.2f | %10.2f | %10.2f | %10.2f | %10.2f | %10.2f\n",
        e->data,
        e->demanda_residual,
        e->demanda_contratada,
        e->geracao_despachavel,
        e->geracao_termica,
        e->importacoes,
        e->geracao_renovavel_total,
        e->carga_reduzida_manual,
        e->capacidade_instalada,
        e->perdas_geracao_total
    );
}

int main() {
    FILE* arquivo = fopen("ESK2033.csv", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char linha[MAX_LINE];
    fgets(linha, MAX_LINE, arquivo); // Pular o cabeçalho

    printf("%-24s | %10s | %10s | %10s | %10s | %10s | %10s | %10s | %10s | %10s\n",
        "Data", "Dem_Res", "Dem_Cont", "Despach", "Termica", "Import", "RE Total", "MLR", "Capac", "Perdas");

    int count = 0;
    while (fgets(linha, MAX_LINE, arquivo) && count < 10) { // Lê apenas as 10 primeiras linhas
        EletricDates e;
        char* token;
        int coluna = 0;

        token = strtok(linha, ",");
        while (token != NULL) {
            switch (coluna) {
                case 0:
                    strncpy(e.data, token, sizeof(e.data));
                    break;
                case 4:
                    e.geracao_despachavel = atof(token);
                    break;
                case 5:
                    e.demanda_residual = atof(token);
                    break;
                case 6:
                    e.demanda_contratada = atof(token);
                    break;
                case 8:
                    e.importacoes = atof(token);
                    break;
                case 9:
                    e.geracao_termica = atof(token);
                    break;
                case 15:
                    e.carga_reduzida_manual = atof(token);
                    break;
                case 27:
                    e.geracao_renovavel_total = atof(token);
                    break;
                case 32:
                    e.capacidade_instalada = atof(token);
                    break;
                case 35:
                    e.perdas_geracao_total = atof(token);
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ",");
            coluna++;
        }

        print_tabela(&e);
        count++;
    }

    fclose(arquivo);
    return 0;
}
