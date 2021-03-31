//
// Created by thibaut on 01/02/2021.
//

#include "Tile.hpp"
#include <cassert>

Tile::Tile(){
    ground = sf::Vector2i(0,0);
    top = sf::Vector2i(0,0);
    rotate = 0;
    moveSpeed = 1;
    topMoveSpeed = 1;
    crossable = false;
    topCrossable = true;
    mHaveTop = false;
}
void Tile::paint(const sf::Vector2i &id,const float &rotation ){

    setRotation(rotation);

    if(id.x < 0 or id.y < 0 ){
        mHaveTop = false;
        ground = sf::Vector2i(0,0);
        top  = sf::Vector2i(0,0);
    }
    else if( id.x == 0 and id.y == 0){
        if (mHaveTop)
            mHaveTop = false;
        else
            ground = id;
    }
    else if (id.y < 21)
        ground = id;
    else {
        top = id;
        mHaveTop = true;
    }

    if (id.x <= 0 and id.y <= 0){ // pas de texture
        moveSpeed = 0.f;
        crossable = false;
    }
    else if ( id == sf::Vector2i(0,1) or id == sf::Vector2i(0,2) or
            id == sf::Vector2i(1,4) or id == sf::Vector2i(1,10) or
            id == sf::Vector2i(0,13)) { // chemin de terre
        moveSpeed = 1.1f;
        crossable = true;
    }else if ( id.x == 2 and (id.y < 3 or id.y == 20 or id.y == 19) ) { // eau
        moveSpeed = 0.2f;
        crossable = false;
    }else if(id.y<6) {
        moveSpeed = 0.85f;
        crossable = true;
    }else if((id.y == 15 or id.y ==16) and (id.x == 0 or id.x == 1)){
        moveSpeed = 0.85f;
        crossable = true;
    }else if (id.y < 21 ){ // reste sol ( terre / transition )
        moveSpeed = 1.f;
        crossable = false;
    }else if( id.y < 26 ){ // ponts, contour forets
        topMoveSpeed = 0.85f;
        topCrossable = true;
    }else if (id.y < 46){ // batiment + montagnes + foret
        topMoveSpeed = 0.6f;
        topCrossable = false;
    }else{
        topMoveSpeed = 0.6f;
        topCrossable = true;
    }

}


sf::Vector2i Tile::getGround() const{
    return ground;
}

sf::Vector2i Tile::getTop() const{
    return top;
}

bool Tile::haveTop() const {
    return mHaveTop;
}

float Tile::getMoveSpeed() const{
    if (mHaveTop)
        return topMoveSpeed;
    else
        return moveSpeed;
}

bool Tile::isCrossable() const{
    if(mHaveTop) {

        if (top.x == 2 and (top.y == 38 or top.y == 39))
            return true;
        if (top.x == 2 and (top.y == 36 or top.y == 37))
            return true;

        return topCrossable;
    }
    else
        return crossable;
}

bool Tile::isCrossable(Editor::Tool team) const{
    if(mHaveTop) {
        if (top.x == 2 and (top.y == 38 or top.y == 39) and team == Editor::Tool::blueTeam)
            return true;
        if (top.x == 2 and (top.y == 36 or top.y == 37) and team == Editor::Tool::redTeam)
            return true;

        return topCrossable;
    }
    else
        return crossable;
}

float Tile::getRotation() const{
    return rotate;
}

void Tile::setRotation(const float &rotation){
    if (rotation >= 0) rotate = rotation;
    else rotate = static_cast<float>((std::rand()%3)*90);
}