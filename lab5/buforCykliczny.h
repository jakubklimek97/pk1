/*
 * buforCykliczny.h
 *
 *  Created on: 28 maj 2018
 *      Author: klimek
 */

#ifndef BUFORCYKLICZNY_H_
#define BUFORCYKLICZNY_H_

#define wielkoscBuforu 5

struct buforCykliczny{
    unsigned short przebieg;
    unsigned int licznik;
    char* wyjscie[wielkoscBuforu];
};
void wyczyscBufor(struct buforCykliczny* bufor, char* plikWyjsciowyDlaDanychZBufora);
void przygotujBufor(struct buforCykliczny* bufor);
void dodajDoBuforu(char* wyjscie, struct buforCykliczny *bufor, char* plikWyjsciowyDlaDanychZBufora);
void wypisz(char* komunikat, struct buforCykliczny* bufor, char* plikWyjsciowy);

#endif /* BUFORCYKLICZNY_H_ */
