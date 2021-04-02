//
// Created by Marc on 13/03/2021.
//

#include "PathfindingTile.hpp"

PathfindingTile::PathfindingTile(){
    cout = 0;
    dist = 0;
    color = 'w';
    parent = nullptr;
}

void PathfindingTile::setCoordinate(int x, int y) {
    coordNoeud.x = x;
    coordNoeud.y = y;
}

