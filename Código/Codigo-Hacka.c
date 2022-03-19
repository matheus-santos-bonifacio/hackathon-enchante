#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

struct Player{

    //Personalizáveis: nome, idade, aparência, gostos...
    char[20] Nome;
    int idade;
    //Ideias: línguas faladas pelo jogador

    //Características inerentes a ele
    int Atravessavel = 0;
    

}


int main (){

    int screenWidth = 1000;
    int screenHeight = 800;

    initWindow(screenWidth, screenHeight, "Hackathon Enchante{é}");
    
    //ABAIXO: Tudo o que aparecerá na tela. A lógica do jogo em si deve estar estruturada acima!

    initDrawing();
    endDrawing();


}