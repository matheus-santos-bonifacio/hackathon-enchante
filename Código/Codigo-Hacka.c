#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

typedef struct linguas{

    //Aqui o jogador pode definir se ele prefere que o jogo apare�a em FR ou em PT.    
    int Preferencia_PTouFR;

    //Aqui o jogador pode definir se ele � proficiente em outras l�nguas, de b�s-int-av� definindo um valor de 0-2.
    int ProficienciaPT;
        int visivelPT;
    int ProficienciaFR;
        int visivelFR;
    int ProficienciaEN;
        int visivelEN;
    int ProficienciaSP;
        int visivelSP;

    //Op��o de personaliza��o, caso o jogador decida adicionar mais l�nguas a seu perfil.
    char Outros1[10];
    int ProficienciaO1;
        int visivelO1;
    char Outros2[10];
    int ProficienciaO2;
        int visivelO2;

} Linguas;

typedef struct player{

    //Personaliz�veis: nome, idade, apar�ncia, bio...
    char Nome[20];
    int idade;
    Linguas Linguagens;
    char Bio[300];

    //Caracter�sticas inerentes a ele
    int Atravessavel;
    Amigos ListaAmigos;
    int N�vel;

} Player;

typedef struct amigos{
    //Fazer como Lista Duplamente Encadeada.
    //Inicializar fun��o s� para isso.

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
    //Fun��o lista de amigos;
    Jogador.N�vel = 0;

    initWindow(screenWidth, screenHeight, "Hackathon Enchante{�}");
    
    //ABAIXO: Tudo o que aparecer� na tela. A l�gica do jogo em si deve estar estruturada acima!

    initDrawing();
    endDrawing();

    return 0;
}