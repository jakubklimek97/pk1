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
int main()
{
    srand(time(NULL));
    struct buforCykliczny bufor;
    przygotujBufor(&bufor);
    int iloscPomiarow = 5;
    while(iloscPomiarow--){
        int tmp = (rand() % ((zakresTempNaPlus-zakresTempNaMinus)*10))+(zakresTempNaMinus*10);
        float temperatura = (float)tmp/10;
        printf("Wygenerowana temperatura: %.1fC\n", temperatura);
        dodajDoBuforu(temperatura,&bufor,"hist.txt");
    }
    return 0;
}
