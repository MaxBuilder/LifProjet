//
// Created by Marc on 13/03/2021.
//

#ifndef ASTAR_PROJET_ASTARTILE_H
#define ASTAR_PROJET_ASTARTILE_H

#include "../Map/TilesMap.hpp"



class AstarTile{
private:
public:
    AstarTile();
    int cout;
    char color;
    double dist;
    sf::Vector2i coordNoeud;
    AstarTile *parent;

    void setCoordinate(int x,int y);
};




#endif //ASTAR_PROJET_ASTARTILE_H
