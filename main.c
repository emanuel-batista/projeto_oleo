#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pss.h"

void clear(){
#ifdef _linux_
    system("clear");
#elif _WIN32
    system("cls");
#endif
}

typedef struct {
    long int dia, mes, ano;
    double qtd;
    char nome_arquivo[11];
} reg;

reg auxiliar;

int transfere_txt_array_reg(char *nome_arquivo) {
    FILE *ptr_arquivo_txt = fopen(nome_arquivo, "r");
    if (ptr_arquivo_txt == NULL) return 1;

    FILE *ptr_arquivo_bin = fopen("dados.bin", "ab");
    if (ptr_arquivo_bin == NULL) {
        fclose(ptr_arquivo_txt);
        return 1;
    }

    reg registro_unico;
    char linha[19];        // dd/mm/aaaa 9999.99
    char string_data[11];  // dd/mm/aaaa
    char string_litros[8]; // 9999.99
    char dia[3], mes[3], ano[5], *caractere_nao_convertido;

    while (fgets(linha, sizeof(linha), ptr_arquivo_txt) != NULL) {
        sscanf(linha, "%s %s\n", string_data, string_litros); // desmembrando em 2 partes
        sscanf(string_data, "%2[0-9]/%2[0-9]/%4[0-9]", dia, mes, ano); // desmenbrando em 3 partes

        // Convertendo as variáveis desmembradas em inteiros e decimal
        registro_unico.dia = strtol(dia, &caractere_nao_convertido, 10);
        registro_unico.mes = strtol(mes, &caractere_nao_convertido, 10);
        registro_unico.ano = strtol(ano, &caractere_nao_convertido, 10);
        registro_unico.qtd = strtod(string_litros, &caractere_nao_convertido);

        // Transferindo o nome do arquivo para a variável de registro
        snprintf(registro_unico.nome_arquivo, sizeof(registro_unico.nome_arquivo), "%s", nome_arquivo);

        // Ajustando o formato da string do nome do arquivo (conforme necessário)
        registro_unico.nome_arquivo[10] = '\0';
        registro_unico.nome_arquivo[2] = '/';
        registro_unico.nome_arquivo[5] = '/';

        // Escrevendo no arquivo binário
        fwrite(&registro_unico, sizeof(reg), 1, ptr_arquivo_bin);
    }

    fclose(ptr_arquivo_txt);
    fclose(ptr_arquivo_bin);
    printf("\nDados Transferidos com sucesso!\n");
    return 0;
}


int listar_dados_no_csv() {
    
    FILE *ptr_arquivo_bin = fopen("dados.bin", "rb");
    if (ptr_arquivo_bin == NULL) return -2;

    FILE *ptr_arquivo_csv = fopen("todos_registros.csv", "a");
    if (ptr_arquivo_csv == NULL) {
        fclose(ptr_arquivo_bin);
        return -2;
    }

    // Estrutura para armazenar cada registro lido
    reg registro_unico;

    // Adicionar cabeçalho apenas se o arquivo CSV estiver vazio
    fseek(ptr_arquivo_csv, 0, SEEK_END);
    long tamanho_arquivo = ftell(ptr_arquivo_csv);
    if (tamanho_arquivo == 0) {
        fprintf(ptr_arquivo_csv, "Data,Litros,NomeArq\n");
    }

    // Reiniciar a posição do arquivo binário para o início
    fseek(ptr_arquivo_bin, 0, SEEK_SET);

    printf("\n*************\n");
    printf("DIA MES ANO  LITROS      LOTE\n");
    printf("***********\n");
    while (fread(&registro_unico, sizeof(reg), 1, ptr_arquivo_bin)) {
        fprintf(ptr_arquivo_csv, "%ld/%ld/%ld,%.1lf,%s\n", registro_unico.dia, registro_unico.mes, registro_unico.ano, registro_unico.qtd, registro_unico.nome_arquivo);
        printf("%ld/%ld/%ld     %.1lf     %s\n", registro_unico.dia, registro_unico.mes, registro_unico.ano, registro_unico.qtd, registro_unico.nome_arquivo);
    }
    printf("***********\n");
    printf("Dados importados para todos_registros.csv\n\n");
    fclose(ptr_arquivo_bin);
    fclose(ptr_arquivo_csv);

    return 0;
}

int eliminarLote() {
    char data_a_excluir[12];
    printf("\nDigite a data do lote a ser excluido(DD/MM/AAAA): ");
    fgets(data_a_excluir, sizeof(data_a_excluir), stdin);

    size_t indice_nova_linha = strcspn(data_a_excluir, "\n");

    // Substitua o caractere de nova linha por '\0'
    data_a_excluir[indice_nova_linha] = '\0';

    FILE *arquivo_original = fopen("todos_registros.csv", "r");
    if (arquivo_original == NULL) {
        perror("Erro ao abrir arquivo original");
        return 1;
    }

    // Gerar um nome de arquivo temporário exclusivo
    char nome_arquivo_temp[] = "tempfileXXXXXX";
    int temp_fd = mkstemp(nome_arquivo_temp);
    if (temp_fd == -1) {
        perror("Erro ao criar arquivo temporário");
        fclose(arquivo_original);
        return 1;
    }

    FILE *arquivo_temporario = fdopen(temp_fd, "w");
    if (arquivo_temporario == NULL) {
        perror("Erro ao abrir arquivo temporário");
        close(temp_fd);
        fclose(arquivo_original);
        return 1;
    }

    char linha[256]; // Aumentar o tamanho do buffer para linhas longas
    while (fgets(linha, sizeof(linha), arquivo_original) != NULL) {
        char data_na_linha[11]; // Assumindo que a data está sempre no formato dd/mm/aaaa
        sscanf(linha, "%*[^,],%*[^,],%10[^,\n]", data_na_linha);

        if (strcmp(data_na_linha, data_a_excluir) != 0) {
            fputs(linha, arquivo_temporario);
        }
    }

    fclose(arquivo_original);
    fclose(arquivo_temporario);

    // Remover o arquivo original
    if (remove("todos_registros.csv") != 0) {
        perror("Erro ao remover o arquivo original");
        return 1;
    }

    // Renomeie o arquivo temporário para substituir o arquivo original
    if (rename(nome_arquivo_temp, "todos_registros.csv") != 0) {
        perror("Erro ao renomear arquivo temporário");
        return 1;
    }

    printf("Lote com a data %s excluído com sucesso!\n", data_a_excluir);
    return 0;
}


int main(int argc, char *argv[]) {

    if (argc == 2) {
        char *nomeArquivo = argv[1];
        printf("\n\nIniciando com o arquivo: %s\n", nomeArquivo);
        if (transfere_txt_array_reg(nomeArquivo) == 1) {
            printf("Erro ao abrir o arquivo.\n");
        }
    }

    reg registros[10000]; // define a struct
    //int indice_registros = 0; // variável que define o indice dos registros

    int op = -1;

    while (op != 5) {

        do {
           //clear();
            printf("**********\n");
            printf(" 1 - Inserir lote           \n");
            printf(" 2 - Eliminar lote          \n");
            printf(" 3 - Somatorio mensal (csv) \n");
            printf(" 4 - Listagem (csv)         \n");
            printf(" 5 - Encerrar               \n");
            printf("**********\n");
            op = input_d("Digite uma opcao: [1-5]:");

        } while (op < 1 || op > 5);

        switch (op) {

            case 1:
                printf("\n\nCaso 1 - INSERIR LOTE\n");
                char nomeArquivo[20];
                input_s("Insira o nome do arquivo (Exemplo: 05_04_2024.txt): ", nomeArquivo, sizeof(nomeArquivo));
                if (transfere_txt_array_reg(nomeArquivo) == 1) {
                    printf("Erro ao abrir o arquivo.\n");
                }
                break;


            case 2:
                eliminarLote();
                break;

            case 3:
                printf("Caso 3");
                break;

            case 4:
                printf("\n\nCaso 4 - Listagem (CSV)");
                if (listar_dados_no_csv() == -2) {
                    perror("Erro ao criar o arquivo CSV.\n");
                }
               // ler_do_binario_para_imprimir(registros);
                break;

            case 5:
                printf("Encerrando o programa.\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
}