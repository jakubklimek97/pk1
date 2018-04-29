/*
 * analityk.c
 *
 *  Created on: 29 kwi 2018
 *      Author: klimek
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

void sortuj(int* tab, int rozmiarTablicy){
	int i = 0;
	int j = --rozmiarTablicy;
	while(j)
	{
		i = 0;
		while(i < j){
			if(tab[i] > tab[i+1]){
				int tmp = tab[i];
				tab[i] = tab[i+1];
				tab[i+1] = tmp;
			}
			++i;
		}
		--j;
	}

}

int main(int argc, char** argv){
		int iloscArgumentow = 0;
		char** tablicaArgumentow;
		int dlugoscArgumentu;
		FILE* argumentyPlik = fopen("propertis.dat","r");
		if(argumentyPlik != NULL){
			fread((void*)&iloscArgumentow,sizeof(int),1,argumentyPlik);
			tablicaArgumentow = malloc(iloscArgumentow*sizeof(char*));
			int i = 0;
			while(i < iloscArgumentow){
				fread((void*)&dlugoscArgumentu,sizeof(int),1,argumentyPlik);
				tablicaArgumentow[i] = malloc(dlugoscArgumentu*sizeof(char));
				fread((void*)tablicaArgumentow[i],sizeof(char),dlugoscArgumentu,argumentyPlik);
				i++;
			}

			fclose(argumentyPlik);
			i = 0;
			int iloscPomiarow = atoi(tablicaArgumentow[0]);
			double* srednie = malloc((iloscArgumentow-2)*sizeof(double));
			memset(srednie,0,(iloscArgumentow-2)*sizeof(double));
			unsigned int* mini = malloc((iloscArgumentow-2)*sizeof(unsigned int));
			memset(mini,20,(iloscArgumentow-2)*sizeof(unsigned int));
			int* max = malloc((iloscArgumentow-2)*sizeof(int));
			memset(max,INT_MIN,(iloscArgumentow-2)*sizeof(int));
			double* mediany = malloc((iloscArgumentow-2)*sizeof(double));
			int** tablicaWynikow = malloc((iloscArgumentow-2)*sizeof(int*));
			while(i< iloscArgumentow -2){
				tablicaWynikow[i] = malloc(iloscPomiarow*sizeof(int));
				i++;
			}
			FILE* plikCsv = fopen(tablicaArgumentow[1],"r");
			if(plikCsv != NULL){
				int i = 0;
				int j;
				while(i < iloscPomiarow){
					j = 0;
					while(j < iloscArgumentow-2){
						fscanf(plikCsv,"%d%*[;]",tablicaWynikow[j]+i);
						srednie[j] += *(tablicaWynikow[j]+i);
						j++;
					}
					i++;
				}
				fclose(plikCsv);
				j = 0;
				while(j< iloscArgumentow-2){
					srednie[j] = srednie[j]/iloscPomiarow;
					j++;
				}
				j = 0;
				if(iloscPomiarow %2){
					while(j< iloscArgumentow-2){
						sortuj(tablicaWynikow[j],iloscPomiarow);
						mediany[j] = tablicaWynikow[j][iloscPomiarow/2];
						mini[j] = tablicaWynikow[j][0];
						max[j] = tablicaWynikow[j][iloscPomiarow-1];
						j++;
					}
				}
				else{
					while(j< iloscArgumentow-2){
						sortuj(tablicaWynikow[j],iloscPomiarow);
						mediany[j] = (double)(tablicaWynikow[j][iloscPomiarow/2] + tablicaWynikow[j][(iloscPomiarow/2)-1])/2;
						mini[j] = tablicaWynikow[j][0];
						max[j] = tablicaWynikow[j][iloscPomiarow-1];
						j++;
					}
				}
				j=0;
				while(j< iloscArgumentow-2){
					printf("%s\n****************************\nSrednia: %f\nMediana: %f\nWartosc minimalna: %d\nWartosc maksymalna: %d\n\n",
						tablicaArgumentow[j+2],srednie[j],mediany[j],mini[j],max[j]);
					j++;
				}

				free(srednie);

				free(max);
				free(mini);
				free(mediany);
				i = 0;
				while(i< iloscArgumentow -2){
					free(tablicaWynikow[i]);
					i++;
				}
				free(tablicaWynikow);

			}
		}
		int i = 0;
		while(i < iloscArgumentow){
			free(tablicaArgumentow[i]);
			i++;
		}
		free(tablicaArgumentow);


	return 0;
}
