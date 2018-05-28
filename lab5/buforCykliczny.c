/*
 * buforCykliczny.c
 *
 *  Created on: 28 maj 2018
 *      Author: klimek
 */

#include "buforCykliczny.h"
#include <stdlib.h>
#include <stdio.h>

void wyczyscBufor(struct buforCykliczny* bufor, char* plikWyjsciowyDlaDanychZBufora){
	int i = bufor->licznik;
	if(bufor->przebieg){
	        FILE* plik = fopen(plikWyjsciowyDlaDanychZBufora,"a");
	        if(plik){
	        	for(;i< wielkoscBuforu; ++i){
	        		fprintf(plik,"%s",bufor->wyjscie[i]);
	        		free(bufor->wyjscie[i]);
	        	}
	            fclose(plik);
	        }
	        else printf("Wystapil blad! Nie mozna otworzyc pliku %s\n",plikWyjsciowyDlaDanychZBufora);
	    }
	FILE* plik = fopen(plikWyjsciowyDlaDanychZBufora,"a");
	if(plik){
		for(i = 0; i <bufor->licznik; ++i){
			fprintf(plik,"%s",bufor->wyjscie[i]);
			free(bufor->wyjscie[i]);
	}
	fclose(plik);
	}
	else printf("Wystapil blad! Nie mozna otworzyc pliku %s\n",plikWyjsciowyDlaDanychZBufora);
	free(bufor);
}
void przygotujBufor(struct buforCykliczny* bufor){
    bufor->licznik = 0;
    bufor->przebieg = 0;
    memset(bufor,0,sizeof(char*)*wielkoscBuforu);
}
void dodajDoBuforu(char* wyjscie, struct buforCykliczny *bufor, char* plikWyjsciowyDlaDanychZBufora){
    if(bufor->przebieg){
        FILE* plik = fopen(plikWyjsciowyDlaDanychZBufora,"a");
        if(plik){
            fprintf(plik,"%s",bufor->wyjscie[bufor->licznik]);
            fclose(plik);
        }
        else printf("Wystapil blad! Nie mozna otworzyc pliku %s\n",plikWyjsciowyDlaDanychZBufora);
    }
    if(bufor->wyjscie[bufor->licznik] == NULL){
    	free(bufor->wyjscie[bufor->licznik]);
    }
    bufor->wyjscie[bufor->licznik] = wyjscie;
    printf("%s",bufor->wyjscie[bufor->licznik]);
    bufor->licznik++;
    if(bufor->licznik == wielkoscBuforu){
        bufor->przebieg = 1;
        bufor->licznik = 0;
    }
}

void wypisz(char* komunikat, struct buforCykliczny* bufor, char* plikWyjsciowy){
	dodajDoBuforu(komunikat,bufor,plikWyjsciowy);
}
