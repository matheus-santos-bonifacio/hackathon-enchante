#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

typedef struct linguas{

    //Aqui o jogador pode definir se ele prefere que o jogo apareça em FR ou em PT.    
    int Preferencia_PTouFR;

    //Aqui o jogador pode definir se ele é proficiente em outras línguas, de bás-int-avç definindo um valor de 0-2.
    int ProficienciaPT;
        int visivelPT;
    int ProficienciaFR;
        int visivelFR;
    int ProficienciaEN;
        int visivelEN;
    int ProficienciaSP;
        int visivelSP;

    //Opção de personalização, caso o jogador decida adicionar mais línguas a seu perfil.
    char Outros1[10];
    int ProficienciaO1;
        int visivelO1;
    char Outros2[10];
    int ProficienciaO2;
        int visivelO2;

} Linguas;

typedef struct player{

    //Personalizáveis: nome, idade, aparência, bio...
    char Nome[20];
    int idade;
    Linguas Linguagens;
    char Bio[300];

    //Características inerentes a ele
    int Atravessavel;
    Amigos ListaAmigos;
    int Nível;

} Player;

typedef struct amigos{
    //Fazer como Lista Duplamente Encadeada.
    //Inicializar função só para isso.

    int quantidade;
    Amigos* primeiro;
    Amigos* prox;
    Amigos* ant;

} Amigos;

int main (){

    int screenWidth = 1000;
    int screenHeight = 800;
    Player Jogador;

    Jogador.Atravessavel = 0;
    //Função lista de amigos;
    Jogador.Nível = 0;

    initWindow(screenWidth, screenHeight, "Hackathon Enchante{é}");
    
    //ABAIXO: Tudo o que aparecerá na tela. A lógica do jogo em si deve estar estruturada acima!

    initDrawing();
    endDrawing();

    return 0;
}