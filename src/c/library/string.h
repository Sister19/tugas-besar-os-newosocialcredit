#ifndef __STRING_H
#define __STRING_H

#include "../includes/std_type.h"

// Operasi manipulasi string

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

#endif