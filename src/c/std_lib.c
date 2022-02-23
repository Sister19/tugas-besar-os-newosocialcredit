#include "header/std_lib.h"

// Opsional : Tambahkan implementasi kode C

// Operator matematika umum
// Implementasikan
// Tips : Gunakan C integer division
int div(int a, int b){
    return a/b;
}

int mod(int a, int n){
    return a - n*(a/n);
}

// Operasi standar bahasa C
// Implementasikan
void memcpy(byte *dest, byte *src, unsigned int n)
// Mengcopy n bytes yang ditunjuk src ke dest
{
    for(int i=0;i<n;i++){
        dest[i]=src[i];
    }
}

unsigned int strlen(char *string)
// Mengembalikan panjang suatu null terminated string
{
    int i=0;
    while(string[i]!='\0'){
        i++;
    }
    return i;
}

bool strcmp(char *s1, char *s2)
// Mengembalikan true jika string sama
{
    int i=0;
    while(s1[i]!='\0' && s2[i]!='\0'){
        if(s1[i]!=s2[i]){
            return false;
        }
        i++;
    }
    return true;
}

void strcpy(char *dst, char *src)
// Melakukan penyalinan null terminated string
{
    int i=0;
    while(src[i]!='\0'){
        dst[i]=src[i];
        i++;
    }
    dst[i]='\0';
}

void clear(byte *ptr, unsigned int n)
// Mengosongkan byte array yang memiliki panjang n
{
    for(int i=0;i<n;i++){
        ptr[i]=0;
    }
}