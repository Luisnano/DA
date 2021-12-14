// Luis Fernando Torres Moya
// Practica 3

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include "cronometro.h"
     
using namespace Asedio;

//cellCenterToPosition lo que hara es crear una posicion con la que podamos trabajar del centro de la celda.

Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){

    return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0);

}

//positionToCell hace todo lo contrario, es decir pasa de la posicion a la celda

void positionToCell(const Vector3 pos, int &i_out, int &j_out, float cellWidth, float cellHeight)
{
    i_out = (int)(pos.y * 1.0f / cellHeight);
    j_out = (int)(pos.x * 1.0f / cellWidth);
}

//----------------------------------ALGORITMOS DE EL EXAMPLE.CPP-------------------------------------------------------

float defaultCellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
    , float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
    	
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;

    Vector3 cellPosition((col * cellWidth) + cellWidth * 0.5f, (row * cellHeight) + cellHeight * 0.5f, 0);
    	
    float val = 0;
    for (List<Object*>::iterator it=obstacles.begin(); it != obstacles.end(); ++it) {
	    val += _distance(cellPosition, (*it)->position);
    }

    return val;
}

void DEF_LIB_EXPORTED placeDefenses3(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , List<Object*> obstacles, List<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 

	cronometro c;
    long int r = 0;
    c.activar();
    do {	
		List<Defense*>::iterator currentDefense = defenses.begin();
		while(currentDefense != defenses.end() && maxAttemps > 0) {

			(*currentDefense)->position.x = ((int)(_RAND2(nCellsWidth))) * cellWidth + cellWidth * 0.5f;
			(*currentDefense)->position.y = ((int)(_RAND2(nCellsHeight))) * cellHeight + cellHeight * 0.5f;
			(*currentDefense)->position.z = 0; 
			++currentDefense;
		}
		
		++r;
    } while(c.tiempo() < 1.0);
    c.parar();
    std::cout << (nCellsWidth * nCellsHeight) << '\t' << c.tiempo() / r << '\t' << c.tiempo()*2 / r << '\t' << c.tiempo()*3 / r << '\t' << c.tiempo()*4 / r << std::endl;
}

//------------------------------------ALGORITMO DE LA PRACTICA 1----------------------------

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