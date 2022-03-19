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

struct ponteiros{

    Player info;
    struct ponteiros *prox;
    struct ponteiros *ant;

};

typedef struct ponteiros *ptAmigos;

typedef struct amigos{
    //Fazer como Lista Duplamente Encadeada;
    int quantidade;
    ptAmigos *primeiro;

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
    int Nivel;

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

        Jogador->ListaAmigos.primeiro = NULL;
        Jogador->Nivel = 0;
        Jogador->Atravessavel = 0;

        //Implementar linguagens e lista de amigos depois!!
        //Escrever no arquivo texto exatamente como escrito na struct!
        fprintf(Arq, "%s, %d, %s, %d, %d",
        Jogador->Nome, Jogador->Idade, Jogador->Bio,
        Jogador->Nivel, Jogador->Atravessavel);

        fclose(Arq);
        //Sucesso;
        return 1;
    }
    else {
        //Adiciona funcionalidades na tela de personalização implementadas com as informações.
        //Copia tudo para o arquivo já existente, sobre-escrevendo o antigo.
    }


        



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