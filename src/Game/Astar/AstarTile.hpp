//
// Created by Marc on 13/03/2021.
//

#ifndef ASTAR_PROJET_ASTARTILE_H
#define ASTAR_PROJET_ASTARTILE_H

#include "../Map/TilesMap.hpp"

struct coordinate{
    int x,y;
};

class AstarTile{
private:
public:
    AstarTile();
    int cout;
    char color;
    double dist;
    float moveSpeed;
    bool isCrossable;
    coordinate coordNoeud;
    AstarTile *parent;

    float getMoveSpeed() const;
    void setMoveSpeed(const float &speed);
    void setColor(char col);
    void setCoordinate(int x,int y);
};




#endif //ASTAR_PROJET_ASTARTILE_H