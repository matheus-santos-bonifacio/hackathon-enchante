#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

typedef struct player{

    //Personaliz�veis: nome, idade, apar�ncia, gostos...
    char Nome[20];
    int idade;
    //Ideias: l�nguas faladas pelo jogador

    //Caracter�sticas inerentes a ele
    int Atravessavel;
    int Amigos;
    int N�vel;

} Player;

typedef struct amigos{}

int main (){

    int screenWidth = 1000;
    int screenHeight = 800;
    Player Jogador;

    Jogador.Atravessavel = 0;
    Jogador.Amigos = 0;
    Jogador.N�vel = 0;

    initWindow(screenWidth, screenHeight, "Hackathon Enchante{�}");
    
    //ABAIXO: Tudo o que aparecer� na tela. A l�gica do jogo em si deve estar estruturada acima!

    initDrawing();
    endDrawing();


}