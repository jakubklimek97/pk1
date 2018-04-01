#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void cw1(int* tab);
int cw2(int* tab, size_t rozmiar, int szukanaLiczba);
int cw4(int* tab, size_t rozmiar, int szukanaLiczba);

int main()
{
    /* polecenie 1 */
    int tab2[20];
    cw1(tab2);
    /*polecenie 3 */
        int tab[7] = { 1,3,5,17,22, 2, 3226};
    int pozycja = cw2(tab,7,3226);
    if(pozycja != -2){
        printf("%s %d","Liczba wystepuje na pozycji: \n",pozycja);
    }
    else printf ("Liczba nie znajduje sie w tablicy\n");

    return 0;
}
void cw1(int* tab){
    int i =0;
    while(i<20){
        scanf("%d",(tab+i++)); /* pobieranie 20 liczb do tablicy */
    }
    printf("%s","Wprowadzone liczby:\n");
    while(i){
        printf("%d\n", *(tab+(20-(i--)))); /* wypisywanie liczb */
    }
}
int cw2(int* tab, size_t rozmiar, int szukanaLiczba){
    if(rozmiar > 0){ /* tylko dla rozmiarów > 0 */
        int i = 0;
        while(i < rozmiar && tab[i] != szukanaLiczba){
            i++; /* jeżeli to nie szukana liczba, zwiększ licznik o 1 */
        }
        if(i < rozmiar){
            return i; /* jeżeli i mieści się w zakresie tablicy */
        }
    }
    return -2;
}

int cw4(int* tab, size_t rozmiar, int szukanaLiczba){
    if(rozmiar > 0){ /* tylko dla rozmiarów > 0 */
        int i = 0;
        while(i < rozmiar && *(tab+i) != szukanaLiczba){
            i++; /* jeżeli to nie szukana liczba, zwiększ licznik o 1 */
        }
        if(i < rozmiar){
            return i; /* jeżeli i mieści się w zakresie tablicy */
        }
    }
    return -2;
}

/*funkcja zwraca 0 jeżeli kod IBAN jest poprawny lub 1 jeżeli nie jest.  */
int sprawdzIban(const char* wsk){
    int dlugosc = strlen(wsk);
    enum kraj{
        POLSKA = 1,
        NIEMCY,
        CYPR
    };
    enum kraj wybranyKraj = 0;
    int i;
    if(*wsk == 'P' && *(wsk+1) == 'L' && dlugosc == 28) wybranyKraj = POLSKA;
    else if(*wsk == 'D' && *(wsk+1) == 'E' && dlugosc == 22) wybranyKraj = NIEMCY;
    else if(*wsk == 'C' && *(wsk+1) == 'Y' && dlugosc == 28) wybranyKraj = CYPR;
/*czy dla takiego kraju zaimplementowano */
    if(wybranyKraj > 0)
    {
        for(i =0; i <= dlugosc; ++i){
            switch(wybranyKraj){
    /*sprawdza czy litery i cyfry są na odpowiednich pozycjach*/
                case POLSKA: {
                    if( i > 1){
                        if(wsk[i] > 57 || wsk[i] < 48 ) return 1;
                    }
                    else{
                        if(wsk[i] < 65 || wsk[i] > 90) return 1;
                    }
                    break;
                }
                case NIEMCY:{ if( i > 1){
                        if(wsk[i] > 57 || wsk[i] < 48 ) return 1;
                    }
                    else{
                        if(wsk[i] < 65 || wsk[i] > 90) return 1;
                    }
                    break;
                }
                case CYPR:{ if( i > 1 && i < 12){
                        if(wsk[i] > 57 || wsk[i] < 48 ) return 1;
                    }
                    else if(i < 2){
                        if(wsk[i] < 65 || wsk[i] > 90) return 1;
                    }
                    break;
                }
            }
        }
/*tworzy nową tablicę znaków z przeniesionymi pierwszymi 4 elementami na koniec */
        int liczba = 0;
        char* tmpTab = (char*)malloc((dlugosc+1)*sizeof(char));
        strcpy(tmpTab,wsk+4);
        for(i = 0; i <4; ++i){
            *(tmpTab+dlugosc-4+i) = *(wsk+i);
        }
        *(tmpTab+dlugosc) = '\0';
        char *tabWsk = tmpTab;

/*tworzy z numeru IBAN liczbe i liczy resztę z dzielenia jej przez 97 */
        while(*tabWsk != '\0'){
            if(*tabWsk >= 48 && *tabWsk <= 57){
                liczba*= 10;
                liczba+=(*(tabWsk++)-48);
                liczba%=97;
            }
            else if(*tabWsk >=65 && *tabWsk <= 90){
                liczba*=100;
                liczba+=(*(tabWsk++)-55);
                liczba%=97;
            }
            else{ /* pojawił się nieokreślony znak */
                free(tmpTab); /*zwalnia pamięć zarezerwowaną funkcją malloc */
                return 1;
            }
        }
        free(tmpTab); /*zwalnia pamięć zarezerwowaną funkcją malloc */
        if(liczba == 1){
            return 0;
        }
        else return 1;
    }
    else{
        return 1;
    }
}

