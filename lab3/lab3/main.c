#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define rozmiarGotowego 50

int main()
{
    char dopuszczalneZnaki[] = "1234567890";
    char* wyraz1 = "Koncowe wyrazenie wynosi ";
    char *gotowyCiag = malloc(rozmiarGotowego*sizeof(char));
    memset(gotowyCiag,0,rozmiarGotowego);
    strcat(gotowyCiag, wyraz1);
    char *stringNumeryczny = malloc((rozmiarGotowego-strlen(wyraz1))*sizeof(char));
    memset(stringNumeryczny,0,rozmiarGotowego-strlen(wyraz1));
    char znak[2] = "\0\0";
    puts("Wpisuj ciag liczbowy - niecyfra przerywa");
    *znak = getchar();
    int czyznak = isdigit(*znak);
    assert(czyznak != 0);
    while(isdigit(*znak)){
        strcat(stringNumeryczny,znak);
        *znak = getchar();
    }
    strcat(stringNumeryczny,znak);
    puts("Podany ciag: ");
    sprintf(stringNumeryczny,"%s\n",stringNumeryczny);
    printf("%s",stringNumeryczny);
    char* czescNumeryczna = calloc(strlen(stringNumeryczny),sizeof(char));
    memcpy(czescNumeryczna, stringNumeryczny,strspn(stringNumeryczny,dopuszczalneZnaki));
    strcat(gotowyCiag,czescNumeryczna);
    sprintf(gotowyCiag,"%s\n",gotowyCiag);
    puts(gotowyCiag);
    system("pause");

    char *czescPoLiczbie = strdup(gotowyCiag);
    puts("\nSkracam do ostatniej liczby...\n");
    memmove(czescPoLiczbie+25,czescPoLiczbie+strlen(czescPoLiczbie)-2,3);
    puts(czescPoLiczbie);

	free(stringNumeryczny);
	free(gotowyCiag);


    return 0;
}
