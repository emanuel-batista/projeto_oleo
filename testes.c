#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE* bin_file = fopen("teste.dat", "wb");

    if (bin_file == NULL)
    {
        printf("Could not locate the file\n");
        exit(-1);
    }

    int number = 32;
    fwrite(&number, sizeof(int), 1, bin_file);

    double myDouble = 3.1415;
    fwrite(&myDouble, sizeof(double), 1, bin_file);
    
    
}