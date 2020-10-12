//
// Created by lucas on 12/10/20.
//

#ifndef ORGA_DECIMALABINARIO_H
#define ORGA_DECIMALABINARIO_H

#include <stdint.h>

#define CANTIDAD_BITS 32
#define MAX_UINT32 4294967295

void inicializarBinario(char binario[]);

void decimalABinario(uint32_t x, char binario[]);

void imprimirBinario(char binario[]);

#endif //ORGA_DECIMALABINARIO_H
