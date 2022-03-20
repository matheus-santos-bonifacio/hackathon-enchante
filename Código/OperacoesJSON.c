#include "./lib/cJSON.h"
#include "./lib/cJSON.c"
#include "./Codigo-Hacka.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Tenho que eliminar a importacao do Jogador


char *insere_usuario(Player jogador, char *array_json)
{
    cJSON *objeto = cJSON_CreateObject();
    cJSON_AddItemToObject(objeto, "nome", cJSON_CreateString(jogador.Nome));
    cJSON_AddItemToObject(objeto, "idade", cJSON_CreateNumber(jogador.Idade));
    cJSON_AddItemToObject(objeto, "bio", cJSON_CreateString(jogador.Bio));
    cJSON_AddItemToObject(objeto, "atravessa_vel", cJSON_CreateNumber(jogador.Atravessavel));
    cJSON_AddItemToObject(objeto, "nivel", cJSON_CreateNumber(jogador.Nivel));
    cJSON_AddItemToObject(objeto, "aluno_af", cJSON_CreateNumber(jogador.AlunoAF));

    cJSON *array = cJSON_Parse(array_json);
    cJSON_AddItemToArray(array, objeto);
    
    char *string = cJSON_Print(array);

    cJSON_Delete(array);

    return string;
}

Player mostra_usuarios(const char * const jogadores_json)
{
    cJSON *jogadores_cjson_objeto = cJSON_Parse(jogadores_json);
    cJSON *jogador_cjson_objeto = NULL;
    Player jogador;

    if (jogadores_cjson_objeto == NULL)
    {
        printf("algo aconteceu de errado");
    }

    cJSON_ArrayForEach(jogador_cjson_objeto, jogadores_cjson_objeto)
    {
        Player jogador;

        memcpy(jogador.Nome, cJSON_GetObjectItemCaseSensitive(jogador_cjson_objeto, "nome")->valuestring, 20);
        memcpy(jogador.Bio, cJSON_GetObjectItemCaseSensitive(jogador_cjson_objeto, "bio")->valuestring, 20);

        jogador.Idade = cJSON_GetObjectItemCaseSensitive(jogador_cjson_objeto, "idade")->valueint;
        jogador.Atravessavel = cJSON_GetObjectItemCaseSensitive(jogador_cjson_objeto, "atravessa_vel")->valueint;
        jogador.Idade = cJSON_GetObjectItemCaseSensitive(jogador_cjson_objeto, "idade")->valueint;
        jogador.Idade = cJSON_GetObjectItemCaseSensitive(jogador_cjson_objeto, "idade")->valueint;
    }


    cJSON_Delete(jogadores_cjson_objeto);

    return jogador;
}

