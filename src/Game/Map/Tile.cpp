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
    topCrossable = false;
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

    if (ground.x <= 0 and ground.y <= 0){ // pas de texture
        moveSpeed = 0.f;
        crossable = false;
    }
    else if ( ground == sf::Vector2i(0,1) or ground == sf::Vector2i(0,2) or
              ground == sf::Vector2i(1,4) or ground == sf::Vector2i(1,10) or
              ground == sf::Vector2i(0,13)) { // chemin de terre
        moveSpeed = 1.3f;
        crossable = true;
    }else if ( ground.x == 2 and (ground.y < 3 or ground.y == 20 or ground.y == 19) ){ // eau
        moveSpeed = 0.2f;
        crossable = false;
    }else if (ground.y < 21 ){ // reste sol ( terre / transition )
        moveSpeed = 1.f;
        crossable = true;
    }else if( ground.y < 26 ){ // ponts, contour forets
        topMoveSpeed = 0.85f;
        topCrossable = true;
    }else{ // batiment + montagnes + foret
        topMoveSpeed = 0.2f;
        topCrossable = false;
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
        return moveSpeed*topMoveSpeed;
    else
        return moveSpeed;
}

bool Tile::isCrossable() const{
    if(mHaveTop)
        return topCrossable;
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