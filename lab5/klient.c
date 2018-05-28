/*
 * klient.c
 *
 *  Created on: 28 maj 2018
 *      Author: klimek
 */

#ifndef KLIENT_C_
#define KLIENT_C_
#include <stdio.h>
#include "buforCykliczny.h"
#include <zmq.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char** argv){
	if(argc == 3){
		struct buforCykliczny* buforCyk = malloc(sizeof(struct buforCykliczny));
		przygotujBufor(buforCyk);
		FILE* plik = fopen(argv[1],"r");
		if(plik){
			void *context = zmq_ctx_new ();
			void *requester = zmq_socket (context, ZMQ_REQ);
			zmq_connect (requester, "tcp://localhost:51256");
			char bufor[256];
			int wynikOdczytu;
			while(1){
				memset(bufor,0,sizeof(char)*256);
				wynikOdczytu = fscanf(plik,"%[^<]", bufor);
				if(wynikOdczytu == EOF) break;
				char bufor2[256];
				char* wsk = bufor;
				while(sscanf(wsk,"%s",bufor2) != EOF){
					wsk += strlen(bufor2)+1;
					zmq_send(requester,bufor2,256,0);
					zmq_recv(requester, bufor2,256,0);
					char* buforTxt = malloc(sizeof(char)*128);
					sprintf(buforTxt,"%s\n",bufor2);
					wypisz(buforTxt,buforCyk, argv[2]);
				}
				wynikOdczytu = fscanf(plik,"%[^>]", bufor);

				if(wynikOdczytu == EOF) break;
				fgetc(plik);
			}

			zmq_close(requester);
			zmq_ctx_destroy(context);
			fclose(plik);
			wyczyscBufor(buforCyk, argv[2]);
		}
		else fprintf(stderr,"Blad przy otwarciu pliku\n");

	}
	else{
		fprintf(stderr,"Program przyjmuje 2 argumenty - nazwe pliku html i plik z logiem\n");
	}
	return 0;
}


#endif /* KLIENT_C_ */
