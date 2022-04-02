#ifndef __STD_LIB_H
#define __STD_LIB_H

#include "std_type.h"

// Operator matematika umum

// Modulo dua bilangan bulat
int mod(int a, int n);

// Operasi standar bahasa C
// Menyalin n bytes yang ditunjuk src ke dest
void memcpy(byte *dest, byte *src, unsigned int n);

// Mengembalikan panjang suatu null terminated string
unsigned int strlen(char *string);

// Mengembalikan true jika kedua string sama
bool strcmp(char *s1, char *s2);

// Melakukan penyalinan null terminated string
// Prekondisi: dst terdefinisi, punya panjang yang sama dengan src.
void strcpy(char *dst, char *src);

// Melakukan penyalinan null terminated string mulai dari indeks 0 hingga n.
// Prekondisi: dst terdefinisi, n lebih kecil dari src
void strncpy(char *dst, char *src, unsigned int n);

// Konkatenasi dua buah string
void strcat(char *dst, char *src);

// Mengosongkan byte array yang memiliki panjang n
void clear(byte *ptr, unsigned int n);

// Mengosongkan char array dengan panjang n
void clear_char(char *ptr, unsigned int n);

// Konversi uinteger ke string
char* itoa(int x);

// Melakukan pembulatan ke atas terhadap pembagian
int divc(int n, int div);

#endif