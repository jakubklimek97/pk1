/*
 * main.c
 *
 *  Created on: 16 maj 2018
 *      Author: klimek
 */
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define liczbaKlientow 6
#define wielkoscBuforu 10

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
	polaczony
};
struct klient;

struct polecenie;
struct klient{
	int id;
	enum stany stan;
};
struct polecenie{
	enum polecenieK polecenie;
};

struct Klient{
	struct polaczenie* polecenie;
	int id;
	struct Klient* rozmowca;
	enum stany stan;
};

struct buforCykliczny{
    unsigned short przebieg;
    unsigned int licznik;
    char* wyjscie[wielkoscBuforu];
};
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
void wykonaj(struct Klient* nadawca, struct Klient* odbiorca, struct polecenie* polec, char* bufor){
	if(polec->polecenie == polacz){
		if(nadawca->stan == nasluch){
			if(odbiorca->stan == nasluch){
				nadawca->stan = oczekiwanie;
				odbiorca->stan = wolny;
				sprintf(bufor,"Dzwoni telefon odbiorcy | %d <-> %d\n",nadawca->id, odbiorca->id);
				nadawca->rozmowca = odbiorca;
				odbiorca->rozmowca = nadawca;
			}
			else sprintf(bufor,"Odbiorca zajety | %d <-> %d\n",nadawca->id, odbiorca->id);
		}
	}
	else if(polec->polecenie == odbierz){
			if(nadawca->stan == wolny){
				nadawca->stan = zajety;
				nadawca->rozmowca->stan = polaczenie;
				sprintf(bufor,"Odbiorca odebral polaczenie | %d <-> %d\n",nadawca->id, odbiorca->id);

			}
	}
	else if(polec->polecenie == zamknij){
			if(nadawca->stan == wolny){
				if(nadawca->rozmowca != NULL){
					sprintf(bufor,"Odbiorca odrzucil polaczenie | %d <-> %d\n",nadawca->id, odbiorca->id);
					nadawca->stan = nasluch;
					nadawca->rozmowca->stan = nasluch;
					nadawca->rozmowca->rozmowca = NULL;
					nadawca->rozmowca = NULL;
				}else{
					nadawca->stan = nasluch;
				}

			}
			else if(nadawca->stan == oczekiwanie){
				sprintf(bufor,"Nadawca zaniechal proby dodzwonienia | %d <-> %d\n",nadawca->id, nadawca->rozmowca->id);
				nadawca->stan = nasluch;
				nadawca->rozmowca->stan = nasluch;
				nadawca->rozmowca->rozmowca = NULL;
				nadawca->rozmowca = NULL;
			}
			else if(nadawca->stan == polaczenie){
				sprintf(bufor,"Rozmowa zakonczona | %d <-> %d\n",nadawca->id, odbiorca->id);
				nadawca->stan = nasluch;
				nadawca->rozmowca->stan = nasluch;
				nadawca->rozmowca->rozmowca = NULL;
				nadawca->rozmowca = NULL;
			}
			else if(nadawca->stan == zajety){
				sprintf(bufor,"Rozmowa zakonczona | %d <-> %d\n",nadawca->id, odbiorca->id);
				nadawca->stan = nasluch;
				nadawca->rozmowca->stan = nasluch;
				nadawca->rozmowca->rozmowca = NULL;
				nadawca->rozmowca = NULL;
			}
	}
}

void wylosujPolecenie(struct Klient** klienci, struct Klient **nadawca, struct Klient** odbiorca, struct polecenie* polec){
	int idNadawcy, idOdbiorcy;
	idNadawcy = rand() % liczbaKlientow;
	*nadawca = &((*klienci)[idNadawcy]);
	if((*nadawca)->stan == nasluch){
		/* proba polaczenia */
		polec->polecenie = polacz;
		idOdbiorcy = rand() % liczbaKlientow;
		while(idNadawcy == idOdbiorcy)idOdbiorcy = rand() % liczbaKlientow;
		*odbiorca = &((*klienci)[idOdbiorcy]);
	}
	else if((*nadawca)->stan == wolny){
		int czyOdbierze = rand() %1000;
		if(czyOdbierze > 500){
			/*odrzucenie*/
			polec->polecenie = zamknij;
			*odbiorca = (*nadawca)->rozmowca;
		}
		else{
			polec->polecenie = odbierz;
			*odbiorca = (*nadawca)->rozmowca;
		}
	}
	else if((*nadawca)->stan == zajety){
		/*jedyne co mozemy to zakonczyc polaczenie*/
		polec->polecenie = zamknij;
		*odbiorca = (*nadawca)->rozmowca;
	}
	else if((*nadawca)->stan == polaczenie){
		/*jedyne co mozemy to zakonczyc polaczenie*/
		polec->polecenie = zamknij;
		*odbiorca = (*nadawca)->rozmowca;
	}
	else{/*oczekiwanie*/
		polec->polecenie = zamknij;
		*odbiorca = (*nadawca)->rozmowca;
	}
}
static volatile int czyDzialac = 1;
void odDzialaj(int e){
	czyDzialac = 0;
}
int main(int argc, char** argv){
	if(argc > 1){

		srand(time(NULL));
			struct Klient *klienci = malloc(sizeof(struct Klient)*liczbaKlientow);
			struct buforCykliczny* buforCyk = malloc(sizeof(struct buforCykliczny));
			przygotujBufor(buforCyk);
			char* bufor = malloc(sizeof(char)*128);
			sprintf(bufor,"Centrala zaczyna prace\n");
			wypisz(bufor,buforCyk, argv[1]);
			int i;
			for(i = 0; i < liczbaKlientow; ++i){
				klienci[i].id = i;
				klienci[i].polecenie = NULL;
				klienci[i].rozmowca = NULL;
				klienci[i].stan = nasluch;
			}
			struct Klient *nadawca, *odbiorca;
			struct polecenie polec;
			signal(SIGINT,odDzialaj);
			signal(SIGQUIT,odDzialaj);

			while(czyDzialac){
				bufor = malloc(sizeof(char)*128);
				wylosujPolecenie(&klienci, &nadawca, &odbiorca, &polec);
				wykonaj(nadawca,odbiorca,&polec, bufor);
				wypisz(bufor,buforCyk, argv[1]);
				sleep(1);
			}

			free(klienci);
			bufor = malloc(sizeof(char)*128);
			sprintf(bufor,"Centrala konczy prace\n");
			wypisz(bufor,buforCyk, argv[1]);
			wyczyscBufor(buforCyk, argv[1]);
	}
	else printf("1. argument - nazwa pliku, do ktorego ma logowac\n");


	return 0;
}

