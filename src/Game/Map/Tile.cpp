//
// Created by thibaut on 01/02/2021.
//

#include "Tile.hpp"
#include <cassert>
#include <iostream>

Tile::Tile(){
    ground = sf::Vector2i(0,0);
    top = sf::Vector2i(0,0);
    rotate = 0;
    moveSpeed = 1;
    topMoveSpeed = 1;
    crossable = false;
    topCrossable = true;
}
void Tile::paint(const sf::Vector2i &id,const float &rotation ){
    if( id.x == 0 and id.y == 0){
        ground = id;
        top = id;
    }
    else if (id.y < 21)
        ground = id;
    else {
        std::cout<<"set Top : "<<id.x<<" "<<id.y<<std::endl;
        top = id;
    }

    if (rotation >= 0) rotate = rotation;
    else rotate = static_cast<float>((std::rand()%3)*90);

    if ( ground.x == 1 and ground.y == 0) {
        moveSpeed = 1.f;
        crossable = true;
    }else if ( ground.y < 6){
        moveSpeed = 0.7;
        crossable = true;
    }else if ( ground.y < 16){
        moveSpeed = 1.3;
        crossable = true;
    }else if ( ground.y < 21){
        moveSpeed = 1;
        crossable = false;
    }else if ( ground.y < 26){
        topMoveSpeed = 1;
        topCrossable = true;
    }else if ( ground.y < 31){
        topMoveSpeed = 0.3;
        topCrossable = false;
    }

}


sf::Vector2i Tile::getGround() const{
    return ground;
}

sf::Vector2i Tile::getTop() const{
    return top;
}

float Tile::getMoveSpeed() const{
    return moveSpeed*topMoveSpeed;
}

bool Tile::isCrossable() const{
    return (crossable and topCrossable);
}

void Tile::setGround(const sf::Vector2i &id){
    ground = id;
}

void Tile::setMoveSpeed(const float &speed){
    assert(speed >= 0);
    moveSpeed = speed;
}

void Tile::setcrossable(const bool &cross){
    crossable = cross;

}

float Tile::getRotation() const{
    return rotate;
}

void Tile::setRotation(const float &rotation){
    assert(rotation == 0 or rotation == 90 or rotation == 180 or rotation == 270);
    rotate = rotation;
}