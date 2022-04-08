#include "std_lib.h"
#include "../includes/constant.h"

// Operator matematika umum
int mod(int a, int n)
{
    return a - n * (a / n);
}

// Operasi standar bahasa C
void memcpy(byte *dest, byte *src, unsigned int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        dest[i] = src[i];
    }
}

unsigned int strlen(char *string)
{
    unsigned int i = 0;
    while (string[i] != nullt)
    {
        i++;
    }
    return i;
}

bool strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i] != nullt && s2[i] != nullt)
    {
        if (s1[i] != s2[i])
            return false;
        i++;
    }
    return (s1[i] == nullt && s2[i] == nullt);
}

void strcpy(char *dst, char *src)
{
    unsigned int i = 0;
    while (src[i] != nullt)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = nullt;
}

void strncpy(char *dst, char *src, unsigned int n)
{
    unsigned int i = 0;
    while (i < n && src[i] != nullt)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = nullt;
}

void strcat(char *dst, char *src)
{
    unsigned int i = 0;
    while (dst[i] != nullt)
        ++i;
    while (src[i] != nullt)
    {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = nullt;
}

void clear(byte *ptr, unsigned int n)
{
    unsigned int i;
    for (i = 0; i < n; ++i)
        ptr[i] = 0;
}

char str[8];

char* itoa(int x)
{
    int i = 0;
    do {
        str[i++] = mod(x, 10) + '0';
        x /= 10;
    } while (x);
    str[i] = nullt;
    return str;
}

int divc(int n, int div){
    return (n / div) + (mod(n, div) > 0);
}