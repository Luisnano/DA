// ###### Config options ################


// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

using namespace Asedio;

/*void DEF_LIB_EXPORTED selectDefenses(std::list<Defense*> defenses, unsigned int ases, std::list<int> &selectedIDs
            , float mapWidth, float mapHeight, std::list<Object*> obstacles) {

    unsigned int cost = 0;
    std::list<Defense*>::iterator it = defenses.begin();
    while(it != defenses.end()) {
        if(cost + (*it)->cost <= ases) {
            selectedIDs.push_back((*it)->id);
            cost += (*it)->cost;
        }
        ++it;
    }
}*/

/*--------------------------------PROBLEMA 1----------------------------------------------*/
/*Formalice a continuación y describa la función que asigna un determinado valor a cada uno de los tipos de
defensas*/

struct defensa_valoracion{

    Defense * d;

    float valoracion;

    defensa_valoracion(Defense * d_, float v):d(d_),valoracion(v){}

    //ordenaremos segun el coste de la defensa (almacenado en defensa.cost) para una correcta ordenacion de los datos
    bool operator < (const defensa_valoracion& b){

        return d->cost < b.d->cost;

    }

};

//asigna el valor de cada una de las defensas
std::list<defensa_valoracion> defense_value(std::list<Defense*>&defenses){
    std::list<defensa_valoracion> def_value_list;

    //el criterio para seleccionar cual es mejor es la suma del daño + la salud + el alcance menos el coste
    for (auto i : defenses)
    {
        def_value_list.push_back(defensa_valoracion(i,(i->damage+i->health+i->range)-i->cost));
    }

    return def_value_list;
}
