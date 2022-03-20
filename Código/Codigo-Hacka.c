#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
//Bibliotecas externas!
#include <raylib.h>
#include <libwebsockets.h>
#include <signal.h>

#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 800
#define MAX_INPUT_CHARS 140
#define COLUNAS 18
#define LINHAS 10
#define LARGURA_PIXEL 72
#define ALTURA_PIXEL 44

#define LWS_PLUGIN_STATIC
#include "protocol_lws_minimal.c"

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

//FUNÇÕES PRÉ-JOGO

void CadastroLinguas(Player* Jogador){

    int proficiente;

    printf("Qual a sua proficiência em...\n");

    //Fazer lógica de select para ficar mais otimizado.

    printf("\tPT-BR?\n");
    printf("|------------------------|");
    printf("|0 - Básico              |\n");
    printf("|------------------------|");
    printf("|1 - Intermediário       |\n");
    printf("|------------------------|");
    printf("|2 - Avançado ou Fluente |\n");
    printf("|------------------------|");
    
    scanf("%d", &proficiente);
    Jogador->Linguagens.ProficienciaPT = proficiente;

    printf("\tFR?\n");
    printf("|------------------------|");
    printf("|0 - Básico              |\n");
    printf("|------------------------|");
    printf("|1 - Intermediário       |\n");
    printf("|------------------------|");
    printf("|2 - Avançado ou Fluente |\n");
    printf("|------------------------|");
    
    scanf("%d", &proficiente);
    Jogador->Linguagens.ProficienciaFR = proficiente;

    printf("\tEN?\n");
    printf("|------------------------|");
    printf("|0 - Básico              |\n");
    printf("|------------------------|");
    printf("|1 - Intermediário       |\n");
    printf("|------------------------|");
    printf("|2 - Avançado ou Fluente |\n");
    printf("|------------------------|");
    
    scanf("%d", &proficiente);
    Jogador->Linguagens.ProficienciaEN = proficiente;

}

int PersonalizaPlayer(Player* Jogador){

    FILE *Arq;

    if (Arq == NULL){

        Arq = fopen("player.c", "w");

        printf("Escolha um nome de exibição.\n");
        getchar();
	    fgets(Jogador->Nome, 20, stdin);

        printf("Idade\n");
        scanf("%d", &Jogador->Idade);

        printf("Línguas estrangeiras\n");
        CadastroLinguas(Jogador);

        printf("Conte um pouco sobre você! Insira uma bio.\n");
        //Opcional;
        getchar();
	    fgets(Jogador->Bio, 300, stdin);

        Jogador->ListaAmigos.primeiro = InicializaLAmigos();
        Jogador->Nivel = 0;
        Jogador->Atravessavel = 0;
        Jogador->PosicaoPlayer.PosX = SCREEN_WIDTH/2;
        Jogador->PosicaoPlayer.PosY = SCREEN_HEIGHT/2;

        //Se for aluno da AF, isso deverá ser puxado com base no arquivo.txt do cadastro.

        //Implementar lista de amigos depois!!
        //Escrever no arquivo texto exatamente como escrito na struct!
        fprintf(Arq, "%s, %d, %d, %d, %d, %s, %d, %d",
        Jogador->Nome, Jogador->Idade, Jogador->Linguagens.ProficienciaPT,
        Jogador->Linguagens.ProficienciaFR, Jogador->Linguagens.ProficienciaEN,
        Jogador->Bio,Jogador->Nivel, Jogador->Atravessavel);

        fclose(Arq);
        //Sucesso;
        return 1;
    }
    else {
        //Adiciona funcionalidades na tela de personalização implementadas com as informações.
        //Copia tudo para o arquivo já existente, sobre-escrevendo o antigo.
    }
}

int CadastroUser(){

    FILE* NewInfo_Login;
    FILE* NewInfo_Senha;
    
    char NovoLogin[30];
    char Senha[72];
    int u, d, s;
    int sucesso;

    printf("Defina o login!");
    getchar();
    fgets(NovoLogin, 30, stdin);

    printf("Defina a senha!");
    do {
        getchar();
        fgets(Senha, 72, stdin);
        
        for (int i = 0; i < 72; i++){
            if ( isupper(Senha[i]) )
                u++;
            if ( isdigit(Senha[i]) )
                d++;
            if ( ispunct(Senha[i]) ) 
                s++;        
            if ( (u + d + s) >= 3) {
                printf("Sua senha é forte!\n");
                break;
            }
        }

            if (u == 0 || d == 0 || s == 0)
                printf("Sua senha é fraca, tente novamente!");
    }
    while(u == 0 || d == 0 || s == 0);

    NewInfo_Login = fopen("INFO_Login.txt", "a+");
    sucesso = fprintf(NewInfo_Login, "%s\n", NovoLogin);
    if (sucesso < 0){
        printf("Algo deu errado, falha no cadastro!\n");
        fclose(NewInfo_Login);
        return 0;
    }
    else {
        NewInfo_Senha = fopen("INFO_Pw.txt", "a+");
        sucesso = fprintf(NewInfo_Senha, "%s\n", Senha);
        if (sucesso < 0){
            printf("Algo deu errado, falha no cadastro!\n");
            fclose(NewInfo_Senha);
        }
        else {
            printf("Cadastro realizado com sucesso!\n");
            fclose(NewInfo_Senha);
            return 1;
        }
    }
}

int LoginUser(FILE* Arq_Lg, FILE* Arq_Pw){

    char Login[30], Correspondente[30];
    char Senha[72], Insere[72];
    int n = 0;
    int Encontrado, Senha_Correta;

    printf("Login\n");
    getchar();
    fgets(Login, 30, stdin);

    printf("Senha\n");
    getchar();
    fgets(Insere, 72, stdin);

    Arq_Lg = fopen("INFO_Login.txt", "r");

    while(!feof(Arq_Lg) && Encontrado == 0){
        fscanf(Arq_Lg, "%s", &Correspondente);
        n++;
        if (!strcmp(Correspondente, Login)){
            Encontrado = 1;
        }
    }

    if (Encontrado = 0)
        printf("Login incorreto!\n");

    fclose(Arq_Lg);

    Arq_Pw = fopen("INFO_Pw.txt", "r");
    fseek(Arq_Pw, (n-1), 0);
    fscanf(Arq_Pw, "%s", Senha);

    if (!strcmp(Insere, Senha))
        Senha_Correta = 1;
        //Adquirir info do usuário armazenada em um arquivo.txt para preencher o Player;
    else{
        printf("Senha incorreta!\n");
    }
    
}

//------> Funções relacionadas à LDE da Lista de Amigos;

Amigos* InicializaLAmigos(void){

    return NULL;
}

Amigos* AdicionarAmigos(Amigos* ListaAmigos, ptAmigos Infos, Player NOT_Jogador){

    ptAmigos Novo;
    ptAmigos ptAux = ListaAmigos;
    ptAmigos* ptAnt = NULL;

    Novo = (ptAmigos*)malloc(sizeof(ptAmigos*));
    Infos->info = NOT_Jogador;

    Novo->info = Infos->info;
    
    //Sempre inserir no começo para dar prioridade aos novos amigos.

    if (ListaAmigos == NULL){

        ListaAmigos->primeiro = Novo;
        ListaAmigos->fim = ptAux;
        Novo->prox = ptAux;
        Novo->ant = NULL;
        ptAux->prox = NULL;
        ptAux->ant = Novo;

    }
    else {        
        ListaAmigos->primeiro = Novo;
        Novo->prox = ptAux;
        Novo->ant = NULL;
        while(ptAux->prox != NULL);{
        ptAux->ant = ptAux;
        ptAux = ptAux->prox;
        ptAux->prox = ptAux->prox->prox;
        }
    }

    return ListaAmigos;

}

void ImprimeLista(Amigos* ListaAmigos){

    ptAmigos PtAux = ListaAmigos;
    
    if (ListaAmigos == NULL)
        printf("Nenhum amigo ainda!\n");
    else{
        do {
            printf("Nome = %s\n, Idade = %d\n",
            PtAux->info.Nome,
            PtAux->info.Idade);
            PtAux = PtAux->prox;
        } 
        while (PtAux != NULL);
    }
}

void InitPlayer(Player *Jogador){

    Jogador->PosicaoPlayer.PosX = SCREEN_WIDTH/2;
    Jogador->PosicaoPlayer.PosY = 0;
    drawPlayer(Jogador, 1);

}

void drawPlayer(Player *Jogador, int caso){

    Image Jogador = LoadImage("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Persona 02a");
    Texture Pl = LoadTextureFromImage(Jogador);
    Image Jogador_Andando = LoadImage("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Persona 02b");
    Texture Pl_wlk = LoadTextureFromImage(Jogador_Andando));
    Image Jogador_Falando= LoadImage("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Persona 02c");
    Texture Pl_tlk = LoadTextureFromImage(Jogador_Falando);
    Image Jogador_Sentado = LoadImage(Image Jogador_Andando("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Persona 02d");
    Texture Pl_st = LoadTextureFromImage(Jogador_Sentado);

    Switch(caso){

        case 1:
        //Andando
        while(!(WindowShouldClose));{

            BeginDrawing();
            DrawTexture(Pl_wlk, (Jogador->PosicaoPlayer.Posx)/COLUNAS, ((Jogador->PosicaoPlayer.Posx)/LINHAS), RAYWHITE);
            JogadorMovimenta(Jogador, Sala, CurrentScreen);
            EndDrawing();

        }
        break;

        case 2:
        //sentando
        while(!(WindowShouldClose));{

            BeginDrawing();
            DrawTexture(Pl_st, (Jogador->PosicaoPlayer.Posx)/COLUNAS, ((Jogador->PosicaoPlayer.Posx)/LINHAS), RAYWHITE);
            JogadorMovimenta(Jogador, Sala, CurrentScreen);
            EndDrawing();

        }
        break;

        case 3:
        //Pega
        while(!(WindowShouldClose));{

            BeginDrawing();
            //Textura temporária. Deveria ser a correta de pegar.
            DrawTexture(Pl, (Jogador->PosicaoPlayer.Posx)/COLUNAS, ((Jogador->PosicaoPlayer.Posx)/LINHAS), RAYWHITE);
            JogadorMovimenta(Jogador, Sala, CurrentScreen);
            EndDrawing();

        }
        break;

        case 4:
        //Falando
        while(!(WindowShouldClose));{

            BeginDrawing();
            DrawTexture(Pl_tlk, (Jogador->PosicaoPlayer.Posx)/COLUNAS, ((Jogador->PosicaoPlayer.Posx)/LINHAS), RAYWHITE);
            JogadorMovimenta(Jogador, Sala, CurrentScreen);
            EndDrawing();

        }
        break;

        case 5:
        //Interage objeto.
        while(!(WindowShouldClose));{

            //Textura temporária. Deveria ser a correta de interagir com objetos inanimados.
            BeginDrawing();
            DrawTexture(Pl_talk, (Jogador->PosicaoPlayer.Posx)/COLUNAS, ((Jogador->PosicaoPlayer.Posx)/LINHAS), RAYWHITE);
            JogadorMovimenta(Jogador, Sala, CurrentScreen);
            EndDrawing();

        }
        break;

        default:
        printf("Erro na elaboração da Sprite!");
    }
    
}

Amigos* RemoveLista(Amigos* ListaAmigos, ptAmigos Infos){

    ptAmigos PtAux;
    ptAmigos PtProx;
    int elemento;

    PtProx = PtAux->prox;

    //Quando a pessoa clica em remover uma determinada pessoa de sua lista de amigos.

    printf("Deseja mesmo remover esta pessoa da sua lista de amigos?\n");
    printf("1 - Sim || 0 - Não");
    scanf("%d", &elemento);

    if (elemento == 1){

        if (ListaAmigos == NULL)
            return NULL;
        else {

            for (PtAux = ListaAmigos; (PtAux->prox = NULL); PtAux = PtProx){
                for (PtAux; PtAux->prox = NULL; PtAux = PtProx){
                        free(PtAux);
                        PtAux = PtProx;
                    }
                    break;
                }
            }

        }

    return ListaAmigos;
}


void PreencheInfosSala(int CurrentScreen, Cenario* Sala[][800], ObjInteracao Objetos[]){

    //CADA SALA/TELA DEVERÁ SER ANALISADA INDIVIDUALMENTE!

       switch(CurrentScreen){
        
        /*Fachada
        case 0:

        */

        //Café Principal
        case 1:

        int m, n;
        int aux[4]={0,0,3000,3000};

        for (m = 0; m < 1440; m++){
            for (n = 0; n < 800; n++){
                //Parede
                if ((Sala[m][n]->PosicaoCenario.PosX >= 0 && Sala[m][n]->PosicaoCenario.PosX < 1440)&&
                Sala[m][n]->PosicaoCenario.PosY >= 0 && Sala[m][n]->PosicaoCenario.PosY < 475){
                    Sala[m][n]->Atravessavel = 0;
                    Sala[m][n]->isDoor = 0;
                    Sala[m][n]->TipoInteracao = 0;
                }
                //Cadeira #1
                else if ((Sala[m][n]->PosicaoCenario.PosX >= 625 && Sala[m][n]->PosicaoCenario.PosX <740)&&
                Sala[m][n]->PosicaoCenario.PosY >= 380 && Sala[m][n]->PosicaoCenario.PosY < 440){
                    Sala[m][n]->Atravessavel = 1;
                    Sala[m][n]->isDoor = 0;
                    Sala[m][n]->TipoInteracao = 1;
                }
                //Cadeira #2
                else if ((Sala[m][n]->PosicaoCenario.PosX >= 905 && Sala[m][n]->PosicaoCenario.PosX < 1015)&&
                Sala[m][n]->PosicaoCenario.PosY >= 370 && Sala[m][n]->PosicaoCenario.PosY < 425){
                    Sala[m][n]->Atravessavel = 1;
                    Sala[m][n]->isDoor = 0;
                    Sala[m][n]->TipoInteracao = 1;
                }
                //Bateria
                else if ((Sala[m][n]->PosicaoCenario.PosX >= 0 && Sala[m][n]->PosicaoCenario.PosX <320)&&
                Sala[m][n]->PosicaoCenario.PosY >= 240 && Sala[m][n]->PosicaoCenario.PosY < 540){
                    char Nome[20];
                    Sala[m][n]->Atravessavel = 0;
                    Sala[m][n]->isDoor = 0;
                    Sala[m][n]->TipoInteracao = 3;
                    if (m >= aux[0])
                        aux[0] = m;
                    if (m <= aux[2])
                        aux[2] = m;
                    if (n >= aux[1])
                        aux[1] = n;
                    if (n <= aux[3])
                        aux[3] = n;
                }
                //Nada relevante para a movimentação;
                else {
                    Sala[m][n]->Atravessavel = 1;
                    Sala[m][n]->isDoor = 0;
                    Sala[m][n]->TipoInteracao = 0;
                }
                }
            }
            //Preenche as variáveis auxiliares para uso posterior;
            if ((Sala[m][n]->PosicaoCenario.PosX >= 0 && Sala[m][n]->PosicaoCenario.PosX <320)&&
                Sala[m][n]->PosicaoCenario.PosY >= 240 && Sala[m][n]->PosicaoCenario.PosY < 540){
                char Nome[30] = "Bateria";
                strcpy(Objetos[0].Info, Nome);
                Objetos[0].aux[0][0] = aux[0];
                Objetos[0].aux[0][1] = aux[1];
                Objetos[0].aux[0][2] = aux[2];
                Objetos[0].aux[0][3] = aux[3];
                }
            break;

            case (1):
            break;

            case(2):
            break;

            case(3):
            break;

            case(4):
            break;

            default:
            printf("Erro na análise do cenário!\n");
       }
    }

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
                CurrentScreen = 1;
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
    
    CurrentScreen = 1;

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

//FUNÇÕES IN-GAME

void JogadorMovimenta(Player Jogador, Cenario* Sala[][800], int CurrentScreen){

    Vector2 Mouse;
    int i, j;
    int m, n;
    int comando; //-->Comando para definir qual será o desenho;

    Cenario Hipotetico;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsGestureDetected(GESTURE_TAP)){
        Mouse = GetMousePosition();
        Hipotetico.PosicaoCenario.PosX = Mouse.x;
        Hipotetico.PosicaoCenario.PosY = Mouse.y;
        for (i = 0; i < 1440; i++){
            for (j = 0; j < 800; j++){
                if (Hipotetico.PosicaoCenario.PosX == Sala[i][j]->PosicaoCenario.PosX &&
                Hipotetico.PosicaoCenario.PosY == Sala[i][j]->PosicaoCenario.PosY)
                    break;
            }
        }

    if (Sala[i][j]->isDoor == 0){

            //CASO 1: Jogador vai para X >= atual e Y >= atual;
            if (Jogador.PosicaoPlayer.PosX >= Sala[i][j]->PosicaoCenario.PosX &&
            Jogador.PosicaoPlayer.PosY >= Sala[i][j]->PosicaoCenario.PosY){
                for (m = Jogador.PosicaoPlayer.PosX; m == Sala[i][j]->PosicaoCenario.PosX; m++){
                    if (Sala[m][Jogador.PosicaoPlayer.PosY]->Atravessavel == 1){
                        Jogador.PosicaoPlayer.PosX++;
                        for (n = Jogador.PosicaoPlayer.PosY; n == Sala[i][j]->PosicaoCenario.PosY; n++){
                            if (Sala[m][n]->Atravessavel == 1)
                            Jogador.PosicaoPlayer.PosY++;
                        }
                    }
                }
            }
            //CASO 2: Jogador vai para X >= atual e Y < atual;
            else if (Jogador.PosicaoPlayer.PosX >= Sala[i][j]->PosicaoCenario.PosX &&
            Jogador.PosicaoPlayer.PosY < Sala[i][j]->PosicaoCenario.PosY){
                for (m = Jogador.PosicaoPlayer.PosX; m == Sala[i][j]->PosicaoCenario.PosX; m++){
                    if (Sala[m][Jogador.PosicaoPlayer.PosY]->Atravessavel == 1){
                        Jogador.PosicaoPlayer.PosX++;
                        for (n = Jogador.PosicaoPlayer.PosY; n == Sala[i][j]->PosicaoCenario.PosY; n--){
                            if (Sala[m][n]->Atravessavel == 1)
                            Jogador.PosicaoPlayer.PosY--;
                        }
                    }
                }
            }
            //CASO 3: Jogador vai para X < atual e Y >= atual;
            else if (Jogador.PosicaoPlayer.PosX < Sala[i][j]->PosicaoCenario.PosX &&
            Jogador.PosicaoPlayer.PosY >= Sala[i][j]->PosicaoCenario.PosY){
                for (m = Jogador.PosicaoPlayer.PosX; m == Sala[i][j]->PosicaoCenario.PosX; m--){
                    if (Sala[m][Jogador.PosicaoPlayer.PosY]->Atravessavel == 1){
                        Jogador.PosicaoPlayer.PosX--;
                        for (n = Jogador.PosicaoPlayer.PosY; n == Sala[i][j]->PosicaoCenario.PosY; n++){
                            if (Sala[m][n]->Atravessavel == 1)
                            Jogador.PosicaoPlayer.PosY++;
                        }
                    }
                }
            }
            //CASO 4: Jogador vai para X < atual e Y < atual;

            else if (Jogador.PosicaoPlayer.PosX < Sala[i][j]->PosicaoCenario.PosX &&
            Jogador.PosicaoPlayer.PosY < Sala[i][j]->PosicaoCenario.PosY){
                for (int m = Jogador.PosicaoPlayer.PosX; m == Sala[i][j]->PosicaoCenario.PosX; m--){
                    if (Sala[m][Jogador.PosicaoPlayer.PosY]->Atravessavel == 1){
                        Jogador.PosicaoPlayer.PosX--;
                        for (int n = Jogador.PosicaoPlayer.PosY; n == Sala[i][j]->PosicaoCenario.PosY; n--){
                            if (Sala[m][n]->Atravessavel == 1)
                            Jogador.PosicaoPlayer.PosY--;
                        }
                    }
                }
            }
        }
        else {
            Door(&Jogador, CurrentScreen, Mouse, Sala);
        }

        if (Sala[m][n]->TipoInteracao == 1);
            //Função Sentar;
        if (Sala[m][n]->TipoInteracao == 2);
            //Função Pegar;
        if (Sala[m][n]->TipoInteracao == 3);
            //Função InteragirPersonagem;
        if (Sala[m][n]->TipoInteracao == 4);
            //Função InteragirObjetosSala;

        comando = Sala[m][n]->TipoInteracao;
        drawPlayer(&Jogador, comando);
        
        
    /*As funções de sentar e pegar basicamente alteram o Sprite do personagem enquanto 3 e 4 não, mas
    geram ações ou pequenos pop-ups, depende da sala. Se for NPC, gera interação pronta. Se não, você
    abre um mini-perfil daquele usuário e tem a opção de mandar uma mensagem (uma aba separada), adicionar 
    como amigo ou bloqueá-lo.*/

    }

}

void InterageObjetos(int CurrentScreen, Player* Jogador, Vector2 Mouse, Cenario Sala[][800], ObjInteracao Objetos[]){

    //Também uma função que depende individualmente de cada sala;

    switch(CurrentScreen){
        
        case 0:

        Music Bateria = LoadMusicStream("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Jazz.mp3");
        Texture2D PrintExemplo = LoadTexture("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/printvideo.png");

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
        (Mouse.x >= Objetos[0].aux[0][0]||Mouse.x <= Objetos[0].aux[0][2]) &&
        (Mouse.y >= Objetos[0].aux[0][1]||Mouse.y <= Objetos[0].aux[0][3]) &&
        !(IsMusicPlaying))
            PlayMusicStream(Bateria);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
        (Mouse.x >= Objetos[0].aux[0][0]||Mouse.x <= Objetos[0].aux[0][2]) &&
        (Mouse.y >= Objetos[0].aux[0][1]||Mouse.y <= Objetos[0].aux[0][3]) &&
        IsMusicPlaying == true)
            StopMusicStream(Bateria);

        BeginDrawing();
        //Draw Rectangle --> Adicionar balão;
        //DrawTexture(PrintExemplo, posições certas);
        EndDrawing();

        UnloadTexture(PrintExemplo);

    }
}

void Fachada(Cenario Sala[][800], Player* Jogador){

    //---->Fica rascunhada apenas, pois faltou terminar o preenchimento da função PreencheIndosSala;

    int caso = 1;
    CurrentScreen = 0;

    Image Bg("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Fachada Enchantee.png");
    Texture Fachada = LoadTextureFromImage(Bg);

    PreencheInfosSala(CurrentScreen, Sala, Objetos);

    while(!WindowShouldClose);{
        BeginDrawing();
        DrawTexture(Fachada, 0, 800, RAYWHITE);
        DrawPlayer(Jogador, Cenarios, CurrentScreen);
        EndDrawing();
    }

}

const char* SpeechBubble(){

    //Função deve ser inserida dentro do looping do game.
    char Texto[140];
    int QuantLetras = 0;
    bool envia;
    
    int key = GetCharPressed();

    while (key > 0 && envia == false){

                if ((key >= 32) && (key <= 125) && (QuantLetras < MAX_INPUT_CHARS)){
                    Texto[QuantLetras] = (char)key;
                    Texto[QuantLetras+1] = '\0';
                    QuantLetras++;
                }

                key = GetCharPressed(); //Pega a próxima.

                if (IsKeyPressed(KEY_BACKSPACE)){ //Deleta;
                QuantLetras--;
                if (QuantLetras < 0) QuantLetras = 0;
                Texto[QuantLetras] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER)){ //Envia;
                    envia = true;
                    return Texto;
                }
            }
}

void DrawSpeechBubble(char Texto[], Player* Jogador){

    Rectangle SBubble = { SCREEN_WIDTH/2.0f - 100, 180, 225, 50 };

    BeginDrawing();

    DrawRectangle(SBubble, );

    EndDrawing();

}

void Door(Player* Jogador, int CurrentScreen, Vector2 Mouse, Cenario Sala[][800]){

    int x = (int)Mouse.x;
    int y = (int)Mouse.y;

    CurrentScreen = Sala[x][y].DoorTrue_ProxSala;

    CarregaArqSala(CurrentScreen, &Sala);

    //LoadNovaTela;

    Jogador->PosicaoPlayer.PosX = Sala[x][y].PosicaoCenario.PosX;
    Jogador->PosicaoPlayer.PosY = Sala[x][y].PosicaoCenario.PosY;

}

// FUNÇÕES DE SERVIDOR
static struct lws_protocols protocols[] = {
	{ "http", lws_callback_http_dummy, 0, 0, 0, NULL, 0},
	LWS_PLUGIN_PROTOCOL_MINIMAL,
	LWS_PROTOCOL_LIST_TERM
};

static const lws_retry_bo_t retry = {
	.secs_since_valid_ping = 3,
	.secs_since_valid_hangup = 10,
};

static int interrupted;

static const struct lws_http_mount mount = {
	/* .mount_next */		NULL,		/* linked-list "next" */
	/* .mountpoint */		"/",		/* mountpoint URL */
	/* .origin */			"./mount-origin",  /* serve from dir */
	/* .def */			"index.html",	/* default filename */
	/* .protocol */			NULL,
	/* .cgienv */			NULL,
	/* .extra_mimetypes */		NULL,
	/* .interpret */		NULL,
	/* .cgi_timeout */		0,
	/* .cache_max_age */		0,
	/* .auth_mask */		0,
	/* .cache_reusable */		0,
	/* .cache_revalidate */		0,
	/* .cache_intermediaries */	0,
	/* .cache_no */			0,
	/* .origin_protocol */		LWSMPRO_FILE,	/* files in a dir */
	/* .mountpoint_len */		1,		/* char count */
	/* .basic_auth_login_file */	NULL,
};

#if defined(LWS_WITH_PLUGINS)
/* if plugins enabled, only protocols explicitly named in pvo bind to vhost */
static struct lws_protocol_vhost_options pvo = { NULL, NULL, "lws-minimal", "" };
#endif

void sigint_handler(int sig)
{
	interrupted = 1;
}

//FUNÇÕES CLIENTE

/*
 * This represents your object that "contains" the client connection and has
 * the client connection bound to it
 */

static struct my_conn {
	lws_sorted_usec_list_t	sul;	     /* schedule connection retry */
	struct lws		*wsi;	     /* related wsi if any */
	uint16_t		retry_count; /* count of consequetive retries */
} mco;

static struct lws_context *context;
static int interrupted, port = 443, ssl_connection = LCCSCF_USE_SSL;
static const char *server_address = "localhost",
		  *pro = "dumb-increment-protocol";

/*
 * The retry and backoff policy we want to use for our client connections
 */

static const uint32_t backoff_ms[] = { 1000, 2000, 3000, 4000, 5000 };

static const lws_retry_bo_t retry = {
	.retry_ms_table			= backoff_ms,
	.retry_ms_table_count		= LWS_ARRAY_SIZE(backoff_ms),
	.conceal_count			= LWS_ARRAY_SIZE(backoff_ms),

	.secs_since_valid_ping		= 3,  /* force PINGs after secs idle */
	.secs_since_valid_hangup	= 10, /* hangup after secs idle */

	.jitter_percent			= 20,
};

/*
 * Scheduled sul callback that starts the connection attempt
 */

static void
connect_client(lws_sorted_usec_list_t *sul)
{
	struct my_conn *m = lws_container_of(sul, struct my_conn, sul);
	struct lws_client_connect_info i;

	memset(&i, 0, sizeof(i));

	i.context = context;
	i.port = port;
	i.address = server_address;
	i.path = "/";
	i.host = i.address;
	i.origin = i.address;
	i.ssl_connection = ssl_connection;
	i.protocol = pro;
	i.local_protocol_name = "lws-minimal-client";
	i.pwsi = &m->wsi;
	i.retry_and_idle_policy = &retry;
	i.userdata = m;

	if (!lws_client_connect_via_info(&i))
		/*
		 * Failed... schedule a retry... we can't use the _retry_wsi()
		 * convenience wrapper api here because no valid wsi at this
		 * point.
		 */
		if (lws_retry_sul_schedule(context, 0, sul, &retry,
					   connect_client, &m->retry_count)) {
			lwsl_err("%s: connection attempts exhausted\n", __func__);
			interrupted = 1;
		}
}

static int
callback_minimal(struct lws *wsi, enum lws_callback_reasons reason,
		 void *user, void *in, size_t len)
{
	struct my_conn *m = (struct my_conn *)user;

	switch (reason) {

	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		lwsl_err("CLIENT_CONNECTION_ERROR: %s\n",
			 in ? (char *)in : "(null)");
		goto do_retry;
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		lwsl_hexdump_notice(in, len);
		break;

	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		lwsl_user("%s: established\n", __func__);
		break;

	case LWS_CALLBACK_CLIENT_CLOSED:
		goto do_retry;

	default:
		break;
	}

	return lws_callback_http_dummy(wsi, reason, user, in, len);

do_retry:
	/*
	 * retry the connection to keep it nailed up
	 *
	 * For this example, we try to conceal any problem for one set of
	 * backoff retries and then exit the app.
	 *
	 * If you set retry.conceal_count to be larger than the number of
	 * elements in the backoff table, it will never give up and keep
	 * retrying at the last backoff delay plus the random jitter amount.
	 */
	if (lws_retry_sul_schedule_retry_wsi(wsi, &m->sul, connect_client,
					     &m->retry_count)) {
		lwsl_err("%s: connection attempts exhausted\n", __func__);
		interrupted = 1;
	}

	return 0;
}

static const struct lws_protocols protocols[] = {
	{ "lws-minimal-client", callback_minimal, 0, 0, 0, NULL, 0 },
	LWS_PROTOCOL_LIST_TERM
};

static void
sigint_handler(int sig)
{
	interrupted = 1;
}



//Função principal --> Une todas as funções elaboradas e implementadas anteriormente.

int main (int argc, const char **argv){

    //SERVER
    {
	struct lws_context_creation_info info;
	struct lws_context *context;
	const char *p;
	int n = 0, logs = LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE
			/* for LLL_ verbosity above NOTICE to be built into lws,
			 * lws must have been configured and built with
			 * -DCMAKE_BUILD_TYPE=DEBUG instead of =RELEASE */
			/* | LLL_INFO */ /* | LLL_PARSER */ /* | LLL_HEADER */
			/* | LLL_EXT */ /* | LLL_CLIENT */ /* | LLL_LATENCY */
			/* | LLL_DEBUG */;

	signal(SIGINT, sigint_handler);

	if ((p = lws_cmdline_option(argc, argv, "-d")))
		logs = atoi(p);

	lws_set_log_level(logs, NULL);
	lwsl_user("LWS minimal ws server | visit http://localhost:7681 (-s = use TLS / https)\n");

	memset(&info, 0, sizeof info); /* otherwise uninitialized garbage */
	info.port = 7681;
	info.mounts = &mount;
	info.protocols = protocols;
	info.vhost_name = "localhost";
#if defined(LWS_WITH_PLUGINS)
	info.pvo = &pvo;
#endif
	info.options =
		LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

#if defined(LWS_WITH_TLS)
	if (lws_cmdline_option(argc, argv, "-s")) {
		lwsl_user("Server using TLS\n");
		info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
		info.ssl_cert_filepath = "localhost-100y.cert";
		info.ssl_private_key_filepath = "localhost-100y.key";
	}
#endif

	if (lws_cmdline_option(argc, argv, "-h"))
		info.options |= LWS_SERVER_OPTION_VHOST_UPG_STRICT_HOST_CHECK;

	if (lws_cmdline_option(argc, argv, "-v"))
		info.retry_and_idle_policy = &retry;

	context = lws_create_context(&info);
	if (!context) {
		lwsl_err("lws init failed\n");
		return 1;
	}

	while (n >= 0 && !interrupted)
		n = lws_service(context, 0);

	lws_context_destroy(context);

    //CLIENTE

    struct lws_context_creation_info info;
	const char *p;
	int n = 0;

	signal(SIGINT, sigint_handler);
	memset(&info, 0, sizeof info);
	lws_cmdline_option_handle_builtin(argc, argv, &info);

	lwsl_user("LWS minimal ws client\n");

	info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
	info.port = CONTEXT_PORT_NO_LISTEN; /* we do not run any server */
	info.protocols = protocols;

#if defined(LWS_WITH_MBEDTLS) || defined(USE_WOLFSSL)
	/*
	 * OpenSSL uses the system trust store.  mbedTLS has to be told which
	 * CA to trust explicitly.
	 */
	info.client_ssl_ca_filepath = "./libwebsockets.org.cer";
#endif

	if ((p = lws_cmdline_option(argc, argv, "--protocol")))
		pro = p;

	if ((p = lws_cmdline_option(argc, argv, "-s")))
		server_address = p;

	if ((p = lws_cmdline_option(argc, argv, "-p")))
		port = atoi(p);

	if (lws_cmdline_option(argc, argv, "-n"))
		ssl_connection &= ~LCCSCF_USE_SSL;

	if (lws_cmdline_option(argc, argv, "-j"))
		ssl_connection |= LCCSCF_ALLOW_SELFSIGNED;

	if (lws_cmdline_option(argc, argv, "-k"))
		ssl_connection |= LCCSCF_ALLOW_INSECURE;

	if (lws_cmdline_option(argc, argv, "-m"))
		ssl_connection |= LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;

	if (lws_cmdline_option(argc, argv, "-e"))
		ssl_connection |= LCCSCF_ALLOW_EXPIRED;

	info.fd_limit_per_thread = 1 + 1 + 1;

	context = lws_create_context(&info);
	if (!context) {
		lwsl_err("lws init failed\n");
		return 1;
	}

	/* schedule the first client connection attempt to happen immediately */
	lws_sul_schedule(context, 0, &mco.sul, connect_client, 1);

	while (n >= 0 && !interrupted)
		n = lws_service(context, 0);

	lws_context_destroy(context);
	lwsl_user("Completed\n");

    //JOGO
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