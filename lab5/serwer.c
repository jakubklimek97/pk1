/*
 * serwer.c
 *
 *  Created on: 28 maj 2018
 *      Author: klimek
 */


#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include "liczbyRzymskie.h"
#include <stdlib.h>
#include "buforCykliczny.h"
static volatile int czyDzialac = 1;
void odDzialaj(int e){
	czyDzialac = 0;
}
int main(int argc, char** argv){
	if(argc < 2 ) return -1;
	struct buforCykliczny* buforCyk = malloc(sizeof(struct buforCykliczny));
	przygotujBufor(buforCyk);
	signal(SIGINT,odDzialaj);
	signal(SIGQUIT,odDzialaj);
	void *context = zmq_ctx_new ();
	void *responder = zmq_socket (context, ZMQ_REP);
	int rc = zmq_bind (responder, "tcp://*:51256");
	assert (rc == 0);
	char* bufor = malloc(sizeof(char)*128);
	sprintf(bufor,"Zaczynam prace\n");
	wypisz(bufor,buforCyk, argv[1]);
	    while (czyDzialac) {
	        char buffer [256];
	        int odebrano = -1;
	        odebrano = zmq_recv (responder, buffer,256, ZMQ_NOBLOCK);
	        if(odebrano != -1){
	        	printf ("Dostalem %s\n",buffer);
	        	int rzymska = romanToDec(buffer);
	        	bufor = malloc(sizeof(char)*128);
	        	if(rzymska == -1) sprintf(bufor,"%s to nie liczba rzymska\n",buffer);
	        	else sprintf(bufor,"Przetworzono liczbe %s na %d\n",buffer, rzymska);
	        	 wypisz(bufor,buforCyk, argv[1]);
	        	zmq_send(responder,bufor,256,0);
	        	odebrano = -1;
	        	sleep (1);
	        }

	    }
	    bufor = malloc(sizeof(char)*128);
	    sprintf(bufor,"Koncze prace\n");
	    wypisz(bufor,buforCyk, argv[1]);
	    wyczyscBufor(buforCyk, argv[1]);
	    printf("Bufor wyczyszczony\n");
	   /* free(bufor);
	    zmq_close(responder);
	    zmq_ctx_destroy(context);*/

	    return 0;

}
