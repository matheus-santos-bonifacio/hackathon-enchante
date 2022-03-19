#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <raylib.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

typedef struct linguas{

    //Aqui o jogador pode definir se ele prefere que o jogo apareça em FR ou em PT.    
    int Preferencia_PTouFR;

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
    int Nivel;
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
    char Simbolo;
    int Atravessavel;
    int TipoInteracao;
    //---> Interações de 0-4, sendo respectivamente: nenhuma, sentar, pegar, conversar, interagir (obj. inanimado);
    //OBS: 0 - Atravessa; 1- Atravessa, mas visor da tela é diferente; 2- Não atravessa; 3 - Não atravessa; 4 - Não atravessa;
    int isDoor;
    //--->Se for uma "porta" em um dos quatro cantos da tela para outra sala, há uma interação especial.

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

Amigos* AcionarAmigos(Amigos* ListaAmigos, ptAmigos Infos, Player NOT_Jogador){

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




void CarregaArqSala(int SALA, Cenario* Sala[][800]){

    FILE* arq;
    
    //Fazer um swtich case para os tipos de sala que vão puxar um determinado arquivo no mapa.
    switch (SALA){
        case 0:   
        if (!(arq = fopen("Inicio.txt", "r")))
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
        printf("Não foi possível carregar o jogo!\n");
        }
    
    //Fazer função de impressão visual da Sala.

}

//FUNÇÕES IN-GAME

void JogadorMovimenta(Player Jogador, Cenario* Sala[][800]){

    int Prox_Mov_X, Prox_Mov_Y;
    bool movimento;

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){

        Prox_Mov_X = Jogador.PosicaoPlayer.PosX;
        Prox_Mov_Y = Jogador.PosicaoPlayer.PosY + 1;

        //Análises das propriedades de Sala[Prox_Mov_X][Prox_Mov_Y]

        //Caso 1: "Vazio na frente"
        if (Sala[Prox_Mov_X][Prox_Mov_Y]->Atravessavel == 1 && Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 0)
            movimento = true;
        //Caso 2: "Sentar"
        else if (Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 1)
            movimento = true;
        //Caso 3: Pegar, Interagir ou Parede/Canto da tela;
        else if (Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 2 || Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 3 || Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 4 )
            movimento = false;
        
        if (movimento == true)
            Jogador.PosicaoPlayer.PosY++;
        
        //Caso 4: O jogador está no canto da tela em uma porta para uma nova Sala.

        if (Sala[Prox_Mov_X][Prox_Mov_Y]->isDoor == 1);
            //Função Door;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){

        Prox_Mov_X = Jogador.PosicaoPlayer.PosX;
        Prox_Mov_Y = Jogador.PosicaoPlayer.PosY - 1;

        //Análises das propriedades de Sala[Prox_Mov_X][Prox_Mov_Y]

        //Caso 1: "Vazio na frente"
        if (Sala[Prox_Mov_X][Prox_Mov_Y]->Atravessavel == 1 && Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 0)
            movimento = true;
        //Caso 2: "Sentar"
        else if (Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 1)
            movimento = true;
        //Caso 3: Pegar, Interagir ou Parede/Canto da tela;
        else if (Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 2 || Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 3 || Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 4 )
            movimento = false;
        
        if (movimento == true)
            Jogador.PosicaoPlayer.PosY--;
        
        //Caso 4: O jogador está no canto da tela em uma porta para uma nova Sala.

        if (Sala[Prox_Mov_X][Prox_Mov_Y]->isDoor == 1);
            //Função Door;
    }

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_S)){

        Prox_Mov_X = Jogador.PosicaoPlayer.PosX + 1;
        Prox_Mov_Y = Jogador.PosicaoPlayer.PosY;

        //Análises das propriedades de Sala[Prox_Mov_X][Prox_Mov_Y]

        //Caso 1: "Vazio na frente"
        if (Sala[Prox_Mov_X][Prox_Mov_Y]->Atravessavel == 1 && Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 0)
            movimento = true;
        //Caso 2: "Sentar"
        else if (Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 1)
            movimento = true;
        //Caso 3: Pegar, Interagir ou Parede/Canto da tela;
        else if (Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 2 || Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 3 || Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 4 )
            movimento = false;
        
        if (movimento == true)
            Jogador.PosicaoPlayer.PosX++;
        
        //Caso 4: O jogador está no canto da tela em uma porta para uma nova Sala.

        if (Sala[Prox_Mov_X][Prox_Mov_Y]->isDoor == 1);
            //Função Door;
    }

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){

        Prox_Mov_X = Jogador.PosicaoPlayer.PosX - 1;
        Prox_Mov_Y = Jogador.PosicaoPlayer.PosY;

        //Análises das propriedades de Sala[Prox_Mov_X][Prox_Mov_Y]

        //Caso 1: "Vazio na frente"
        if (Sala[Prox_Mov_X][Prox_Mov_Y]->Atravessavel == 1 && Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 0)
            movimento = true;
        //Caso 2: "Sentar"
        else if (Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 1)
            movimento = true;
        //Caso 3: Pegar, Interagir ou Parede/Canto da tela;
        else if (Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 2 || Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 3 || Sala[Prox_Mov_X][Prox_Mov_Y]->TipoInteracao == 4 )
            movimento = false;
        
        if (movimento == true)
            Jogador.PosicaoPlayer.PosX - 1;
        
        //Caso 4: O jogador está no canto da tela em uma porta para uma nova Sala.

        if (Sala[Prox_Mov_X][Prox_Mov_Y]->isDoor == 1);
            //Função Door;
    }

    //Implementar a versão com sprites quando ficarem prontas!

}

int main (){

    Player Jogador;
    Cenario Sala[1000][800]; //-->Por ora, tem que analisar a divisão correta da tela 
    //                          (aka se vai ter uma sessão da tela só para chat e etc.);

    setlocale(LC_ALL, "Portuguese");

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hackathon Enchante{é} - Demo do Time 3");
    
    //ACIMA: Toda a lógica do jogo e arquivos.
    //ABAIXO: Tudo o que aparecerá na tela/UX Design estruturado para a biblioteca. 

    BeginDrawing();
    EndDrawing();

    return 0;
}