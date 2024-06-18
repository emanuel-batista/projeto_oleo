#include <stdio.h>
#include <stdlib.h>
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
/*
int transfere_txt_array_reg(char *nome_arquivo, reg *registros, int *indice_registros){

    FILE *ptr_arquivo_txt = fopen(nome_arquivo, "r");
    if (ptr_arquivo_txt == NULL) return 1;

    FILE *ptr_arquivo_bin = fopen("dados.bin", "ab");
    if (ptr_arquivo_bin == NULL) {
        fclose(ptr_arquivo_txt);
        return 1;
    }

    reg registro_unico;

    char linha[19],        // dd/mm/aaaa 9999.99
    string_data[11],  // dd/mm/aaaa
    string_litros[8]; // 9999.99

    char dia[3], mes[3], ano[5], *caractere_nao_convertido;

    while (fgets(linha, sizeof(linha), ptr_arquivo_txt) != NULL) {

       // printf("\nsaiu do arquivo .txt      : %s",linha);// importante para debug

        sscanf(linha, "%s %s\n", string_data, string_litros); // desmembrando em 2 partes

        sscanf(string_data, " %2[0-9]/%2[0-9]/%4[0-9]", dia, mes, ano); // desmenbrando em 3 partes.   possivel fazer todos os desmenbramentos em uma unica linha

       // printf("desmembrado em 4 variaveis: |%s|%s|%s|%s|\n",dia, mes, ano,string_litros);// importante para debug

        //Vou converter as variaveis desmenbradas em inteiros e decimal
        registro_unico.dia = strtol(dia, &caractere_nao_convertido, 10);
        registro_unico.mes = strtol(mes, &caractere_nao_convertido, 10);
        registro_unico.ano = strtol(ano, &caractere_nao_convertido, 10);
        registro_unico.qtd = strtod(string_litros, &caractere_nao_convertido);
        //Vou transferir o nome do arquivo para a variavel de registro
        snprintf(registro_unico.nome_arquivo, sizeof(registros->nome_arquivo), "%s", nome_arquivo);

        registro_unico.nome_arquivo[10] = '\0';
        registro_unico.nome_arquivo[2] = '/';
        registro_unico.nome_arquivo[5] = '/';

        printf("transferido p/ 'reg'      : %ld %ld %ld %lf %s\n",registro_unico.dia, registro_unico.mes, registro_unico.ano, registro_unico.qtd, registro_unico.nome_arquivo); // importante para debug

        //Transfiro para o array de registros
        registros[(*indice_registros)++] = registro_unico;
       // getchar();

        // Escrevo no arquivo binário
        fwrite(&registro_unico, sizeof(reg), 1, ptr_arquivo_bin);

    }


    fclose(ptr_arquivo_txt);
    fclose(ptr_arquivo_bin);
    printf("\nDados Transferidos com sucesso!\n");
    return 0;
}
*/

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





/* ANTIGO - SEM BINARIO
int listar_dados_no_csv(reg *registros, int indice_registros){

    FILE *ptr_arquivo_csv = fopen("todos_registros.csv", "w");
    if (ptr_arquivo_csv == NULL)return -2;


    fprintf(ptr_arquivo_csv, "Data,Litros,NomeArq\n");
    int i;
    for (i=0; i<indice_registros; i++)
        fprintf(ptr_arquivo_csv, "%ld/%ld/%ld,%.1lf,%s\n", registros[i].dia, registros[i].mes, registros[i].ano, registros[i].qtd, registros[i].nome_arquivo);

    fclose(ptr_arquivo_csv);

    return 0;
}*/

int listar_dados_no_csv() {
    FILE *ptr_arquivo_bin = fopen("dados.bin", "rb");
    if (ptr_arquivo_bin == NULL) return -2;

    FILE *ptr_arquivo_csv = fopen("todos_registros.csv", "w");
    if (ptr_arquivo_csv == NULL) {
        fclose(ptr_arquivo_bin);
        return -2;
    }

    reg registro_unico;
    fprintf(ptr_arquivo_csv, "Data,Litros,NomeArq\n");

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
/*
void ler_do_binario_para_imprimir(reg *registros){
    // Lendo...    rb

    int quantidadeRegistros = 0;

    FILE *ptr_arquivo_bin = fopen("dados.bin","rb");  //Abri o arquivo. Meu ponteiro está no byte 0

    while(fread(&auxiliar,sizeof(reg),1,ptr_arquivo_bin)) //Apos a execucao de read(), meu ponteiro vai para o byte x
        registros[quantidadeRegistros++]=auxiliar;
    fclose(ptr_arquivo_bin);

    // Imprimindo registros
    printf("\n*************\n");
    printf("DIA MES ANO  LITROS      LOTE\n");
    printf("***********\n");
    for ( int i = 0; i < quantidadeRegistros; i++)
        printf("%ld/%ld/%ld     %.1lf     %s\n", registros[i].dia, registros[i].mes, registros[i].ano, registros[i].qtd, registros[i].nome_arquivo);
    printf("***********");
    printf("\n\n");
}
*/




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
                printf("Caso 2");
                break;

            case 3:
                printf("Caso 3");
                break;

            case 4:
                printf("\n\nCaso 4 - Listagem (CSV)");
                if (listar_dados_no_csv() == -2) {
                    printf("Erro ao criar o arquivo CSV.\n");
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