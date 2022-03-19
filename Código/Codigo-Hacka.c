#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

typedef struct player{

    //Personalizáveis: nome, idade, aparência, gostos...
    char Nome[20];
    int idade;
    //Ideias: línguas faladas pelo jogador

    //Características inerentes a ele
    int Atravessavel;
    int Amigos;
    int Nível;

} Player;

typedef struct amigos{}

int main (){

    int screenWidth = 1000;
    int screenHeight = 800;
    Player Jogador;

    Jogador.Atravessavel = 0;
    Jogador.Amigos = 0;
    Jogador.Nível = 0;

    initWindow(screenWidth, screenHeight, "Hackathon Enchante{é}");
    
    //ABAIXO: Tudo o que aparecerá na tela. A lógica do jogo em si deve estar estruturada acima!

    initDrawing();
    endDrawing();


}