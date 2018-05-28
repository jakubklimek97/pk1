#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liczbyRzymskie.h"

char *znaki[] ={
		"M\0",
		"CM\0",
		"D\0",
		"CD\0",
		"C\0",
		"XC\0",
		"L\0",
		"XL\0",
		"X\0",
		"IX\0",
		"V\0",
		"IV\0",
		"I\0"
};
int wartosci[] = {
		1000,
		900,
		500,
		400,
		100,
		90,
		50,
		40,
		10,
		9,
		5,
		4,
		1
};
int decToRoman(int liczba, char* bufor){
		if(liczba < 0) {printf("Za mala liczba\n"); return 1;}
	 	else if(liczba > 3999) {printf("Za duza liczba\n"); return 1;}
		int i;
		for(i =0; i < 13; i++){
			if(liczba >= wartosci[i]){
				strcat(bufor,znaki[i]);
				liczba -= wartosci[i];
				i--;
			}
		}
		return 0;
}
int GetWartosc(char znak){

	    if (znak == 'I')
	        return 1;
	    if (znak == 'V')
	        return 5;
	    if (znak == 'X')
	        return 10;
	    if (znak == 'L')
	        return 50;
	    if (znak == 'C')
	        return 100;
	    if (znak == 'D')
	        return 500;
	    if (znak == 'M')
	        return 1000;

	    return -1;
}
int romanToDec(char* liczba)
{
    int wynik = 0;
    int i;
    int dlugosc = strlen(liczba);
    for (i=0; i< dlugosc; i++)
    {
        int w1 = GetWartosc(*(liczba+i));
        if(w1 == -1) return -1;
        if (i+1 < dlugosc)
        {
            int nast = GetWartosc(*(liczba+i+1));
            if(nast == -1) return -1;
            if (w1 >= nast)
            {
                wynik = wynik + w1;
            }
            else
            {
                wynik = wynik + nast - w1;
                i++;
            }
        }
        else
        {
            wynik = wynik + w1;
            i++;
        }
    }
    return wynik;
}
