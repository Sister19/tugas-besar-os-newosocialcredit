#ifndef __STD_LIB_H
#define __STD_LIB_H

#include "../includes/std_type.h"

// Operator matematika umum

// Modulo dua bilangan bulat
int mod(int a, int n);

// Operasi standar bahasa C
// Menyalin n bytes yang ditunjuk src ke dest
void memcpy(byte *dest, byte *src, unsigned int n);

// Mengosongkan byte array yang memiliki panjang n
void clear(byte *ptr, unsigned int n);

// Konversi uinteger ke string
char* itoa(int x);

// Melakukan pembulatan ke atas terhadap pembagian
int divc(int n, int div);

void seed(int s);
int rand();

#endif