#include <stdio.h>
#include <stdlib.h>
#include "pss.h"

int main(int argc, char *argv[]) {
    int aux_1; //variavel auxiliadora para o do while
    int aux_2; //variavel auxiliadora para o switch
    int incrementador = 1; //variavel para verificar se o programa foi executado mais de uma vez
    do{
        if(incrementador < 1){
            printf("\n----BEM-VINDO AO PROGRAMA DE GERENCIAMENTO DE OLEO----\n");
        }//caso o programa tenha rodado uma vez, digitar essa mensagem de bem-vindo
        printf("\nDigite uma das opcoes abaixo:");
        printf("\n1 - INSERIR LOTE");
        printf("\n2 - ELIMINAR LOTE");
        printf("\n3 - SOMATORIO MENSAL(CSV)");
        printf("\n4 - LISTAGEM(CSV)");
        printf("\n5 - ENCERRAR");
        aux_2 = input_d("\n>>>");

        switch(aux_2){
            case 1:
                printf("Caso 1");
                break;

            case 2:
                printf("Caso 2");
                break;

            case 3:
                printf("Caso 3");
                break;

            default:
                printf("Foda-se");
                break;
                return 1;
        }
        getchar();

        incrementador++;
    }while(aux_2 != 5);
}
