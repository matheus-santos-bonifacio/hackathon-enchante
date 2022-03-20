#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <raylib.h>
//#include "libwebsockets.h"

#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 800
#define MAX_INPUT_CHARS 140
#define COLUNAS 18
#define LINHAS 10
#define LARGURA_PIXEL 72
#define ALTURA_PIXEL 44

//Organizar struct de objetos para interação;

typedef struct interacao{
    int aux[4][5];
    //-->Assumem-se, NO MÁX, 5 objetos interação/sala, cada um exigindo 4 variáveis auxiliares (minx, miny, maxx, maxy);
    char Info[30];
} ObjInteracao; //Será um vetor, por sala;

typedef struct linguas{

    //Aqui o jogador pode definir se ele é proficiente em outras línguas, de bás-int-avç definindo um valor de 0-2.
    int ProficienciaPT;
    int ProficienciaFR;        
    int ProficienciaEN;

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
    ptAmigos *fim;

} Amigos;

typedef struct player{

    //Personalizáveis: nome, idade, aparência, bio...
    char Nome[20];
    int Idade;
    Linguas Linguagens;
    char Bio[300];
    Posicao PosicaoPlayer;

    //Características inerentes a ele
    int Atravessavel;
    Amigos ListaAmigos;
    int Nivel; //Gamificação!
    int AlunoAF;

} Player; //Informações provavelmente serão carregadas por meio de um arquivo texto durante o login.

typedef struct posicao_player{

    int PosX;
    int PosY;

} Posicao;

typedef struct local{
    /*---> O cenário que aparecerá na tela deverá ser gerado baseado em um arquivo.txt específico de cada região.
    Entretanto, é preciso ler a matriz do arquivo.txt de sala (um mapa de caracteres) e analisar
    caracter por caracter para conseguir limitar e analisar as interações dos personagens com a
    sala. Isso será implementado por meio de uma função de análise de matriz. 
    
    Logo, essas infos serão preenchidas pela função de análise da sala baseado nas no arquivo.txt*/
    Posicao PosicaoCenario;
    int Atravessavel;
    int TipoInteracao;
    //---> Interações de 0-4, sendo respectivamente: nenhuma, sentar, pegar, conversar, interagir (obj. inanimado);
    //OBS: Resumo lógico: 0 - Atravessa; 1- Atravessa, mas visor da tela é diferente; 2- Não atravessa; 3 - Não atravessa; 
    //     4 - Não atravessa;
    int isDoor;
    int DoorTrue_ProxSala;
    //--->Se for uma "porta", é para ocorrer uma função específica para a troca de cenário.
    //--->O valor DoorTrue_ProxSala será -1 (não é porta) ou 0-4 com o valor da próxima CurrentScreen;

} Cenario;


void LoadMenuPrincipal(int CurrentScreen, FILE* Arq_Lg, FILE* Arq_Pw, Player Jogador, Cenario Sala[][800]){
    
    Image Menu = LoadImage("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Enchantee_capa.png");
    Texture Bg = LoadTextureFromImage(Menu);
    Image Login = LoadImage("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Login.png");
    Texture Lg = LoadTextureFromImage(Login);
    Font Aleo = LoadFont("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Aleo-Italic.ttf");

     while(!(WindowShouldClose));{
        
        switch(CurrentScreen){
            case -1:
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsGestureDetected(GESTURE_TAP) || IsGestureDetected(GESTURE_DOUBLETAP) )
                CurrentScreen = -2; //Mudar para parte de login;
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(Bg, 0, 800, WHITE);
            DrawTextEx(Aleo, "Pressione em qualquer lugar para efetuar login", (Vector2){480,120}, 14,1.5, GRAY);
            EndDrawing();
            break;

            case -2:
            int sucesso;
            sucesso = LoginUser(Arq_Lg, Arq_Pw);
            if (sucesso == 1){
                CurrentScreen = 0;
                LoadCafePrincipal(Jogador, CurrentScreen, Sala);

            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(Lg, 0, 800, WHITE);
            EndDrawing();
            break;

            default:
            printf("Erro inesperado!\n");

            }
        }
    }

}

void LoadCafePrincipal(Player Jogador, int CurrentScreen, Cenario Sala[][800]){
    
    CurrentScreen = 0;

    Image Bg = LoadImage("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Principal");
    Texture Fundo = LoadTextureFromImage(Bg);
    
    while (!WindowShouldClose){

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsGestureDetected(GESTURE_TAP) || 
    IsGestureDetected(GESTURE_DOUBLETAP) )
        JogadorMovimenta(Jogador, Sala, CurrentScreen);

    BeginDrawing();

    DrawTexture(Fundo, 0, 0, WHITE);

    EndDrawing();
    }

}

int main (){

    FILE* Arq_Lg;
    FILE* Arq_Pw;
    int TodasasSalas[5] = {0,1,2,3,4};
    //Inclui Fachada, Principal, Estudos (onde ficam as Redes Sociais + Biblioteca), Palco e Cinema.
    ObjInteracao Objetos[5];
    int CurrentScreen = -1;
    //É o que definirá qual tela deverá aparecer --> Começa no menu principal;
    Player Jogador;
    Cenario Sala[1440][800];

    setlocale(LC_ALL, "Portuguese");

    InitPlayer(&Jogador);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hackathon Enchante{é} - Demo do Time 3");
    LoadMenuPrincipal(CurrentScreen, Arq_Lg, Arq_Pw, Jogador, Sala);
    SetTargetFPS(60);
    
    
    //ACIMA: Toda a lógica do jogo e arquivos.
    //ABAIXO: Tudo o que aparecerá na tela/UX Design estruturado para a biblioteca. 

    BeginDrawing();
    EndDrawing();

    return 0;
}