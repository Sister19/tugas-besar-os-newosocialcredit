#include "std_lib.h"
#include "../includes/constant.h"

// Operator matematika umum
int mod(int a, int n)
{
    int ret = a - n * (a / n);
    if (n < 0)
        return -mod(-a, -n);
    if (ret < 0) ret += n;
    return ret;
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

void clear(byte *ptr, unsigned int n)
{
    unsigned int i;
    for (i = 0; i < n; ++i)
        ptr[i] = 0;
}

char str[8];

char* itoa(int x)
{
    int i = 0, len;
    char temp;
    do {
        str[i++] = mod(x, 10) + '0';
        x /= 10;
    } while (x);
    len = i;
    i = 0;
    while (i < len / 2) {
        temp = str[i];
        str[i] = str[len-i-1];
        str[len-i-1] = temp;
        i++;
    }
    str[len] = nullt;
    return str;
}

int divc(int n, int div){
    return (n / div) + (mod(n, div) > 0);
}

// Xorshift LSFR (Linear-Feedback Shift Register) PRNG
// 2^64-1 period for 64bit integers
// Based on Marsaglia, George. 2003. Xorshift RNGs. https://www.jstatsoft.org/article/view/v008i14/xorshift.pdf
int MAX_INT = 0xff00;
static int state = 0;
void seed(int s) {
    state = s;
}
int rand() {
    state ^= (state << 3);
    state ^= (state >> 5);
    state ^= (state << 7);
    state &= MAX_INT;
    return state;
}