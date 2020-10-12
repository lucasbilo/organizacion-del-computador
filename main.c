#include <stdio.h>
#include "decimalABinario.h"

void imprimirNumeroEnBinario(uint32_t x){
    char xBinario[CANTIDAD_BITS];
    decimalABinario(x, xBinario);
    printf("Binario: ");
    imprimirBinario(xBinario);
}

int main() {
    printf("Hello, World!\n");
    printf("Ingrese un numero positivo: ");

    int entrada = 0;
    scanf("%d", &entrada);

    printf("Numero ingresado en ");
    imprimirNumeroEnBinario((uint32_t) entrada);
    return 0;
}
