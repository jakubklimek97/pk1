/*
 * main.c
 *
 *  Created on: 20 maj 2018
 *      Author: klimek
 */
#include <stdio.h>
#include <unistd.h>
enum stany{
	nasluch,
	oczekiwanie,
	polaczenie,
	wolny,
	zajety
};
enum polecenieK{
	polacz,
	odbierz,
	zamknij,
	polaczeni
};
struct klient;

struct polecenie;
struct klient{
	int id;
	enum stany stan;
	struct polecenie* aktualnePolecenie;
};
struct polecenie{
	enum polecenieK polecenie;
	struct klient* nadawca, odbiorca;
};
struct serwer{
	struct klient* zbiorKlientow;
	struct polecenie* listaPolecen;
	int maksymalnaLiczbaPolecen;
	int aktualnaLiczbaPolecen;
	struct polecenie* nastepujace;
	struct polecenie* powracajacePolecenia;
	int maksymalnaLiczbaPowracajacych;
	int aktualnaLiczbaPowracajacych;
};


struct Klient{
	struct polaczenie* polecenie;
	int id;
	struct Klient* rozmowca;
	enum stany stan;
};
void wypisz(char* komunikat){

}
void wykonaj(struct Klient* nadawca, struct Klient* odbiorca, struct polecenie* polec){
	if(polec->polecenie == polacz){
		if(nadawca->stan == nasluch){
			if(odbiorca->stan == nasluch){
				nadawca->stan = oczekiwanie;
				odbiorca->stan = wolny;
				printf("Odbiorca dzwoni\n");
				nadawca->rozmowca = odbiorca;
				odbiorca->rozmowca = nadawca;
			}
			else printf("Zajete\n");
		}
	}
	else if(polec->polecenie == odbierz){
			if(nadawca->stan == wolny){
				nadawca->stan = zajety;
				nadawca->rozmowca->stan = polaczenie;
			}
	}
	else if(polec->polecenie == zamknij){
			if(nadawca->stan == wolny){
				if(nadawca->rozmowca != NULL){
					printf("Odrzucono polaczenenie\n");
					nadawca->stan = nasluch;
					nadawca->rozmowca->stan = wolny;
					nadawca->rozmowca->rozmowca = NULL;
					nadawca->rozmowca = NULL;
				}else{
					nadawca->stan = nasluch;
				}

			}
			else if(nadawca->stan == oczekiwanie){
				nadawca->stan = nasluch;
				nadawca->rozmowca->stan = nasluch;
				nadawca->rozmowca->rozmowca = NULL;
				nadawca->rozmowca = NULL;
			}
			else if(nadawca->stan == polaczenie){
				nadawca->stan = nasluch;
				nadawca->rozmowca->stan = wolny;
				nadawca->rozmowca->rozmowca = NULL;
				nadawca->rozmowca = NULL;
			}
	}
}
int main(void){

	return 0;
}

