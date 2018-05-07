#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*centrala glowna */
enum stan{
    nasluch,
    polaczenie,
    oczekiwanie,
    wolny,
    zajety
};
enum operacja{
    polacz,
    rozlacz
};
struct Port{
    short typPortu;
    struct centrala* cel;
    int id;
    enum stan stanPortu;
};
struct centrala{
    struct Port* porty;
};
struct Port* pobierzPort(struct centrala** ptr, int nrPortu){
    return (*ptr)->porty+nrPortu;
}
void init(struct centrala** ptr, int iloscPortow){
    *ptr = malloc(sizeof(struct centrala));
    (*ptr)->porty =  (struct Port*)malloc(sizeof(struct Port)* iloscPortow);
}
void ustawPort(struct centrala** ptr, int port,int typPortu, struct centrala* cel){
    (*ptr)->porty[port].typPortu = typPortu;
    if(typPortu == 0){
        (*ptr)->porty[port].id = port;
        (*ptr)->porty[port].stanPortu = wolny;
    }else {
        (*ptr)->porty[port].cel = cel;
    }
}
enum stan zapytajPort(struct Port* ptr){
    return (ptr)->stanPortu;
};
void loguj(char* msg){
    printf(msg);
}
void wykonajOperacje(struct centrala** ptr, int port, struct centrala** celPtr, int celPort, enum operacja Operacja){
    if(Operacja == polacz){
        if(zapytajPort(pobierzPort(ptr,port))== wolny || zapytajPort(pobierzPort(ptr,port))== oczekiwanie){
            pobierzPort(ptr,port)->stanPortu = oczekiwanie;
            if(pobierzPort(celPtr, celPort)->stanPortu == )
        }
    }
}
int main()
{
    struct centrala *centrala1;
    init(&centrala1,4);
    ustawPort(&centrala1,0,0, NULL);
    ustawPort(&centrala1,1,0, NULL);
    ustawPort(&centrala1,2,0, NULL);
    ustawPort(&centrala1,3,0, NULL);






}
