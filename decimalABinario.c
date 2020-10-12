//
// Created by lucas on 12/10/20.
//

#include "decimalABinario.h"
#include <stdio.h>

void inicializarBinario(char binario[]){
    for(int i = 0; i < CANTIDAD_BITS; i++){
        binario[i] = '0';
    }
}

void decimalABinario(uint32_t decimal, char binario[]){
    inicializarBinario(binario);
    unsigned int contador = CANTIDAD_BITS - 1;
    unsigned int resto = 0;

    while(decimal > 0){
        resto = decimal%2;
        decimal /= 2;
        binario[contador] = resto == 1? '1' : '0';
        contador--;
    }
}

void imprimirBinario(char binario[]){
    printf("\n");
    for(int i = 0; i < CANTIDAD_BITS; i++){
        printf("%c", binario[i]);
        if((i + 1) % 8 == 0){
            printf(" ");
        }
    }
    printf("\n");
}
