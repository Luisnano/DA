// ###### Config options ################

//#define PRINT_DEFENSE_STRATEGY 1    // generate map images

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

#ifdef PRINT_DEFENSE_STRATEGY
#include "ppm.h"
#endif

#ifdef CUSTOM_RAND_GENERATOR
RAND_TYPE SimpleRandomGenerator::a;
#endif

using namespace Asedio;
Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){ return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0); }

float cellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses, List<Map*> map) {
    float value = 0;

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    //Vamos a revisar las celdas de alrededor para ver que hay y si es una buena celda
    //Los criterios a seguir seran:
    //  -Cuanto mas cerca este de una defensa mejor
    //  -Cuanto mas lejos este de un obstacúlo mejor
    //Antes que nada debemos determinar la celda, eso lo haremos con la funcion que se nos proporciona en preguntas frecuentes.
    
    Vector3 cellInPosition = cellCenterToPosition(row, col, cellWidth, cellHeight);    

    //El primer factor de valor, que esté lejos de un obstaculo
    List<Object*>::iterator currentObstacle = obstacles.begin();
    while (currentObstacle != obstacles.end()){
        //Como es mejor cuanto mas lejos este de un obstaculo mejor, esta distancia se sumara al value
        value += (*currentObstacle)->position.subtract(cellInPosition).length();
        ++currentObstacle; 
    }

    //El segundo factor de valor
    //Usamos el cellCenterTo position para buscar la celda mas cercana al centro,
    //  con esto ya podemos comparar la posicion de la celda con la del centro
    //  Cuanto mas cerca este, mejor. Entonces invertimos la distancia y ese es el puntuaje.
    int mapCenterAuxW = mapWidth / 2;
    int mapCenterAuxH = mapHeight / 2;
    Vector3 mapCenter = cellCenterToPosition (mapCenterAuxW, mapCenterAuxH, mapWidth, mapHeight);
    value += 1/(mapCenter.subtract(cellInPosition).length());
    //Hasta aqui seria la implemetacion del primer ejercicio, pero como no hay que implementarla
    //  usare esta para añadirle las variables que hace que esta funcion sea para darle valores 
    //  a las celdas de defensa.
    //El primer criterio para la colocacion de una defensa sera que haya mas defensas cerca
    List<Defense*> :: iterator currentDefense = defenses.begin();
    while (currentDefense != defenses.end()){
        value += 1/((*currentDefense)->position.subtract(cellInPosition).length());
        ++currentDefense;
    }
	return value; // implemente aqui la funci�n que asigna valores a las celdas
}

bool factibilidad(int row, int col, Defense* defensa, List<Object*> obstacles, List<Defense*> defenses, 
                  bool **freeCells,float mapHeight, float mapWidth,int nCellsWidth,
                  int nCellsHeight) {
    //COMENZAMOS CON LA FUNCION DE FACTIBILIDAD
    //De variables necesitamos lo de siempre, las celdas en forma normal y la celda actual

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    Vector3 cellInPosition = cellCenterToPosition(row, col, cellWidth, cellHeight);
    std::cout<<"STOP FUERA DE CONDICIONALES"<<std::endl;

    bool esValido = true;

    //Primero: ¿Está dentro de los limites del mapa?
    if (cellInPosition.x > mapWidth || cellInPosition.y > mapHeight){
        std::cout<<"STOP 1"<<std::endl;
        esValido = false;
    }//if

    //El radio de el objeto no se encuentra fuera del mapa
    if (cellInPosition.x + defensa->radio >mapWidth && cellInPosition.y + defensa->radio > mapHeight){
        std::cout<<"STOP 2"<<std::endl;
        esValido = false;
    } 

    //Ya de paso vamos a mirar que no estamos intentando colocarlo en una posicion negativa
    if (cellInPosition.x + defensa->radio < 0 && cellInPosition.y + defensa->radio < 0){
        std::cout<<"STOP 3"<<std::endl;
        esValido = false;
    } 


    //2: Esta ocupada la celda por:
    //2.1   -Una defensa?
    List<Defense*> ::iterator currentDefense = defenses.begin();
    while (currentDefense != defenses.end())
    {
        std::cout<<"STOP while defenses"<<std::endl;
        if((*currentDefense)->position.x == cellInPosition.x && (*currentDefense)->position.y == cellInPosition.y && (*currentDefense)->position.z == cellInPosition.z){
            esValido = false;
        }//if
        ++currentDefense;
    }//while

    //2.2   -Un obstaculo? AQUIIIIIII
    List<Object*> ::iterator currentObstacle = obstacles.begin();
    while (currentObstacle != obstacles.end())
    {
        std::cout<<"STOP while obstacles"<<std::endl;
        if((*currentObstacle)->position.x == cellInPosition.x && (*currentObstacle)->position.y == cellInPosition.y && (*currentObstacle)->position.z == cellInPosition.z){
            esValido =  false;
        }//if 
        ++currentObstacle;
    }//while
    return esValido;
}//fin_funcion

void DEF_LIB_EXPORTED placeDefenses(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , std::list<Object*> obstacles, std::list<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 

    int maxAttemps = 1000;
    List<Defense*>::iterator currentDefense = defenses.begin();
    while(currentDefense != defenses.end() && maxAttemps > 0) {

        int row = ((int)(_RAND2(nCellsWidth))) * cellWidth + cellWidth * 0.5f;
        int col = ((int)(_RAND2(nCellsHeight))) * cellHeight + cellHeight * 0.5f;

        std::cout<<"hola caracola"<<std::endl;

        bool diosito_de_mis_amores = factibilidad(row, col,(*currentDefense), obstacles, defenses, freeCells, mapHeight, mapWidth, nCellsWidth, nCellsHeight);
    
        if (diosito_de_mis_amores == true)
        {

            
            (*currentDefense)->position.x = ((int)(_RAND2(nCellsWidth))) * cellWidth + cellWidth * 0.5f;
            (*currentDefense)->position.y = ((int)(_RAND2(nCellsHeight))) * cellHeight + cellHeight * 0.5f;
            (*currentDefense)->position.z = 0; 
            ++currentDefense;
        }//if
    }

#ifdef PRINT_DEFENSE_STRATEGY

    float** cellValues = new float* [nCellsHeight]; 
    for(int i = 0; i < nCellsHeight; ++i) {
       cellValues[i] = new float[nCellsWidth];
       for(int j = 0; j < nCellsWidth; ++j) {
           cellValues[i][j] = ((int)(cellValue(i, j))) % 256;
       }
    }
    dPrintMap("strategy.ppm", nCellsHeight, nCellsWidth, cellHeight, cellWidth, freeCells
                         , cellValues, std::list<Defense*>(), true);

    for(int i = 0; i < nCellsHeight ; ++i)
        delete [] cellValues[i];
	delete [] cellValues;
	cellValues = NULL;

#endif
}
//--------------------------------PROBLEMA 2-----------------------------------------------------------------------
// -------------------------------FUNCION DE FACTIBILIDAD----------------------------------------------------------
//Para la funcion de factibilidad hay que tener en cuenta dos factores que haran si es haral una celda o no:
//  -Que la celda este dentro del mapa.
//  -Que la celda no este ocupada por un obstaculo o una defensa, es decir, que este libre.


