#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> //se nao usar sleep, tirar essa
#include <direct.h>
#include "pss.h"

void clear()
{
#ifdef __linux__
    system("clear");
#elif _WIN32
    system("cls");
#endif
}

typedef struct
{
    int dia, mes, ano;
    double litragem;
    char nome_arq[12];
} reg;



void transBinario(char nome_arq[12], reg *Registros, int *indice_registros)
{

    reg registro_unico;
    nome_arq[strlen(nome_arq) - 1] = '\0';


    for (size_t i = 0; i < 13; i++)
    {
        printf("%d: %c\n", i, nome_arq[i]);
    }
    
    
    FILE *fp = fopen(nome_arq, "r");
    if (fp == NULL)
    {
        perror("Erro ao localizar arquivo");
        exit(-1);
    }


    char linha[19],        // dd/mm/aaaa 9999.99
         string_data[11],  // dd/mm/aaaa
         string_litros[8]; // 9999.99

    char dia[3], mes[3], ano[5], *caractere_nao_convertido;

    while (fgets(linha, sizeof(linha), fp) != NULL) {

        printf("\nsaiu do arquivo .txt      : %s",linha);// importante para debug

       /*  if(strspn(linha, "0123456789/.") != strlen(linha)){
            printf("Erro ao converter linha, verifique o arquivo.");
            exit(-1);
        } */

        sscanf(linha, "%s %s\n", string_data, string_litros); // desmembrando em 2 partes(a primeira parte é a data que ta em string e a segunda eh a litragem q tbm ta em string)

        sscanf(string_data, " %2[0-9]/%2[0-9]/%4[0-9]", dia, mes, ano); // desmenbrando em 3 partes. É possivel fazer todos os desmenbramentos em uma unica linha

        printf("desmembrado em 4 variaveis: |%s|%s|%s|%s|\n",dia, mes, ano,string_litros);// importante para debug

        //Vou converter as variaveis desmenbradas em inteiros e decimal
        registro_unico.dia = strtol(dia, &caractere_nao_convertido, 10);
        registro_unico.mes = strtol(mes, &caractere_nao_convertido, 10);
        registro_unico.ano = strtol(ano, &caractere_nao_convertido, 10);
        registro_unico.litragem = strtod(string_litros, &caractere_nao_convertido);
        //Vou transferir o nome do arquivo para a variavel de registro
        snprintf(registro_unico.nome_arq, sizeof(registro_unico.nome_arq), "%s", nome_arq);

        registro_unico.nome_arq[10] = '\0';
        registro_unico.nome_arq[2] = '/';
        registro_unico.nome_arq[5] = '/';

		printf("transferido p/ 'reg'      : %ld %ld %ld %lf %s\n",registro_unico.dia, registro_unico.mes, registro_unico.ano, registro_unico.litragem, registro_unico.nome_arq); // importante para debug

        //transformar em arquivo binario
		
        FILE *bn = fopen("dados.bin", "ab");
        if(bn == NULL){
            perror("Arquivo binario nao pode ser aberto");
            exit(-1);
        }
        size_t num_written = fwrite(&registro_unico, sizeof(registro_unico), 1, bn);
        if (num_written != 1) {
            perror("Error writing to file");
            fclose(bn);
            exit(-1);
        }

        Registros[(*indice_registros)++] = registro_unico;
        getchar();
    }
    
    
    fclose(fp);
    printf("%d", *indice_registros);
    sleep(5);

}

int main(int argc, char *argv[])
{
    int *a;
    clear();
    //sleep(5);
    char nome_arq[12];//passando o nome do arquivo de maneira bruta pra essa variavel
    if (argc > 1) {
        strcpy(nome_arq, argv[1]);//passando 
    }else {
        a = 1;
        printf("Nenhum argumento fornecido.\n");
    }
   
    reg Registros[10000];
    int indice_registros = 0;

    int op = -1;

    while (op != 5)
    {

        do
        {

            clear();
            printf("****************************\n");
            printf(" 1 - Inserir lote           \n");
            printf(" 2 - Eliminar lote          \n");
            printf(" 3 - Somatorio mensal (csv) \n");
            printf(" 4 - Listagem (csv)         \n");
            printf(" 5 - Encerrar               \n");
            printf("****************************\n");
            op = input_d("Digite uma opcao: [1-5]:");

        } while (op < 1 || op > 5);

        switch (op)
        {

        case 1:
            if (a == 1)
            {
                printf("Digite o nome do arquivo: ");
                fgets(nome_arq, 15, stdin);
                //nome_arq[strlen(nome_arq) - 1] = '\0';
            }
            
            transBinario(nome_arq, Registros, &indice_registros);

            a = 1;
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            return 0;
        }
    }
}