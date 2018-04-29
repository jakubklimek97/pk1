#include <stdio.h>
#include <string.h>


int main(int argc, char** argv){
	if(argc > 3){
		int i = 1;
		FILE* argumentyPlik = fopen("propertis.dat","wb");
		if(argumentyPlik!= NULL){
			int iloscLiter;
			int iloscArg = argc -1;
			fwrite((void*)&iloscArg,sizeof(int),1,argumentyPlik);

			while(i<argc){
				iloscLiter = strlen(argv[i])+1;
				fwrite((void*)&iloscLiter,sizeof(int),1,argumentyPlik);
				fwrite((void*)argv[i],sizeof(char),iloscLiter,argumentyPlik);
				i++;
			}
			fclose(argumentyPlik);
			srand(time(NULL));
					int iloscPomiarow = atoi(argv[1]);
					int iloscKategorii = argc -3;
					int tmpWynik;
					if(iloscKategorii > 0){
						FILE* plikWyjsciowy =fopen(argv[2],"w");
						while(iloscPomiarow){

							i = iloscKategorii;
							while(i){
								tmpWynik = (rand() % 200) - 100;
								fprintf(plikWyjsciowy,"%d",tmpWynik);
								if(i > 1) fputs(";",plikWyjsciowy);
								else if(i == 1)fputs("\n",plikWyjsciowy);
								i--;
							}
							iloscPomiarow--;
						}
						fclose(plikWyjsciowy);
					}
		}


	}
	else{
		printf("generator <ilosc pomiarow> <nazwaPliku.csv> <kategoria>  [<kategoria> ...]\n");
	}
	return 0;
}


