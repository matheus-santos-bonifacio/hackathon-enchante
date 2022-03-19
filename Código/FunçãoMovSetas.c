
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
            Door(&Jogador, CurrentScreen, Prox_Mov_X, Prox_Mov_Y, Sala);
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
            Door(&Jogador, CurrentScreen, Prox_Mov_X, Prox_Mov_Y, Sala);
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
            Door(&Jogador, CurrentScreen, Prox_Mov_X, Prox_Mov_Y, Sala);
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
            Door(&Jogador, CurrentScreen, Prox_Mov_X, Prox_Mov_Y, Sala);
    }