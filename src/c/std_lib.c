#include "header/std_lib.h"

// Operator matematika umum
int div(int a, int b)
{
    return a / b;
}

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
    int i = 0;
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
    return true;
}

void strcpy(char *dst, char *src)
{
    int i = 0;
    while (src[i] != nullt)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

void clear(byte *ptr, unsigned int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        ptr[i] = 0;
    }
}