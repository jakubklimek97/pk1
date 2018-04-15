#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define wielkoscBuforu 3
#define zakresTempNaMinus 0
#define zakresTempNaPlus 30
struct buforCykliczny{
    unsigned short przebieg;
    unsigned int licznik;
    float temperatura[wielkoscBuforu];
};
void przygotujBufor(struct buforCykliczny* bufor){
    bufor->licznik = 0;
    bufor->przebieg = 0;
}
void dodajDoBuforu(float temperatura, struct buforCykliczny *bufor, char* plikWyjsciowyDlaDanychZBufora){
    if(bufor->przebieg){
        FILE* plik = fopen(plikWyjsciowyDlaDanychZBufora,"a");
        if(plik){
            fprintf(plik,"%.1f\n",bufor->temperatura[bufor->licznik]);
            fclose(plik);
        }
        else printf("Wystapil blad! Nie mozna otworzyc pliku %s\n",plikWyjsciowyDlaDanychZBufora);
    }
    bufor->temperatura[bufor->licznik] = temperatura;
    bufor->licznik++;
    if(bufor->licznik == wielkoscBuforu){
        bufor->przebieg = 1;
        bufor->licznik = 0;
    }
}

struct listaTemp{
    struct listaTemp *elementPoprzedni;
    struct listaTemp *elementNastepny;
    float temperatura;

};
int zaladujILicz(struct listaTemp** poczatekListy, char* plikWejsciowy, float zadanaWartosc){
    FILE* plik = fopen(plikWejsciowy,"r");
    if(plik){
            struct listaTemp* aktualnyElement;
            int licznik = 0;
            float tmp;
            while(1){
            if(feof(plik)){
                break;
            }
            fscanf(plik,"%f\n",&tmp);
            if(tmp > zadanaWartosc) licznik++;
            if(*poczatekListy == NULL){
                struct listaTemp* tmp2 = malloc(sizeof(struct listaTemp));
                tmp2->temperatura = tmp;
                tmp2->elementPoprzedni = NULL;
                tmp2->elementNastepny = NULL;
                *poczatekListy = tmp2;
                aktualnyElement = *poczatekListy;
            }
            else{
            aktualnyElement->elementNastepny = malloc(sizeof(struct listaTemp));
            aktualnyElement->elementNastepny->elementPoprzedni = aktualnyElement;
            aktualnyElement->elementNastepny->elementNastepny = NULL;
            aktualnyElement->elementNastepny->temperatura = tmp;
            aktualnyElement = aktualnyElement->elementNastepny;
            }
    }
    return licznik;
    }else {
        printf("Wystapil blad podczas otwierania pliku\n");
        return 0;
    }
}
void wyczyscListe(struct listaTemp** poczatekListy){
    struct listaTemp* aktualnyElement = *poczatekListy;
    struct listaTemp* kolejnyElement;
    *poczatekListy = NULL;
    while(aktualnyElement){
        kolejnyElement = aktualnyElement->elementNastepny;
        free(aktualnyElement);
        aktualnyElement = kolejnyElement;
    }
}
enum {TEMPERATURA, SILA_WIATRU, OPADY} ostatnieDane;
union pogodynka{

    float temp;
    unsigned short silaW;
    long mmOpadow;
};
void ustawTemperature(union pogodynka* przechowywacz, float temperatura){
    przechowywacz->temp =temperatura;
    ostatnieDane = TEMPERATURA;
}
void ustawSileWiatru(union pogodynka* przechowywacz, unsigned short silaWiatru){
    przechowywacz->silaW = silaWiatru;
    ostatnieDane = SILA_WIATRU;
}
void ustawOpadyDeszczu(union pogodynka* przechowywacz, long mmOpadowDeszczu){
    przechowywacz->mmOpadow = mmOpadowDeszczu;
    ostatnieDane = OPADY;
}
void odczytajPogodynke(union pogodynka* przechowywacz){
    switch(ostatnieDane){
    case TEMPERATURA: printf("Temperatura w pogodynce wynosi %.1f stopni Celsjusza\n",przechowywacz->temp); break;
    case SILA_WIATRU:printf("Sila wiatru wynosi %u stopni w skali Beauforta\n",przechowywacz->silaW);break;
    case OPADY:printf("Ilosc opadow wynosi %lumm/m3\n", przechowywacz->mmOpadow); break;
    }
}
int main()
{
    /*srand(time(NULL));
    struct buforCykliczny bufor;
    przygotujBufor(&bufor);
    int iloscPomiarow = 5;
    while(iloscPomiarow--){
        int tmp = (rand() % ((zakresTempNaPlus-zakresTempNaMinus)*10))+(zakresTempNaMinus*10);
        float temperatura = (float)tmp/10;
        printf("Wygenerowana temperatura: %.1fC\n", temperatura);
        dodajDoBuforu(temperatura,&bufor,"hist.txt");
    }*/
    struct listaTemp** root = malloc(sizeof(struct listaTemp*));
    *root = NULL;
    printf("%i",zaladujILicz(root,"hist.txt",17.2));
    wyczyscListe(root);
    return 0;

   /* union pogodynka pojemnik;
    ustawTemperature(&pojemnik, 22.5);
    odczytajPogodynke(&pojemnik);
    ustawSileWiatru(&pojemnik,8);
    odczytajPogodynke(&pojemnik);
    ustawOpadyDeszczu(&pojemnik,421);
    odczytajPogodynke(&pojemnik);*/

}
