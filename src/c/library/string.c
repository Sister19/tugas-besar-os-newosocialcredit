#include "string.h"
#include "../includes/constant.h"


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