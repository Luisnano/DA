bool funcion_factibilidad(int row, int col, List<Object*> obstacles, List<Defense*> defenses,
bool **freeCells,float mapHeight, float mapWidth,int nCellsWidth, int nCellsHeight, const Defense& d){
    //definicion de variables
    float cellWidth = mapWidth / nCellsWidth; //anchura de la celula
    float cellHeight = mapHeight / nCellsHeight;//altura de la celula
    Vector3 p = cellCenterToPosition(row,col,cellWidth,cellHeight); //Creamos la posicion con los datos dados
    bool token = true;

    //Primeiro comprobamos que la celda no esta en ninguna posicion limite
    if(p.x + d.radio > mapWidth ||p.x - d.radio < 0 ||
        p.y + d.radio > mapHeight ||p.y - d.radio < 0)
    {
        return false; //si se cumple alguna condicion la defensa no cabe al alcanzar posiciones limite del mapa
    }    
    else{//Si no cumple el primer requisito no sera necesario continuar, en cambio , si lo cumple entonces:
        //Comprobaremos que no colisionan con las defensas/obstaculos que ya estan colocadas
        for(auto i : obstacles){
            //Colisionara en caso de que las distancias entre puntos centrales de los obstaculos
            //sea menor que los radios de la defensa a colocar y el obstaculo
            if((d.radio + i->radio)>_distance(p,i->position))
                token = false;
        }
        if(token){//si ya se ha detectado que no es posible colocarla en un obstaculo
                  //para que comprobar las defensas
            //Se comprobara de forma similar a los obstaculos con las defensas
            for (auto i: defenses){
                if((d.radio + i->radio)>_distance(p,i->position))
                    token = false;
            }
        }
    }
    return token;      
}