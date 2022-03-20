FILE* arq;
    
    //Fazer um swtich case para os tipos de sala que vão puxar um determinado arquivo no mapa.
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
                for(int i = 0; i < SCREEN_WIDTH/COLUNAS; i++)
                {
                    for(int j = 0; j < SCREEN_HEIGHT/LINHAS; j++)
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
                for(int i = 0; i < SCREEN_WIDTH/COLUNAS; i++)
                {
                    for(int j = 0; j < SCREEN_HEIGHT/LINHAS; j++)
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
                for(int i = 0; i < SCREEN_WIDTH/COLUNAS; i++)
                {
                    for(int j = 0; j < SCREEN_HEIGHT/LINHAS; j++)
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
                for(int i = 0; i < SCREEN_WIDTH/COLUNAS; i++)
                {
                    for(int j = 0; j < SCREEN_HEIGHT/LINHAS; j++)
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