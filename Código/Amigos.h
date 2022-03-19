#include <stdio.h>
#include <stdlib.h>
#include "Codigo-Hacka.c"

//Tipo de dado exportado
struct ponteiros{

    Player *info;
    struct ponteiros *prox;
    struct ponteiros *ant;

};

typedef struct ponteiros *ptAmigos;

typedef struct amigos{
    //Fazer como Lista Duplamente Encadeada;
    int quantidade;
    ptAmigos *primeiro;

} Amigos;

//Tipo de função exportada

Amigos* InicializaLista();
Amigos* InsereCrescente(Amigos* ListaAmigos);
Amigos* ImprimeLista(Amigos* ListaAmigos);
Amigos* RemoverAmigo(Amigos* ListaAmigos);
Amigos* DestroiLista(Amigos* ListaAmigos);

