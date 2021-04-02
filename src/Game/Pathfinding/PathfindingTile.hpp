//
// Created by Marc on 13/03/2021.
//

#ifndef ASTAR_PROJET_ASTARTILE_H
#define ASTAR_PROJET_ASTARTILE_H

#include "../Map/TilesMap.hpp"



class PathfindingTile {
private:
public:
    PathfindingTile();
    int cout;
    char color;
    double dist;
    sf::Vector2i coordNoeud;
    PathfindingTile *parent;

    void setCoordinate(int x,int y);
};




#endif //ASTAR_PROJET_ASTARTILE_H
