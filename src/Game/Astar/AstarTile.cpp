//
// Created by Marc on 13/03/2021.
//

#include "AstarTile.hpp"

AstarTile::AstarTile(){
    cout = 0;
    dist = 0;
    color = 'w';
    parent = nullptr;
}

void AstarTile::setCoordinate(int x, int y) {
    coordNoeud.x = x;
    coordNoeud.y = y;
}

