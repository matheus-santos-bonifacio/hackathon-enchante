#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
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

typedef struct amigos{
    //Fazer como Lista Duplamente Encadeada.
    //Inicializar função só para isso.

    int quantidade;
    Amigos* primeiro;
    Amigos* prox;
    Amigos* ant;

} Amigos;

typedef struct player{

    //Personalizáveis: nome, idade, aparência, bio...
    char Nome[20];
    int Idade;
    Linguas Linguagens;
    char Bio[300];

    //Características inerentes a ele
    int Atravessavel;
    Amigos ListaAmigos;
    int Nível;

} Player; //Informações provavelmente serão carregadas por meio de um arquivo texto durante o login.

//FUNÇÕES

int PersonalizaPlayer(Player* Jogador){

    FILE *Arq;

    if (Arq == NULL){

        Arq = fopen("player.c", "w");

        printf("Escolha um nome de exibição.\n");
        getchar();
	    fgets(Jogador->Nome, 20, stdin);

        printf("Idade\n");
        scanf("%d", &Jogador->Idade);

        printf("Quais línguas você fala?\n");
        //Função de linguagens;

        printf("Conte um pouco sobre você! Insira uma bio.\n");
        //Opcional;
        getchar();
	    fgets(Jogador->Bio, 300, stdin);

        



    }

}

//SALAS

int main (){

    int screenWidth = 1000;
    int screenHeight = 800;
    Player Jogador;

    setlocale(LC_ALL, "Portuguese");

    initWindow(screenWidth, screenHeight, "Hackathon Enchante{é}");
    
    //ABAIXO: Tudo o que aparecerá na tela. A lógica do jogo em si deve estar estruturada acima!
    //Fazer em função para ficar mais organizado.

    initDrawing();
    endDrawing();

    return 0;
}