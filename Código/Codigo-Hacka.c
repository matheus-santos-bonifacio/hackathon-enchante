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
    char Simbolo;
    int Atravessavel;
    int TipoInteracao;
    //---> Intera��es de 0-4, sendo respectivamente: nenhuma, sentar, pegar, conversar, interagir (obj. inanimado);
    //OBS: 0 - Atravessa; 1- Atravessa, mas visor da tela � diferente; 2- N�o atravessa; 3 - N�o atravessa; 4 - N�o atravessa;
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

Amigos* AcionarAmigos(Amigos* ListaAmigos, ptAmigos Infos, Player NOT_Jogador){

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


void CarregaArqSala(int CurrentScreen, Cenario* Sala[][800]){

    FILE* arq;
    
    //Fazer um swtich case para os tipos de sala que v�o puxar um determinado arquivo no mapa.
    switch (CurrentScreen){
        case 0:   
        if (!(arq = fopen("Fachada.txt", "r")))
            printf("Erro ao carregar as salas!\n");
        else
        {
            while(!feof(arq))
            {
                for(int i = 0; i < SCREEN_WIDTH; i++)
                {
                    for(int j = 0; j < SCREEN_HEIGHT; j++)
                    {
                        fscanf(arq, "%c", &Sala[i][j]->Simbolo);
                    }
                }
            }
        fclose(arq);
        //CarregaSala;

        }
        break;

        case 1:
        if (!(arq = fopen("Principal.txt", "r")))
            printf("Erro ao carregar as salas!\n");
        else
        {
            while(!feof(arq))
            {
                for(int i = 0; i < SCREEN_WIDTH; i++)
                {
                    for(int j = 0; j < SCREEN_HEIGHT; j++)
                    {
                        fscanf(arq, "%c", &Sala[i][j]->Simbolo);
                    }
                }
            }
        fclose(arq);
        }
        break;

        case 2:
        if (!(arq = fopen("Biblioteca.txt", "r")))
            printf("Erro ao carregar as salas!\n");
        else
        {
            while(!feof(arq))
            {
                for(int i = 0; i < SCREEN_WIDTH; i++)
                {
                    for(int j = 0; j < SCREEN_HEIGHT; j++)
                    {
                        fscanf(arq, "%c", &Sala[i][j]->Simbolo);
                    }
                }
            }
        fclose(arq);
        }
        break;

        if (!(arq = fopen("Palco.txt", "r")))
            printf("Erro ao carregar as salas!\n");
        else
        {
            while(!feof(arq))
            {
                for(int i = 0; i < SCREEN_WIDTH; i++)
                {
                    for(int j = 0; j < SCREEN_HEIGHT; j++)
                    {
                        fscanf(arq, "%c", &Sala[i][j]->Simbolo);
                    }
                }
            }
        fclose(arq);
        }
        break;

        if (!(arq = fopen("Cinema.txt", "r")))
            printf("Erro ao carregar as salas!\n");
        else
        {
            while(!feof(arq))
            {
                for(int i = 0; i < SCREEN_WIDTH; i++)
                {
                    for(int j = 0; j < SCREEN_HEIGHT; j++)
                    {
                        fscanf(arq, "%c", &Sala[i][j]->Simbolo);
                    }
                }
            }
        fclose(arq);
        }
        break;

        default:
        printf("N�o foi poss�vel carregar o jogo!\n");
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
                for (int m = Jogador.PosicaoPlayer.PosX; m == Sala[i][j]->PosicaoCenario.PosX; m++){
                    if (Sala[m][Jogador.PosicaoPlayer.PosY]->Atravessavel == 1){
                        Jogador.PosicaoPlayer.PosX++;
                        for (int n = Jogador.PosicaoPlayer.PosY; n == Sala[i][j]->PosicaoCenario.PosY; n++){
                            if (Sala[m][n]->Atravessavel == 1)
                            Jogador.PosicaoPlayer.PosY++;
                        }
                    }
                }
            }
            //CASO 2: Jogador vai para X >= atual e Y < atual;
            else if (Jogador.PosicaoPlayer.PosX >= Sala[i][j]->PosicaoCenario.PosX &&
            Jogador.PosicaoPlayer.PosY < Sala[i][j]->PosicaoCenario.PosY){
                for (int m = Jogador.PosicaoPlayer.PosX; m == Sala[i][j]->PosicaoCenario.PosX; m++){
                    if (Sala[m][Jogador.PosicaoPlayer.PosY]->Atravessavel == 1){
                        Jogador.PosicaoPlayer.PosX++;
                        for (int n = Jogador.PosicaoPlayer.PosY; n == Sala[i][j]->PosicaoCenario.PosY; n--){
                            if (Sala[m][n]->Atravessavel == 1)
                            Jogador.PosicaoPlayer.PosY--;
                        }
                    }
                }
            }
            //CASO 3: Jogador vai para X < atual e Y >= atual;
            else if (Jogador.PosicaoPlayer.PosX < Sala[i][j]->PosicaoCenario.PosX &&
            Jogador.PosicaoPlayer.PosY >= Sala[i][j]->PosicaoCenario.PosY){
                for (int m = Jogador.PosicaoPlayer.PosX; m == Sala[i][j]->PosicaoCenario.PosX; m--){
                    if (Sala[m][Jogador.PosicaoPlayer.PosY]->Atravessavel == 1){
                        Jogador.PosicaoPlayer.PosX--;
                        for (int n = Jogador.PosicaoPlayer.PosY; n == Sala[i][j]->PosicaoCenario.PosY; n++){
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
        
    //IMPLEMENTAR A SPRITE DO PERSONAGEM!

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



//Fun��o Main

int main (){

    int TodasasSalas[5] = {0,1,2,3,4};
    //Inclui Fachada, Principal, Estudos (onde ficam as Redes Sociais + Biblioteca), Palco e Cinema.

    int CurrentScreen = 0;
    //� o que definir� qual tela dever� aparecer.
    Player Jogador;
    Cenario Sala[1000][800]; //-->Por ora, tem que analisar a divis�o correta da tela 
    //                          (aka se vai ter uma sess�o da tela s� para chat e etc.);

    setlocale(LC_ALL, "Portuguese");

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hackathon Enchante{�} - Demo do Time 3");
    
    //ACIMA: Toda a l�gica do jogo e arquivos.
    //ABAIXO: Tudo o que aparecer� na tela/UX Design estruturado para a biblioteca. 

    BeginDrawing();
    EndDrawing();

    return 0;
}