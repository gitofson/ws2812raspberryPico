#ifndef  __SYMBOL_H
#define __SYMBOL_H

#include <stdio.h>
#include <stdlib.h>


//#define N_SYMBOL_ROWS          (14)
//#define N_SYMBOL_COLUMNS       (70)

//typedef struct __symbol { uint8_t s[N_SYMBOL_ROWS * N_SYMBOL_COLUMNS]; } symbol_t;

enum symbol{
    sym_rhomoid,
    sym_rhomoid_thin,
    sym_test,
    sym_christmas_tree,
    direct_sym_test
};


//extern const uint8_t symTemp[];
//extern const uint8_t symRhomboid[];

void getSymbol(uint8_t*, enum symbol);
#endif