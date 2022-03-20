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

//Organizar struct de objetos para intera��o;

typedef struct interacao{
    int aux[4][5];
    //-->Assumem-se, NO M�X, 5 objetos intera��o/sala, cada um exigindo 4 vari�veis auxiliares (minx, miny, maxx, maxy);
    char Info[30];
} ObjInteracao; //Ser� um vetor, por sala;

typedef struct linguas{

    //Aqui o jogador pode definir se ele � proficiente em outras l�nguas, de b�s-int-av� definindo um valor de 0-2.
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

    //Personaliz�veis: nome, idade, apar�ncia, bio...
    char Nome[20];
    int Idade;
    Linguas Linguagens;
    char Bio[300];
    Posicao PosicaoPlayer;

    //Caracter�sticas inerentes a ele
    int Atravessavel;
    Amigos ListaAmigos;
    int Nivel; //Gamifica��o!
    int AlunoAF;

} Player; //Informa��es provavelmente ser�o carregadas por meio de um arquivo texto durante o login.

typedef struct posicao_player{

    int PosX;
    int PosY;

} Posicao;

typedef struct local{
    /*---> O cen�rio que aparecer� na tela dever� ser gerado baseado em um arquivo.txt espec�fico de cada regi�o.
    Entretanto, � preciso ler a matriz do arquivo.txt de sala (um mapa de caracteres) e analisar
    caracter por caracter para conseguir limitar e analisar as intera��es dos personagens com a
    sala. Isso ser� implementado por meio de uma fun��o de an�lise de matriz. 
    
    Logo, essas infos ser�o preenchidas pela fun��o de an�lise da sala baseado nas no arquivo.txt*/
    Posicao PosicaoCenario;
    int Atravessavel;
    int TipoInteracao;
    //---> Intera��es de 0-4, sendo respectivamente: nenhuma, sentar, pegar, conversar, interagir (obj. inanimado);
    //OBS: Resumo l�gico: 0 - Atravessa; 1- Atravessa, mas visor da tela � diferente; 2- N�o atravessa; 3 - N�o atravessa; 
    //     4 - N�o atravessa;
    int isDoor;
    int DoorTrue_ProxSala;
    //--->Se for uma "porta", � para ocorrer uma fun��o espec�fica para a troca de cen�rio.
    //--->O valor DoorTrue_ProxSala ser� -1 (n�o � porta) ou 0-4 com o valor da pr�xima CurrentScreen;

} Cenario;

//FUN��ES PR�-JOGO

void CadastroLinguas(Player* Jogador){

    int proficiente;

    printf("Qual a sua profici�ncia em...\n");

    //Fazer l�gica de select para ficar mais otimizado.

    printf("\tPT-BR?\n");
    printf("|------------------------|");
    printf("|0 - B�sico              |\n");
    printf("|------------------------|");
    printf("|1 - Intermedi�rio       |\n");
    printf("|------------------------|");
    printf("|2 - Avan�ado ou Fluente |\n");
    printf("|------------------------|");
    
    scanf("%d", &proficiente);
    Jogador->Linguagens.ProficienciaPT = proficiente;

    printf("\tFR?\n");
    printf("|------------------------|");
    printf("|0 - B�sico              |\n");
    printf("|------------------------|");
    printf("|1 - Intermedi�rio       |\n");
    printf("|------------------------|");
    printf("|2 - Avan�ado ou Fluente |\n");
    printf("|------------------------|");
    
    scanf("%d", &proficiente);
    Jogador->Linguagens.ProficienciaFR = proficiente;

    printf("\tEN?\n");
    printf("|------------------------|");
    printf("|0 - B�sico              |\n");
    printf("|------------------------|");
    printf("|1 - Intermedi�rio       |\n");
    printf("|------------------------|");
    printf("|2 - Avan�ado ou Fluente |\n");
    printf("|------------------------|");
    
    scanf("%d", &proficiente);
    Jogador->Linguagens.ProficienciaEN = proficiente;

}

int PersonalizaPlayer(Player* Jogador){

    FILE *Arq;

    if (Arq == NULL){

        Arq = fopen("player.c", "w");

        printf("Escolha um nome de exibi��o.\n");
        getchar();
	    fgets(Jogador->Nome, 20, stdin);

        printf("Idade\n");
        scanf("%d", &Jogador->Idade);

        printf("L�nguas estrangeiras\n");
        CadastroLinguas(Jogador);

        printf("Conte um pouco sobre voc�! Insira uma bio.\n");
        //Opcional;
        getchar();
	    fgets(Jogador->Bio, 300, stdin);

        Jogador->ListaAmigos.primeiro = InicializaLAmigos();
        Jogador->Nivel = 0;
        Jogador->Atravessavel = 0;
        Jogador->PosicaoPlayer.PosX = SCREEN_WIDTH/2;
        Jogador->PosicaoPlayer.PosY = SCREEN_HEIGHT/2;

        //Se for aluno da AF, isso dever� ser puxado com base no arquivo.txt do cadastro.

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
        //Adiciona funcionalidades na tela de personaliza��o implementadas com as informa��es.
        //Copia tudo para o arquivo j� existente, sobre-escrevendo o antigo.
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
                printf("Sua senha � forte!\n");
                break;
            }
        }

            if (u == 0 || d == 0 || s == 0)
                printf("Sua senha � fraca, tente novamente!");
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
        //Adquirir info do usu�rio armazenada em um arquivo.txt para preencher o Player;
    else{
        printf("Senha incorreta!\n");
    }
    
}

//------> Fun��es relacionadas � LDE da Lista de Amigos;

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
    
    //Sempre inserir no come�o para dar prioridade aos novos amigos.

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
        printf("Nenhum amigo ainda! Adicione alguns!\n");
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

Amigos* RemoveLista(Amigos* ListaAmigos, ptAmigos Infos){

    ptAmigos PtAux;
    ptAmigos PtProx;
    int elemento;

    PtProx = PtAux->prox;

    //Quando a pessoa clica em remover uma determinada pessoa de sua lista de amigos.

    printf("Deseja mesmo remover esta pessoa da sua lista de amigos?\n");
    printf("1 - Sim || 0 - N�o");
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

    //CADA SALA/TELA DEVER� SER ANALISADA INDIVIDUALMENTE!

       switch(CurrentScreen){

        case (0):

        int aux[4]={0,0,3000,3000};

        for (int m = 0; m < 1440; m++){
            for (int n = 0; n < 800; n++){
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
                //Nada relevante para a movimenta��o;
                else {
                    Sala[m][n]->Atravessavel = 1;
                    Sala[m][n]->isDoor = 0;
                    Sala[m][n]->TipoInteracao = 0;
                }
                }
            }
            //Preenche as vari�veis auxiliares para uso posterior;
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
            printf("Erro na an�lise do cen�rio!\n");
       }
    }


void LoadPrincipal(Cenario Sala[][800]){



    BeginDrawing();
    EndDrawing();

}

//FUN��ES IN-GAME

void JogadorMovimenta(Player Jogador, Cenario* Sala[][800], int CurrentScreen){

    Vector2 Mouse;
    int i, j;
    int m, n;

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
            //Fun��o Sentar;
        if (Sala[m][n]->TipoInteracao == 2);
            //Fun��o Pegar;
        if (Sala[m][n]->TipoInteracao == 3);
            //Fun��o InteragirPersonagem;
        if (Sala[m][n]->TipoInteracao == 4);
            //Fun��o InteragirObjetosSala;
        
    /*As fun��es de sentar e pegar basicamente alteram o Sprite do personagem enquanto 3 e 4 n�o, mas
    geram a��es ou pequenos pop-ups, depende da sala. Se for NPC, gera intera��o pronta. Se n�o, voc�
    abre um mini-perfil daquele usu�rio e tem a op��o de mandar uma mensagem (uma aba separada), adicionar 
    como amigo ou bloque�-lo.*/

    }

}

void InterageObjetos(int CurrentScreen, Player* Jogador, Vector2 Mouse, Cenario Sala[][800], ObjInteracao Objetos[]){

    //Tamb�m uma fun��o que depende individualmente de cada sala;

    switch(CurrentScreen){
        
        case 0:

        Music Bateria = LoadMusicStream("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/Jazz.mp3");
        Rectangle R1;
        Texture2D PrintExemplo = LoadTexture("C:/Users/laums/Documents/GitHub/hackathon-enchante/Identidade Visual/printvideo.png");
        Image PExemplo;
        PExemplo = LoadImageFromTexture(PrintExemplo);


        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && 
        (Mouse.x >= Objetos[0].aux[0][0]||Mouse.x <= Objetos[0].aux[0][2]) &&
        (Mouse.y >= Objetos[0].aux[0][1]||Mouse.y <= Objetos[0].aux[0][3]))
            PlayMusicStream(Bateria);


        //No caso, aqui � s� a bateria.
        BeginDrawing();
        //Tocar uma m�sica + abrir uma janelinha falando um pouquinho a respeito!
        EndDrawing();

    }
}

const char* SpeechBubble(){

    //Fun��o deve ser inserida dentro do looping do game.
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

                key = GetCharPressed(); //Pega a pr�xima.

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
    //Calcular os valores corretos com base na identidade visual da tela;

    BeginDrawing();

    //Fazer identidade visual;

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



//Fun��o principal

int main (){

    int TodasasSalas[5] = {0,1,2,3,4};
    //Inclui Fachada, Principal, Estudos (onde ficam as Redes Sociais + Biblioteca), Palco e Cinema.
    ObjInteracao Objetos[5];
    int CurrentScreen = -1;
    //� o que definir� qual tela dever� aparecer --> Come�a no menu principal;
    Player Jogador;
    Cenario Sala[1440][800];

    setlocale(LC_ALL, "Portuguese");

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hackathon Enchante{�} - Demo do Time 3");
    
    //ACIMA: Toda a l�gica do jogo e arquivos.
    //ABAIXO: Tudo o que aparecer� na tela/UX Design estruturado para a biblioteca. 

    BeginDrawing();
    EndDrawing();

    return 0;
}